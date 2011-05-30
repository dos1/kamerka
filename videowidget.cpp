#include "videowidget.h"
#include <KNotification>
#include <KLocale>
#include <pwd.h>

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

    passwd* user = getpwuid(getuid());

    int c = 0;
    QString counterfilename;
    counterfilename = user->pw_dir;
    counterfilename += "/kamerka/.counter";

    QFile counterfile(counterfilename.toStdString().c_str());
    if (counterfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
          QTextStream counter(&counterfile);
          counter >> c;
    }
    else qDebug("Could not open .counter file!");
    c++;
    counterfile.close();
    //unlink(qfile.toStdString().c_str());

    counterfile.open(QIODevice::WriteOnly);
    QTextStream counter(&counterfile);
    counter << c;
    counterfile.close();

    QString imagepath;
    imagepath = user->pw_dir;
    imagepath += "/kamerka/image";
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
