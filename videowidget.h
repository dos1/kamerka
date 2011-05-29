#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QQueue>
#include <QMutex>
#include <QPixmap>
#include <QtDeclarative>

#include "capturethread.h"

class videowidget : public QWidget
{
Q_OBJECT
public:
    videowidget(QWidget *parent = 0);
    ~videowidget();

    QPixmap pixmap;
    CaptureThread thread;
    QDeclarativeView *ui;
    void setFileName(QString);
    void setDeviceName(QString);
protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:
    void setPicture(QImage);
private:


};

class Focia : public QObject {
Q_OBJECT
public:
  QString filename;
  void setFilename(QString);

signals:

public slots:
  void openFile(unsigned int);
private:

};

#endif // VIDEOWIDGET_H
