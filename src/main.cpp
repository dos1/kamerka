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

#include <KCoreAddons/KAboutData>
#include <KLocalizedString>

#include "mainwindow.h"

#define VERSION "0.20"

int main(int argc, char *argv[]) {
    printf("Kamerka version %s\n", VERSION);
    printf("    Copyright (C) 2011-2019 Sebastian Krzyszkowiak\n");
    printf("                            https://dosowisko.net/\n");
    printf("    Kamerka comes with ABSOLUTELY NO WARRANTY.\n");
    printf("    This is free software, and you are welcome to redistribute it\n");
    printf("    under certain conditions; type `./kamerka --license' for details.\n\n");
    fflush(stdout);

    QApplication a(argc, argv);
    a.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    KAboutData aboutData("kamerka", i18n("Kamerka"), VERSION,
                         i18n("Simple photo taking application with fancy animated interface"),
                         KAboutLicense::GPL, i18n("Copyright (C) 2011-2014 Sebastian Krzyszkowiak") );
    KAboutData::setApplicationData(aboutData);
    QIcon icon(":/icons/kamerka.png");
    a.setWindowIcon(icon);
    new MainWindow();
    return a.exec();
}
