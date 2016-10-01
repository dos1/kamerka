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
#include <QStandardPaths>
#include <KMessageBox>
#include <QtQml/QQmlError>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlExpression>
#include <QtQuick/QQuickItem>
#include <QtQuickWidgets/QQuickWidget>
#include <QScriptValueIterator>
#include <KDeclarative/KDeclarative>
#include <QUrl>

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
        qWarning() << i18n("i18n() takes at least one argument");
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
void MainWindow::QMLStatus(QQuickWidget::Status status) {
    if (status==QQuickWidget::Error) {
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
    //qDebug(QString::number(count).toStdString().c_str());
    if ((count==5) && (Settings::soundontimer())) {
        videoViewer->media->setCurrentSource(QUrl::fromLocalFile(QStandardPaths::locate(QStandardPaths::GenericDataLocation, "kamerka/timer.ogg")));
        videoViewer->media->play();
    }
}

// slot for UI button - open file manager
void MainWindow::showDirectory() {
    QDir dir;
    dir.mkpath(Settings::photodir());
    dir.setPath(Settings::photodir());
    QProcess::startDetached("xdg-open", QStringList() << dir.absolutePath());
}

// slot for UI button
void MainWindow::showConfiguration() {
    confdial->show();
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
            kdialog->show();
        }
        else {
            KMessageBox::error(this, i18n("Could not connect to V4L device!"), i18n("Error"), KMessageBox::Dangerous);
        }
    }
    videoViewer->ui->rootObject()->setProperty("burstPhotoNumber", Settings::burstnumphotos());
    videoViewer->ui->rootObject()->setProperty("delayBetweenPhotosBurst", Settings::delaybetweenphotos());
    videoViewer->ui->rootObject()->setProperty("selftimer", Settings::selftimer());
    videoViewer->resize(this->size());
}

void MainWindow::applyEffect(int effect) {
    videoViewer->thread.effect = effect;
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
        if (confdial->exec()) {
            tryVideoThread();
        } else {
            close();
        }
    }
    else {
        this->show();
    }
    connect(confdial, SIGNAL(settingsChanged(const QString&)), this, SLOT(loadSettings()));
}

void MainWindow::startedCapture(int width, int height) {
    qDebug() << QString("Driver is sending image at %1x%2").arg(
                    QString::number(width), QString::number(height));
    if ((width!=Settings::width()) || (height!=Settings::height())) {
        dialoglabel->setText(i18n("Requested resolution (%1x%2) was not available. Driver used %3x%4 instead.\nPlease check your configuration.", Settings::width(), Settings::height(), width, height));
        kdialog->show();
    }
    else kdialog->hide();
}

MainWindow::MainWindow() {
    // initialize variables
    conf = NULL;
    confdial = NULL;
    dialog = NULL;

    videoViewer = new videowidget(this);

    // setup user interface
    ui = new QQuickWidget(this);
    ui->quickWindow()->setColor(Qt::transparent);
    ui->setAttribute(Qt::WA_AlwaysStackOnTop);

    ui->show();
    //ui->setColor(Qt::transparent);

    KDeclarative::KDeclarative kdeclarative;
    kdeclarative.setDeclarativeEngine(ui->engine());
    kdeclarative.setupBindings();

    connect(ui, SIGNAL(statusChanged(QQuickWidget::Status)), this, SLOT(QMLStatus(QQuickWidget::Status)));

    videoViewer->ui = ui;
    videoViewer->show();

    QVBoxLayout *layout = new QVBoxLayout();
    QWidget *widget = new QWidget(this);
    widget->setLayout(layout);
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    widget->show();
    this->setCentralWidget(widget);

    ui->rootContext()->setContextProperty("fileName", "qrc:/icons/kamerka.png");
    ui->setSource(QUrl("qrc:/qml/kamerka.qml"));

    // let widgets have transparent background
    //	ui->setStyleSheet("background: transparent");
    videoViewer->setStyleSheet("background: transparent");

    // Burst mode settings
    videoViewer->ui->rootObject()->setProperty("burstPhotoNumber", Settings::burstnumphotos());
    videoViewer->ui->rootObject()->setProperty("delayBetweenPhotosBurst", Settings::delaybetweenphotos());
    videoViewer->ui->rootObject()->setProperty("selftimer", Settings::selftimer());

    // resize QML UI together with window
    ui->setResizeMode(QQuickWidget::SizeRootObjectToView);

    // setup info dialog on top of screen
    kdialog = new KDialog( this );
    kdialog->setButtons( KDialog::Ok );
    dialoglabel = new QLabel(kdialog);
    dialoglabel->setWordWrap(true);
    dialoglabel->setAlignment(Qt::AlignCenter);
    kdialog->setMainWidget( dialoglabel );
    kdialog->setStyleSheet("QDialog { background-color: rgba(64,64,64,64); border-bottom: 1px solid white; } QLabel { color: white; }");
    //dialog = ui->scene()->addWidget(kdialog);
    //dialog->hide();

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setOffset(QPointF(0, 0));
    shadow->setBlurRadius(8);
    kdialog->setGraphicsEffect(shadow);
    kdialog->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
    //kdialog->show();
    layout->addWidget(kdialog);
    layout->addWidget(ui);


    // connect UI button signals to slots in this class
    connect(ui->rootObject(), SIGNAL(takePhoto()), this, SLOT(takePhoto()));
    connect(ui->rootObject(), SIGNAL(timerCounter(int)), this, SLOT(timerCounter(int)));
    connect(ui->rootObject(), SIGNAL(showDirectory()), this, SLOT(showDirectory()));
    connect(ui->rootObject(), SIGNAL(showConfiguration()), this, SLOT(showConfiguration()));
    connect(ui->rootObject(), SIGNAL(applyEffect(int)), this, SLOT(applyEffect(int)));

    connect(&(videoViewer->thread), SIGNAL(startedCapture(int, int)), this, SLOT(startedCapture(int, int)));

    resize(Settings::width(), Settings::height());

    // capture from webcam & setup configuration
    first = true;
    tryVideoThread();
}
