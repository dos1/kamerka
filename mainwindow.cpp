/*
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

#include "mainwindow.h"

void MainWindow::photoTaken(){
    videoViewer->ui = ui;
    videoViewer->thread.storeImage=true;
}

void MainWindow::timerCounter(int count) {
    //qDebug(QString::number(count).toStdString().c_str());
    if (count>0) {
      QProcess::startDetached("beep", QStringList() << "-l" << "250");
    }
    else if (count==0) {
      QProcess::startDetached("beep", QStringList() << "-l" << "1000" << "-f" << "1000");
    }
}

void MainWindow::showDirectory() {
    QProcess::startDetached("kde-open", QStringList() << QDir::homePath() + "/kamerka");
    delete videoViewer->media;
    exit(0);
}

void MainWindow::resizeEvent(QResizeEvent *e) {
    videoViewer->resize(this->size());
    QMainWindow::resizeEvent(e);
}

MainWindow::~MainWindow()
{
    delete videoViewer->media;
    exit(0);
}

MainWindow::MainWindow() {
{
    qmlRegisterType<QGraphicsBlurEffect>("Effects",1,0,"Blur");
    qmlRegisterType<QGraphicsDropShadowEffect>("Effects",1,0,"DropShadow");

    ui = new QDeclarativeView;
    videoViewer = new videowidget(this);

    QIcon icon(":/kamerka.png");

    videoViewer->show();
    this->setCentralWidget(ui);
    this->setWindowIcon(icon);

    if (videoViewer->thread.startUlan()) {
      QMessageBox msgbox( QMessageBox::Critical, "Error", "Could not... blablabla, nie dziaua :(");
      msgbox.exec();
      delete videoViewer->media;
      exit(0);
    }

    ui->setSource(QUrl("qrc:/kamerka.qml"));
    ui->rootContext()->setContextProperty("fileName", "");
    ui->setStyleSheet("background:transparent");
    videoViewer->setStyleSheet("background:transparent");

    ui->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    this->setWindowTitle(QString("Kamerka"));

    connect(ui->rootObject(), SIGNAL(photoTaken()), this, SLOT(photoTaken()));
    connect(ui->rootObject(), SIGNAL(timerCounter(int)), this, SLOT(timerCounter(int)));
    connect(ui->rootObject(), SIGNAL(showDirectory()), this, SLOT(showDirectory()));

  }

}
