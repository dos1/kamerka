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

#include <KApplication>
#include <KLocale>
#include <KCmdLineArgs>
#include <KAboutData>

#include "mainwindow.h"

#define VERSION "0.7"

int main(int argc, char *argv[])
{
    printf("Kamerka version %s\n    Copyright (C) 2011 Sebastian Krzyszkowiak\n", VERSION);
    printf("    Kamerka comes with ABSOLUTELY NO WARRANTY.\n");
    printf("    This is free software, and you are welcome to redistribute it\n");
    printf("    under certain conditions; type `./kamerka --license' for details.\n");

    QApplication::setGraphicsSystem("raster"); // improves performance a lot
    KAboutData aboutData("kamerka", 0, ki18n("Kamerka"), VERSION,
                         ki18n("Simple photo taking application with fancy animated interface"),
                         KAboutData::License_GPL, ki18n("Copyright (c) 2011 Sebastian Krzyszkowiak") );
    KCmdLineArgs::init(argc, argv, &aboutData);
    KApplication a;
    QIcon icon(":/icons/kamerka.png");
    a.setWindowIcon(icon);
    MainWindow *w = new MainWindow();
    w->show();
    return a.exec();
}
