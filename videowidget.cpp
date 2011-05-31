/*
 * Copyright (c) Özkan Pakdil <ozkan.pakdil@gmail.com>
 * Copyright (c) Sebastian Krzyszkowiak <dos@dosowisko.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 (or higher) of the License
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#include "videowidget.h"
#include <KNotification>
#include <KLocale>
#include <phonon/AudioOutput>

videowidget::videowidget(QWidget *parent) :
    QWidget(parent)
{
    connect(&thread, SIGNAL(renderedImage(QImage)),
                this, SLOT(setPicture(QImage)));

    setAutoFillBackground(true);
    media = new Phonon::MediaObject(this);
    Phonon::createPath(media, new Phonon::AudioOutput(Phonon::NotificationCategory, this));
    media->setCurrentSource(QUrl("/usr/share/kde4/apps/kamerka/camera_click.ogg"));
}

videowidget::~videowidget() {
    delete media;
}

void videowidget::paintEvent(QPaintEvent */*event*/) {
    QPainter* painter = new QPainter(this);
    painter->setPen(Qt::white);
    painter->setFont(QFont("Arial", 30));
    painter->drawText(rect(), Qt::AlignCenter, "Uruchamianie kamery...");

    painter->drawPixmap(this->rect(),pixmap);
    delete painter;
}

void Focia::setFilename ( QString name ) {
  this->filename = name;
}

void Focia::openFile (unsigned int i) {
  // qDebug(QString::number(i).toStdString().c_str());
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
  if (thread.storeImage) { 
    QDir dir(QDir::homePath());
    dir.mkdir("kamerka");
    media->play();

    int c = 0;
    QString counterfilename;
    counterfilename = QDir::homePath() + "/kamerka/.counter";

    QFile counterfile(counterfilename.toStdString().c_str());
    if (counterfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
          QTextStream counter(&counterfile);
          counter >> c;
    }
    else qWarning("Could not open .counter file!");
    c++;
    counterfile.close();

    counterfile.open(QIODevice::WriteOnly);
    QTextStream counter(&counterfile);
    counter << c;
    counterfile.close();

    QString imagepath;
    imagepath = QDir::homePath() + "/kamerka/image";
    imagepath += QString::number(c);
    imagepath += ".png";
    qDebug("%s",imagepath.toStdString().c_str());
    i.save(imagepath, "PNG");

    ui->rootContext()->setContextProperty("fileName", "file:"+imagepath);
    QMetaObject::invokeMethod(ui->rootObject(), "fotkaZrobiona");

    Focia* fotka = new Focia;
    fotka->setFilename(imagepath);

    QString s = tr2i18n("Zdj\304\231cie zosta\305\202o zapisane do pliku ", 0) + imagepath;

    QPixmap pixmap = QPixmap::fromImage(i);

    KNotification *notification= new KNotification ( "takenPhoto" );
    notification->setText( s );
    notification->setPixmap( pixmap );
    QStringList lista;
    lista << tr2i18n("Poka\305\274 w folderze", 0) << tr2i18n("Otw\303\263rz w GIMPie", 0) << tr2i18n("Otw\303\263rz w Inkscape", 0);
    notification->setActions( lista );
    connect(notification, SIGNAL(activated(unsigned int )), fotka , SLOT(openFile(unsigned int )) );
    notification->sendEvent();

    thread.storeImage=false;
  }

  pixmap=QPixmap::fromImage(i);
  update();
}

void videowidget::setFileName(QString f){
    thread.setFileName(f);
}

void videowidget::setDeviceName(QString f){
    thread.setDeviceName(f);
}
