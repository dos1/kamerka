/*
 * Copyright (c) Sebastian Krzyszkowiak <dos@dosowisko.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <QtScript/QScriptContext>
#include <KLocalizedString>
#include <KStandardDirs>
#include <KMessageBox>

#include "mainwindow.h"
#include "settings.h"

// part of QML hack to access script engine in rw mode
void EngineAccess::setEngine(QScriptValue val) {
    this->engine = val.engine();
}

// allow QML files to be translated
QScriptValue jsi18n(QScriptContext *context, QScriptEngine *engine) {
    Q_UNUSED(engine)
    if (context->argumentCount() < 1) {
        kWarning() << i18n("i18n() takes at least one argument");
        return engine->undefinedValue();
    }
    KLocalizedString message = ki18n(context->argument(0).toString().toUtf8());
    const int numArgs = context->argumentCount();
    for (int i = 1; i < numArgs; ++i) {
        message = message.subs(context->argument(i).toString());
    }
    return message.toString();
}

// show error when loading QML failed
void MainWindow::QMLStatus(QDeclarativeView::Status status) {
    if (status==QDeclarativeView::Error) {
        QString errors = "";
        for (int i=0; i<ui->errors().size(); i++) {
            errors += ui->errors()[i].toString() +'\n';
        }
        KMessageBox::detailedError(this, i18n("Could not load QML interface!"), errors, i18n("Error"), KMessageBox::Dangerous);
        close();
    }
}

// slot for UI button - scheldule photo to be taken from next processed frame
void MainWindow::takePhoto() {
    videoViewer->storeImage=true;
}

// slot for UI button - play timer sound
void MainWindow::timerCounter(int count) {
    //kDebug(QString::number(count).toStdString().c_str());
    if ((count==5) && (Settings::soundontimer())) {
      videoViewer->media->setCurrentSource(KStandardDirs::locate("data", "kamerka/timer.ogg"));
      videoViewer->media->play();
    }
}

// slot for UI button - open file manager
void MainWindow::showDirectory() {
    QDir dir;
    dir.mkpath(Settings::photodir());
    dir.setPath(Settings::photodir());
    QProcess::startDetached("xdg-open", QStringList() << dir.absolutePath());
    QApplication::quit();
}

void MainWindow::closeCanvasLayer() {
    QMetaObject::invokeMethod(ui->rootObject(), "hideCanvasBackground");
    conf->hide();
}

int MainWindow::opacityUpdate(QGraphicsProxyWidget *widget) {
    widget->setOpacity(widget->opacity()+0.05);
    if (widget->opacity()>=1) return 0;
    return 1;
}

void MainWindow::opacityUpdateConf() {
    if (opacityUpdate(conf)) {
        QTimer::singleShot(5, this, SLOT(opacityUpdateConf()));
    }
}


// slot for UI button - show canvas layer with configuration
void MainWindow::showConfiguration() {
    QMetaObject::invokeMethod(ui->rootObject(), "showCanvasBackground");
    conf->setOpacity(0);
    conf->show();
    QTimer::singleShot(500, this, SLOT(opacityUpdateConf()));
}

// resize video widget together with window
void MainWindow::resizeEvent(QResizeEvent *e) {
    videoViewer->resize(this->size());
    if (conf)
        conf->setGeometry(QRectF(QPointF(50,50), QPointF(this->size().rwidth()-50,this->size().rheight()-50)));
    if (dialog)
        dialog->setGeometry(QRectF(QPointF(0,0), QPointF(this->size().rwidth(), dialog->size().rheight())));
    QMainWindow::resizeEvent(e);
}

void MainWindow::loadSettings() {
    if (videoViewer->thread.running) {
        videoViewer->thread.stop();
    }
    if (videoViewer->thread.start()) {
        // if opening V4L device failed:
        //KMessageBox::error(this, i18n("Could not connect to V4L device!"), i18n("Error"), KMessageBox::Dangerous);
        if (this->isVisible()) {
            dialoglabel->setText( i18n("Could not connect to V4L device!") );
            dialog->show();
        }
        else {
            KMessageBox::error(this, i18n("Could not connect to V4L device!"), i18n("Error"), KMessageBox::Dangerous);
        }
    }
}

void MainWindow::tryVideoThread() {
    confdial = new SettingsDialog(0, "settings", Settings::self());
    if ((videoViewer->thread.running==false) && (videoViewer->thread.start())) {
        // if opening V4L device failed:
        if (first) {
            first = false;
            KMessageBox::error(this, i18n("Could not connect to V4L device!"), i18n("Error"), KMessageBox::Dangerous);
        }
        confdial->setFaceType(KConfigDialog::Plain);
        KConfigDialog::showDialog("settings");
        connect(confdial, SIGNAL(cancelClicked()), this, SLOT(close()));
        connect(confdial, SIGNAL(closeClicked()), this, SLOT(close()));
        connect(confdial, SIGNAL(okClicked()), this, SLOT(tryVideoThread()));
    }
    else {
        conf = ui->scene()->addWidget(confdial);
        KConfigDialog::showDialog("settings");
        conf->hide();
        connect(confdial, SIGNAL(hidden()), this, SLOT(closeCanvasLayer()));
        // drop shadow
        QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
        shadow->setOffset(QPointF(0, 0));
        shadow->setBlurRadius(8);
        shadow->setColor(QColor(255,255,255));
        conf->setGraphicsEffect(shadow);
        // show main window
        this->show();
    }
    connect(confdial, SIGNAL(settingsChanged(const QString&)), this, SLOT(loadSettings()));
}

void MainWindow::startedCapture(int width, int height) {
    kDebug() << QString("Driver is sending image at %1x%2").arg(
            QString::number(width), QString::number(height));
    if ((width!=Settings::width()) || (height!=Settings::height())) {
        dialoglabel->setText(i18n("Requested resolution (%1x%2) was not available. Driver used %3x%4 instead.\nPlease check your configuration.", Settings::width(), Settings::height(), width, height));
        dialog->show();
    }
    else dialog->hide();
}

MainWindow::MainWindow() {
    // initialize variables
    conf = NULL;
    confdial = NULL;

    // register QML effects
    qmlRegisterType<QGraphicsBlurEffect>("Effects",1,0,"Blur");
    qmlRegisterType<QGraphicsDropShadowEffect>("Effects",1,0,"DropShadow");

    // setup user interface
    ui = new QDeclarativeView;
    connect(ui, SIGNAL(statusChanged(QDeclarativeView::Status)), this, SLOT(QMLStatus(QDeclarativeView::Status)));

    videoViewer = new videowidget(this);
    videoViewer->ui = ui;
    videoViewer->show();

    this->setCentralWidget(ui);

    //Glorious hack:steal the engine - thanks for KDeclarative, from which I stole this code! :)
    //create the access object
    EngineAccess *engineAccess = new EngineAccess();
    ui->engine()->rootContext()->setContextProperty("__engineAccess", engineAccess);
    //make engineaccess set our engine
    QDeclarativeExpression *expr = new QDeclarativeExpression(ui->engine()->rootContext(), ui->engine()->rootContext()->contextObject(), "__engineAccess.setEngine(this)");
    expr->evaluate();
    delete expr;

    ui->engine()->rootContext()->setContextProperty("__engineAccess", 0);

    //change the old globalobject with a new read/write copy
    QScriptValue originalGlobalObject = engineAccess->engine->globalObject();

    QScriptValue newGlobalObject = engineAccess->engine->newObject();

    QString eval = QLatin1String("eval");
    QString version = QLatin1String("version");
    {
        QScriptValueIterator iter(originalGlobalObject);
        QVector<QString> names;
        QVector<QScriptValue> values;
        QVector<QScriptValue::PropertyFlags> flags;
        while (iter.hasNext()) {
            iter.next();
            QString name = iter.name();
            if (name == version) {
                continue;
            }
            if (name != eval) {
                names.append(name);
                values.append(iter.value());
                flags.append(iter.flags() | QScriptValue::Undeletable);
            }
            newGlobalObject.setProperty(iter.scriptName(), iter.value());
        }
    }
    engineAccess->engine->setGlobalObject(newGlobalObject);
    engineAccess->engine->globalObject().setProperty("i18n", engineAccess->engine->newFunction(jsi18n));
    delete engineAccess;
    // end of hack

    ui->rootContext()->setContextProperty("fileName", "qrc:/icons/kamerka.png");
    ui->setSource(KUrl("qrc:/qml/kamerka.qml"));

    // let widgets have transparent background
    ui->setStyleSheet("background: transparent");
    videoViewer->setStyleSheet("background: transparent");

    // resize QML UI together with window
    ui->setResizeMode(QDeclarativeView::SizeRootObjectToView);

    // setup info dialog on top of screen
    kdialog = new KDialog( this );
    kdialog->setButtons( KDialog::Ok );
    dialoglabel = new QLabel();
    dialoglabel->setWordWrap(true);
    dialoglabel->setAlignment(Qt::AlignCenter);
    kdialog->setMainWidget( dialoglabel );
    kdialog->setStyleSheet("QDialog { background-color: rgba(64,64,64,64); border-bottom: 1px solid white; } QLabel { color: white; }");
    dialog = ui->scene()->addWidget(kdialog);
    dialog->hide();
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setOffset(QPointF(0, 0));
    shadow->setBlurRadius(8);
    dialog->setGraphicsEffect(shadow);

    // connect UI button signals to slots in this class
    connect(ui->rootObject(), SIGNAL(takePhoto()), this, SLOT(takePhoto()));
    connect(ui->rootObject(), SIGNAL(timerCounter(int)), this, SLOT(timerCounter(int)));
    connect(ui->rootObject(), SIGNAL(showDirectory()), this, SLOT(showDirectory()));
    connect(ui->rootObject(), SIGNAL(showConfiguration()), this, SLOT(showConfiguration()));

    connect(&(videoViewer->thread), SIGNAL(startedCapture(int, int)), this, SLOT(startedCapture(int, int)));

    // capture from webcam & setup configuration
    first = true;
    tryVideoThread();
}
