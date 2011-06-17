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

#include <QtGui>
#include <QtDeclarative>
#include <QtScript/QScriptEngine>
#include <KMainWindow>

#include "settingsdialog.h"
#include "videowidget.h"

class EngineAccess : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE void setEngine(QScriptValue val);
    QScriptEngine *engine;
};


class MainWindow : public KMainWindow {
    Q_OBJECT
public:
    MainWindow();
    videowidget *videoViewer;
    QDeclarativeView *ui;
    QGraphicsProxyWidget *conf;
    SettingsDialog *confdial;
private:
    void resizeEvent(QResizeEvent *e);
public slots:
    void takePhoto();
    void timerCounter(int);
    void showDirectory();
    void showConfiguration();
    void QMLStatus(QDeclarativeView::Status);
    int opacityUpdate(QGraphicsProxyWidget*);
    void opacityUpdateConf();
    void closeCanvasLayer();
    void tryVideoThread();
};

