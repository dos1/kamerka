#include <QtGui>
#include <QtDeclarative>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow w;

    qmlRegisterType<QGraphicsBlurEffect>("Effects",1,0,"Blur");
    qmlRegisterType<QGraphicsDropShadowEffect>("Effects",1,0,"DropShadow");

    QDeclarativeView* ui = new QDeclarativeView;
    w.setCentralWidget(ui);
    ui->setSource(QUrl("qrc:/helloworld.qml"));
    ui->setStyleSheet("background:transparent");
    ui->setResizeMode(QDeclarativeView::SizeRootObjectToView);

    w.show();
    return a.exec();
}
