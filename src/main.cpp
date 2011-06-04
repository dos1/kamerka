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

#include <KApplication>
#include <KLocale>
#include <KCmdLineArgs>
#include <KAboutData>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication::setGraphicsSystem("raster");
    KAboutData aboutData("kamerka", 0, ki18n("Kamerka"), "0.5",
    ki18n("Simple photo taking application with fancy animated interface"), KAboutData::License_GPL, ki18n("Copyright (c) 2011 Sebastian Krzyszkowiak") );
    KCmdLineArgs::init(argc, argv, &aboutData);
    printf("Kamerka version 0.5\n    Copyright (C) 2011 Sebastian Krzyszkowiak\n");
    printf("    Kamerka comes with ABSOLUTELY NO WARRANTY.\n");
    printf("    This is free software, and you are welcome to redistribute it\n");
    printf("    under certain conditions; type `./kamerka --license' for details.\n");
    KApplication a;
    MainWindow w;

    w.show();

    return a.exec();
}
