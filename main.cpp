#include "WeatherReport.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WeatherReport w;
    w.show();
    return a.exec();
}
