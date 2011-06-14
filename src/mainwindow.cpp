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
#include <KConfigDialog>
#include <KConfigSkeleton>
#include <KLineEdit>
#include <KUrlRequester>
#include <KUrl>
#include <KGlobalSettings>
#include <KDebug>

#include "KamerkaSettings.h"
#include "mainwindow.h"
#include "settingsdialog.h"

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
    if (count==5) {
      videoViewer->media->setCurrentSource(KStandardDirs::locate("data", "kamerka/timer.ogg"));
      videoViewer->media->play();
    }
}

// slot for UI button - open file manager
void MainWindow::showDirectory() {
    QDir dir(KGlobalSettings::picturesPath());
    dir.mkpath("kamerka");
    QProcess::startDetached("kde-open", QStringList() << KGlobalSettings::picturesPath() + "/kamerka");
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
    conf->setGeometry(QRectF(QPointF(50,50), QPointF(this->size().rwidth()-50,this->size().rheight()-50)));
    QMainWindow::resizeEvent(e);
}

MainWindow::MainWindow() {
    // register QML effects
    qmlRegisterType<QGraphicsBlurEffect>("Effects",1,0,"Blur");
    qmlRegisterType<QGraphicsDropShadowEffect>("Effects",1,0,"DropShadow");

    ui = new QDeclarativeView;
    connect(ui, SIGNAL(statusChanged(QDeclarativeView::Status)), this, SLOT(QMLStatus(QDeclarativeView::Status)));

    videoViewer = new videowidget(this);
    videoViewer->ui = ui;
    videoViewer->show();

    this->setCentralWidget(ui);

    if (videoViewer->thread.start()) {
        // if opening V4L device failed:
        KMessageBox::error(this, i18n("Could not connect to V4L device!"), i18n("Error"), KMessageBox::Dangerous);
        close();
    }

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

    // connect UI button signals to slots in this class
    connect(ui->rootObject(), SIGNAL(takePhoto()), this, SLOT(takePhoto()));
    connect(ui->rootObject(), SIGNAL(timerCounter(int)), this, SLOT(timerCounter(int)));
    connect(ui->rootObject(), SIGNAL(showDirectory()), this, SLOT(showDirectory()));
    connect(ui->rootObject(), SIGNAL(showConfiguration()), this, SLOT(showConfiguration()));

    // setup configuration window
    SettingsDialog *confdial = new SettingsDialog(0, i18n("Settings"), KamerkaSettings::self(), ui->scene());
    conf = confdial->proxy;
    connect(confdial, SIGNAL(hidden()), this, SLOT(closeCanvasLayer()));
}
