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

#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <KNotification>
#include <QPainter>
#include <QtQuickWidgets/QQuickWidget>
#include <phonon/MediaObject>

#include "capturethread.h"

class videowidget : public QWidget
{
    Q_OBJECT
public:
    videowidget(QWidget *parent = 0);
    ~videowidget();

    QPixmap pixmap;
    CaptureThread thread;
    QQuickWidget *ui;
    Phonon::MediaObject* media;
    void resize(const QSize& size);
    void takeImage();
protected:
    void paintEvent(QPaintEvent *event);

public slots:
    void setPicture(QImage);

private:
    bool storeImage;
    int imageDelay;
};

class Notification : public KNotification {
    Q_OBJECT
public:
    Notification(QString, QString filename);
    QString filename;
public slots:
    void openFile(unsigned int);
};

#endif // VIDEOWIDGET_H
