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

#include <KNotification>
#include <KLocale>
#include <Phonon/AudioOutput>
#include <KLocalizedString>
#include <KStandardDirs>
#include <KGlobalSettings>
#include <KDebug>

#include "videowidget.h"

videowidget::videowidget(QWidget *parent) :
    QWidget(parent)
{
    storeImage=false;
    connect(&thread, SIGNAL(renderedImage(QImage)),
                this, SLOT(setPicture(QImage)));

    setAutoFillBackground(true);
    media = new Phonon::MediaObject(this);
    Phonon::createPath(media, new Phonon::AudioOutput(Phonon::NotificationCategory, this));
}

videowidget::~videowidget() {
    thread.stop();
}

void videowidget::paintEvent(QPaintEvent */*event*/) {
    QPainter* painter = new QPainter(this);
    painter->setPen(Qt::white);
    painter->setFont(QFont("Arial", 30));
    painter->drawText(rect(), Qt::AlignCenter, i18n("Starting up webcam..."));

    painter->drawPixmap(this->rect(),pixmap);
    delete painter;
}

void Focia::setFilename ( QString name ) {
  this->filename = name;
}

void Focia::openFile (unsigned int i) {
  // kDebug(QString::number(i).toStdString().c_str());
  QString app;
  QStringList arg;
  if (i==1) {
    app = "dolphin";
    arg << "--select";
  }
  else if (i==2) {
    app = "gimp";
  }
  else {
    app = "inkscape";
  }
  arg << this->filename;
  QProcess::startDetached(app, arg);
}


void videowidget::setPicture(QImage i){
  if (storeImage) {
    QDir dir(KGlobalSettings::picturesPath());
    dir.mkdir("kamerka");
    media->setCurrentSource(KStandardDirs::locate("data", "kamerka/camera_click.ogg"));
    media->play();

    int c = 0;
    QString counterfilename;
    counterfilename = KGlobalSettings::picturesPath() + "/kamerka/.counter";

    QFile counterfile(counterfilename.toStdString().c_str());
    if (counterfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
          QTextStream counter(&counterfile);
          counter >> c;
    }
    else kWarning() << "Could not open .counter file!";
    c++;
    counterfile.close();

    counterfile.open(QIODevice::WriteOnly);
    QTextStream counter(&counterfile);
    counter << c;
    counterfile.close();

    QString imagepath;
    imagepath = KGlobalSettings::picturesPath() + "/kamerka/image";
    imagepath += QString::number(c);
    imagepath += ".png";
    kDebug() << QString("%s").arg(imagepath);
    i.save(imagepath, "PNG");

    ui->rootContext()->setContextProperty("fileName", "file:"+imagepath);
    QMetaObject::invokeMethod(ui->rootObject(), "fotkaZrobiona");

    Focia* fotka = new Focia;
    fotka->setFilename(imagepath);

    QString s = i18n("Photo has been stored in file %1", imagepath);

    QPixmap pixmap = QPixmap::fromImage(i);

    KNotification *notification= new KNotification ( "takenPhoto" );
    notification->setText( s );
    notification->setPixmap( pixmap );
    QStringList lista;
    lista << i18n("Show in directory") << i18n("Open in GIMP") << i18n("Open in Inkscape");
    notification->setActions( lista );
    connect(notification, SIGNAL(activated(unsigned int)), fotka , SLOT(openFile(unsigned int)) );
    notification->sendEvent();

    storeImage=false;
  }

  pixmap=QPixmap::fromImage(i);
  update();
}
