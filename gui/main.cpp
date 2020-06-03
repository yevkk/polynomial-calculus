#include "mainwindow.h"
#include "../src/Polynomial.hpp"

#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    MainWindow w;

    QFontDatabase::addApplicationFont("src/LexendDeca-Regular.ttf");

    w.show();
    return a.exec();
}
