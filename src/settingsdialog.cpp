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

#include <KConfigSkeleton>
#include <KMessageBox>
#include <KLocale>
#include <KFile>
#include "settingsdialog.h"

void SettingsDialog::updateUrl() {
	if (xdggroupbox->isChecked()) {
		QDir dir(KGlobalSettings::picturesPath());
		QString sub = "";
		if (subdircheck->isChecked()) {
			sub = subdir->text();
			subdir->setEnabled(true);
		} else subdir->setEnabled(false);
		config->findItem("photodir")->setProperty(dir.absoluteFilePath(sub));
		urledit->setText(dir.absoluteFilePath(sub));
		urledit->setEnabled(false);
	} else urledit->setEnabled(true);
}

void SettingsDialog::checkDir() {
	QDir dir;
	if (!(dir.exists(config->findItem("photodir")->property().toString()))) {
		int ret = KMessageBox::questionYesNo(0, i18n("Selected path does not exists. Do you want to create it?"));
		if (ret==KMessageBox::Yes) dir.mkpath(config->findItem("photodir")->property().toString());
	}
}

SettingsDialog::SettingsDialog(QWidget *parent, QString name, KConfigSkeleton *conf) :
	KConfigDialog(parent, name, conf)
{
	this->showButton(KDialog::Help, false);
	config=conf;

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

	QCheckBox *checkbox = new QCheckBox(i18n("Enhance contrast"));
	checkbox->setObjectName("kcfg_normalize");
	layout->addRow(checkbox);
	checkbox = new QCheckBox(i18n("Lock aspect ratio"));
	checkbox->setObjectName("kcfg_aspectlock");
	layout->addRow(checkbox);
	checkbox = new QCheckBox(i18n("Mirror output"));
	checkbox->setObjectName("kcfg_mirror");
	layout->addRow(checkbox);
	checkbox = new QCheckBox(i18n("Flip output"));
	checkbox->setObjectName("kcfg_flip");
	layout->addRow(checkbox);

	xdggroupbox = new QGroupBox();
	xdggroupbox->setTitle(i18n("Image Settings"));
	xdggroupbox->setCheckable(false);
	QFormLayout *lay = new QFormLayout(xdggroupbox);
	QSlider *slider = new QSlider();
	slider->setObjectName("kcfg_brightness");
	slider->setMinimum(-100);
	slider->setMaximum(100);
	slider->setTickInterval(10);
	slider->setTickPosition(QSlider::TicksBelow);
	slider->setOrientation(Qt::Horizontal);
	lay->addRow(i18n("Brightness:"), slider);

	slider = new QSlider();
	slider->setObjectName("kcfg_contrast");
	slider->setMinimum(0);
	slider->setMaximum(100);
	slider->setTickInterval(10);
	slider->setTickPosition(QSlider::TicksBelow);
	slider->setOrientation(Qt::Horizontal);
	lay->addRow(i18n("Contrast:"), slider);

	slider = new QSlider();
	slider->setObjectName("kcfg_saturation");
	slider->setMinimum(0);
	slider->setMaximum(100);
	slider->setTickInterval(10);
	slider->setTickPosition(QSlider::TicksBelow);
	slider->setOrientation(Qt::Horizontal);
	lay->addRow(i18n("Saturation:"), slider);

	slider = new QSlider();
	slider->setObjectName("kcfg_hue");
	slider->setMinimum(-100);
	slider->setMaximum(100);
	slider->setTickInterval(10);
	slider->setTickPosition(QSlider::TicksBelow);
	slider->setOrientation(Qt::Horizontal);
	lay->addRow(i18n("Hue:"), slider);

	layout->addRow(xdggroupbox);

	this->addPage(page, i18n("Camera"), "camera-web", i18n("Camera settings") );

	// Capture page
	page = new QWidget(this);
	layout = new QFormLayout(page);
	xdggroupbox = new QGroupBox();
	xdggroupbox->setTitle(i18n("Self-timer"));
	xdggroupbox->setCheckable(false);
	lay = new QFormLayout(xdggroupbox);
	spinbox = new QSpinBox();
	spinbox->setObjectName("kcfg_selftimer");
	spinbox->setSuffix(i18n(" seconds"));
	spinbox->setMinimum(1);
	spinbox->setMaximum(360);
	lay->addRow(i18n("Self-timer timeout:"), spinbox);
	layout->addRow(xdggroupbox);

	xdggroupbox = new QGroupBox();
	xdggroupbox->setTitle(i18n("Burst mode"));
	xdggroupbox->setCheckable(false);
	lay = new QFormLayout(xdggroupbox);
	spinbox = new QSpinBox();
	spinbox->setObjectName("kcfg_burstnumphotos");
	spinbox->setMinimum(2);
	spinbox->setMaximum(20);
	lay->addRow(i18n("Number of photos:"), spinbox);

	spinbox = new QSpinBox();
	spinbox->setObjectName("kcfg_delaybetweenphotos");
	spinbox->setSuffix(i18n(" seconds"));
	spinbox->setMinimum(1);
	spinbox->setMaximum(3600);
	lay->addRow(i18n("Delay between photos:"), spinbox);

	layout->addRow(xdggroupbox);

	this->addPage(page, i18n("Capture"), "camera-photo", i18n("Capture") );


	// storage page
	page = new QWidget(this);
	layout = new QFormLayout(page);

	xdggroupbox = new QGroupBox();
	xdggroupbox->setTitle(i18n("Use default pictures directory"));
	xdggroupbox->setCheckable(true);
	xdggroupbox->setObjectName("kcfg_usexdgpictures");
	connect(xdggroupbox, SIGNAL(toggled(bool)), this, SLOT(updateUrl()));
	lay = new QFormLayout(xdggroupbox);
	xdggroupbox->setLayout(lay);

	subdircheck = new QCheckBox(i18n("Use subdirectory:"));
	subdircheck->setObjectName("kcfg_ifsubdirectory");
	subdir = new KLineEdit();
	connect(subdir, SIGNAL(textChanged(QString)), this, SLOT(updateUrl()));
	subdir->setObjectName("kcfg_subdirectory");
	connect(subdircheck, SIGNAL(stateChanged(int)), this, SLOT(updateUrl()));
	lay->addRow(subdircheck, subdir);

	layout->addRow(xdggroupbox);
	urledit = new KUrlRequester();
	urledit->setMode(KFile::Directory);
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

	updateUrl();
	config->writeConfig();
	connect(this, SIGNAL(settingsChanged(const QString&)), this, SLOT(updateUrl()));
	connect(this, SIGNAL(settingsChanged(const QString&)), this, SLOT(checkDir()));
}
