#include <KConfigSkeleton>
#include <KLineEdit>
#include <KUrlRequester>
#include <KLocale>

#include "KamerkaSettings.h"
#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent, QString name, KConfigSkeleton *skeleton, QGraphicsScene *scene) :
    KConfigDialog(parent, name, skeleton)
{
    this->showButton(KDialog::Help, false);
    proxy = scene->addWidget(this);
    proxy->hide();

    // drop shadow
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setOffset(QPointF(0, 0));
    shadow->setBlurRadius(8);
    shadow->setColor(QColor(255,255,255));
    proxy->setGraphicsEffect(shadow);

    // camera page
    QWidget *page = new QWidget(this);
    QFormLayout *layout = new QFormLayout(page);
    this->addPage(page, i18n("Camera"), "camera-web", i18n("Camera settings") );

    KLineEdit *textedit = new KLineEdit();
    textedit->setObjectName("kcfg_node");
    textedit->setAccessibleName("kcfg_node");
    layout->addRow(i18n("Device node:"), textedit);

    QHBoxLayout *hlayout = new QHBoxLayout();
    QSpinBox *spinbox = new QSpinBox();
    spinbox->setMaximum(10000);
    hlayout->addWidget(spinbox);
    hlayout->addWidget(new QLabel(i18n("x")));
    spinbox = new QSpinBox();
    spinbox->setMaximum(10000);
    hlayout->addWidget(spinbox);
    hlayout->addWidget(new QLabel(i18n("px")));
    layout->addRow(i18n("Resolution:"), hlayout);

    spinbox = new QSpinBox();
    spinbox->setMinimum(0);
    spinbox->setMaximum(1000);
    spinbox->setSpecialValueText(i18n("Disabled"));
    spinbox->setSuffix(i18n(" fps"));
    layout->addRow(i18n("Framerate limit:"), spinbox);

    // storage page
    page = new QWidget(this);
    layout = new QFormLayout(page);
    this->addPage(page, i18n("Storage"), "drive-harddisk", i18n("Photo storage") );

    QGroupBox *groupbox = new QGroupBox();
    groupbox->setTitle(i18n("Use default pictures directory"));
    groupbox->setCheckable(true);
    QFormLayout *lay = new QFormLayout(groupbox);
    groupbox->setLayout(lay);

    QCheckBox *checkbox = new QCheckBox(i18n("Use subdirectory:"));
    textedit = new KLineEdit();
    lay->addRow(checkbox, textedit);

    layout->addRow(groupbox);
    KUrlRequester *urledit = new KUrlRequester();
    layout->addRow(i18n("Photo directory:"), urledit);

    // behaviour page
    page = new QWidget(this);
    layout = new QFormLayout(page);
    this->addPage(page, i18n("Behaviour"), "audio-headset", i18n("Behaviour") );

    checkbox = new QCheckBox(i18n("Play sound on taking photo"));
    layout->addRow(checkbox);
    checkbox = new QCheckBox(i18n("Play timer sounds"));
    layout->addRow(checkbox);
    checkbox = new QCheckBox(i18n("Show notification on taking photo"));
    layout->addRow(checkbox);

}
