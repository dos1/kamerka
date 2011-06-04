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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#include <QtGui>
#include <QtDeclarative>
#include <QtScript/QScriptEngine>
#include "KMainWindow"

#include "videowidget.h"

class EngineAccess : public QObject
{
    Q_OBJECT
public:
    EngineAccess();
    ~EngineAccess();
    Q_INVOKABLE void setEngine(QScriptValue val);
    QScriptEngine *engine;
};


class MainWindow : public KMainWindow {
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow();
    videowidget *videoViewer;
    QDeclarativeView* ui;
private:
    void resizeEvent(QResizeEvent *e);
public slots:
    void photoTaken();
    void timerCounter(int);
    void showDirectory();
};

