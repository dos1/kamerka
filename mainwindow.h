#include <QtGui>
#include <QtDeclarative>
#include "videowidget.h"

class MainWindow : public QMainWindow {
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

