#include <KConfigSkeleton>
#include <KLineEdit>
#include <KUrlRequester>
#include <KLocale>

#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent, QString name, KConfigSkeleton *config) :
    KConfigDialog(parent, name, config)
{
    this->showButton(KDialog::Help, false);

    // camera page
    QWidget *page = new QWidget(this);
    QFormLayout *layout = new QFormLayout(page);

    KLineEdit *textedit = new KLineEdit();
    textedit->setObjectName("kcfg_node");
    layout->addRow(i18n("Device node:"), textedit);

    QHBoxLayout *hlayout = new QHBoxLayout();
    QSpinBox *spinbox = new QSpinBox();
    spinbox->setMaximum(10000);
    spinbox->setObjectName("kcfg_width");
    hlayout->addWidget(spinbox);
    hlayout->addWidget(new QLabel(i18n("x")));
    spinbox = new QSpinBox();
    spinbox->setMaximum(10000);
    spinbox->setObjectName("kcfg_height");
    hlayout->addWidget(spinbox);
    hlayout->addWidget(new QLabel(i18n("px")));
    layout->addRow(i18n("Resolution:"), hlayout);

    spinbox = new QSpinBox();
    spinbox->setMinimum(0);
    spinbox->setMaximum(1000);
    spinbox->setSpecialValueText(i18n("Disabled"));
    spinbox->setSuffix(i18n(" fps"));
    spinbox->setObjectName("kcfg_fps");
    layout->addRow(i18n("Framerate limit:"), spinbox);

    this->addPage(page, i18n("Camera"), "camera-web", i18n("Camera settings") );

    // storage page
    page = new QWidget(this);
    layout = new QFormLayout(page);

    QGroupBox *groupbox = new QGroupBox();
    groupbox->setTitle(i18n("Use default pictures directory"));
    groupbox->setCheckable(true);
    groupbox->setObjectName("kcfg_usexdgpictures");
    QFormLayout *lay = new QFormLayout(groupbox);
    groupbox->setLayout(lay);

    QCheckBox *checkbox = new QCheckBox(i18n("Use subdirectory:"));
    checkbox->setObjectName("kcfg_ifsubdirectory");
    textedit = new KLineEdit();
    textedit->setObjectName("kcfg_subdirectory");
    lay->addRow(checkbox, textedit);

    layout->addRow(groupbox);
    KUrlRequester *urledit = new KUrlRequester();
    urledit->setObjectName("kcfg_photodir");
    urledit->setProperty("kcfg_property", QByteArray("text"));
    layout->addRow(i18n("Photo directory:"), urledit);

    this->addPage(page, i18n("Storage"), "drive-harddisk", i18n("Photo storage") );

    // behaviour page
    page = new QWidget(this);
    layout = new QFormLayout(page);

    checkbox = new QCheckBox(i18n("Play sound on taking photo"));
    checkbox->setObjectName("kcfg_soundontaking");
    layout->addRow(checkbox);
    checkbox = new QCheckBox(i18n("Play timer sounds"));
    checkbox->setObjectName("kcfg_soundontimer");
    layout->addRow(checkbox);
    checkbox = new QCheckBox(i18n("Show notification on taking photo"));
    checkbox->setObjectName("kcfg_notification");
    layout->addRow(checkbox);

    this->addPage(page, i18n("Behaviour"), "audio-headset", i18n("Behaviour") );
}
