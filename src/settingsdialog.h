#include <QtGui>
#include <KConfigDialog>

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

class SettingsDialog : public KConfigDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent, QString name, KConfigSkeleton *skeleton, QGraphicsScene *scene);
    QGraphicsProxyWidget *proxy;
signals:

public slots:

};

#endif // SETTINGSDIALOG_H
