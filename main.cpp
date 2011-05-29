#include <KApplication>
#include <KLocale>
#include <KCmdLineArgs>
#include <KAboutData>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    system("mkdir -p ~/kamerka");
    KAboutData aboutData("kamerka", 0, ki18n("Kamerka"), "0.5",
    ki18n("Prosta aplikacja dla Agaty"), KAboutData::License_GPL, ki18n("Copyright (c) 2011 Sebastian Krzyszkowiak") );
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
