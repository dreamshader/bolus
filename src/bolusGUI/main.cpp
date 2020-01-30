#include "mainwindow.h"
#include <QApplication>
#include <QTime>

#define IS_FRONTEND // set by the GUI source
#include "bolus.h"

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
