#include "videowidget.h"
#include <fstream>
#include <pwd.h>
#include <KNotification>
#include <KLocale>

videowidget::videowidget(QWidget *parent) :
    QWidget(parent)
{
    connect(&thread, SIGNAL(renderedImage(QImage)),
                this, SLOT(setPicture(QImage)));

    setAutoFillBackground(true);
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

  QString cmd;
  if (i==1) {
    cmd = "dolphin --select " + this->filename + " &";
  }
  else if (i==2) {
    cmd = "gimp " + this->filename + " &";
  }
  else {
      cmd = "inkscape " + this->filename + " &";
  }
  system(cmd.toStdString().c_str());
}


void videowidget::setPicture(QImage i){
  if (thread.storeImage) { 
    system("aplay -q /usr/share/kde4/apps/kamerka/kamerka.wav &");
    passwd* lol = getpwuid(getuid());

    int c = 0;
    QString qfile;
    qfile = lol->pw_dir;
    qfile += "/kamerka/.counter";
    //const char* cfile = qfile.toStdString().c_str();
    //qDebug(qfile.toStdString().c_str());

    std::ifstream counter(qfile.toStdString().c_str());
    if (counter) {
      counter >> c;
    }
    else qDebug("fuuuuuuuuu");
    c++;
    counter.close();
    unlink(qfile.toStdString().c_str());
    std::ofstream count(qfile.toStdString().c_str());
    count << c;
    count.close();
    QString s;
    s = lol->pw_dir;
    s += "/kamerka/image";
    s += QString::number(c);
    s += ".png";
    qDebug(s.toStdString().c_str());
    i.save(s, "PNG");

    ui->rootContext()->setContextProperty("fileName", "file:"+s);
    QMetaObject::invokeMethod(ui->rootObject(), "fotkaZrobiona");

    Focia* fotka = new Focia;
    fotka->setFilename(s);

    s = tr2i18n("Zdj\304\231cie zosta\305\202o zapisane do pliku ", 0) + s;

    QPixmap pixmap = QPixmap::fromImage(i);

    KNotification *notification= new KNotification ( "takenPhoto" );
    notification->setText( s );
    notification->setPixmap( pixmap );
    QStringList lista;
    lista << tr2i18n("Poka\305\274 w folderze", 0) << tr2i18n("Otw\303\263rz w GIMPie", 0) << tr2i18n("Otw\303\263rz w Inkscape", 0);
    notification->setActions( lista );
    connect(notification, SIGNAL(activated(unsigned int )), fotka , SLOT(openFile(unsigned int )) );
    notification->sendEvent();
    //delete notification;
    thread.storeImage=false;
  }

    pixmap=QPixmap::fromImage(i);
    update();
    //qApp->processEvents();
}

void videowidget::setFileName(QString f){
    thread.setFileName(f);
}

void videowidget::setDeviceName(QString f){
    thread.setDeviceName(f);
}
