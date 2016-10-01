/*
 * Copyright (c) Özkan Pakdil <ozkan.pakdil@gmail.com>
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

#include <QStandardPaths>
#include <QProcess>
#include <QQmlContext>
#include <QDir>
#include <QQuickItem>
#include <KLocalizedString>
#include <QDesktopServices>
#include <phonon/AudioOutput>

#include "settings.h"
#include "videowidget.h"

// Notification class is used to connect to notification signal
Notification::Notification (QString name, QString filename) : KNotification(name) {
    this->filename = filename;
}

void Notification::openFile (unsigned int i) {
    // qDebug(QString::number(i).toStdString().c_str());
    if (i==1) {
        QDesktopServices::openUrl(this->filename);
    }
    else if (i==2) {
        if (!QProcess::startDetached("dolphin", {"--select", this->filename})) {
            QDesktopServices::openUrl(Settings::photodir());
        }
    }
}

videowidget::videowidget(QWidget *parent) : QWidget(parent) {
    storeImage=false;
    connect(&thread, SIGNAL(renderedImage(QImage)),
            this, SLOT(setPicture(QImage)));

    setAutoFillBackground(true);

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHeightForWidth(true);
    setSizePolicy(sizePolicy);

    media = new Phonon::MediaObject(this);
    Phonon::createPath(media, new Phonon::AudioOutput(Phonon::NotificationCategory, this));
}

void videowidget::resize(const QSize& size) {
    if (Settings::aspectlock()) {
        QSize s(size);

        double aspect = Settings::width()/(double)Settings::height();

        if (s.width()>s.height()*aspect) {
            s.setWidth(s.height()*aspect);
        } else {
            s.setHeight(s.width()/aspect);
        }

        QSize p = this->parentWidget()->size();
        QWidget::setGeometry((p.width()-s.width())/2,(p.height()-s.height())/2, s.width(), s.height());
    } else {
        QWidget::setGeometry(0, 0, size.width(), size.height());
        //QWidget::resize(size);
    }
}


videowidget::~videowidget() {
    if (thread.isRunning())
        thread.stop();
}

void videowidget::takeImage() {
    storeImage = true;
    imageDelay = 3;
}

// draw picture from webcam (pixmap) on repaint
void videowidget::paintEvent(QPaintEvent *) {
    QPainter* painter = new QPainter(this);
    painter->setPen(Qt::white);
    painter->setFont(QFont("Arial", 30));
    painter->drawText(rect(), Qt::AlignCenter, i18n("Starting up webcam..."));

    painter->drawPixmap(this->rect(), pixmap);
    delete painter;
}

// image was transfered from capturethread to us - display it and, if requested, store
void videowidget::setPicture(QImage i) {

    pixmap=QPixmap::fromImage(i);
    update();

    if (storeImage) {
        imageDelay--;
        if (imageDelay) {
            return;
        }
        // we're taking a photo!
        QDir dir;
        dir.mkpath(Settings::photodir());
        dir.setPath(Settings::photodir());

        // play sound
        if (Settings::soundontaking()) {
            media->setCurrentSource(QUrl::fromLocalFile(QStandardPaths::locate(QStandardPaths::GenericDataLocation, "kamerka/camera_click.ogg")));
            media->play();
        }

        // check, which number comes next (so we're able to set correct file name)
        int c = 0;
        QString counterfilename;
        counterfilename = dir.absoluteFilePath(".counter");

        QFile counterfile(counterfilename.toStdString().c_str());
        if (counterfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream counter(&counterfile);
            counter >> c;
        }
        else qWarning() << "Could not open .counter file!";
        counterfile.close();

        // determine file name
        QString imagepath;
        do {
            c++;
            imagepath = "image";
            imagepath += QString::number(c);
            imagepath += ".png";
            imagepath = dir.absoluteFilePath(imagepath);
        } while (QFile::exists(imagepath));

        // store incremented value in counter file
        counterfile.open(QIODevice::WriteOnly);
        QTextStream counter(&counterfile);
        counter << c;
        counterfile.close();

        // save image
        qDebug() << QString("%1").arg(imagepath);
        i.save(imagepath, "PNG");



        // show taken photo and trigger animation in QML UI
        ui->rootContext()->setContextProperty("fileName", "file:"+imagepath);
        QMetaObject::invokeMethod(ui->rootObject(), "photoTaken");

        // display notification
        if (Settings::notification()) {
            Notification* notification = new Notification("photoTaken", imagepath);
            QString s = i18n("Photo has been stored in file %1", imagepath);
            QPixmap pixmap = QPixmap::fromImage(i);
            notification->setText( s );
            notification->setPixmap( pixmap );
            QStringList list;
            list << i18n("Open") << i18n("Show in directory");
            notification->setActions( list );
            notification->setFlags(KNotification::SkipGrouping);
            connect(notification, SIGNAL(activated(unsigned int)), notification , SLOT(openFile(unsigned int)) );
            notification->sendEvent();
        }

        // we don't want to store next frames too
        storeImage=false;
    }
}
