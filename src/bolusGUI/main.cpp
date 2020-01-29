#include "mainwindow.h"
#include <QApplication>
#include "bolus.h"
#include <QTime>

struct _bolus_param gParam;
int currSelTimeBlock;
int currCalcBolus;
int glucoMeasuredValue;
char glucoMeasureMethod;
QTime timeOfEntry;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
