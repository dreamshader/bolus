#include "mainwindow.h"
#include "dlgnoenv.h"
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
char *pBolusEnv;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    if( (pBolusEnv = getenv("BOLUS")) == NULL )
    {
        dlgNoEnv *pErrorMessage = new dlgNoEnv();
        pErrorMessage->exec();
//        w.close();
//        a.exit(1);
    }

    return a.exec();
}
