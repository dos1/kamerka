#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QtDeclarative>

#include "capturethread.h"

class videowidget : public QWidget
{
Q_OBJECT
  public:
    videowidget(QWidget *parent = 0);

    QPixmap pixmap;
    CaptureThread thread;
    QDeclarativeView *ui;
    void setFileName(QString);
    void setDeviceName(QString);
  protected:
    void paintEvent(QPaintEvent *event);

  public slots:
    void setPicture(QImage);
};

class Focia : public QObject {
Q_OBJECT
  public:
    QString filename;
    void setFilename(QString);
  public slots:
    void openFile(unsigned int);
};

#endif // VIDEOWIDGET_H
