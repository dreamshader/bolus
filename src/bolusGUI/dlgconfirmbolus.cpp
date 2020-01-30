#include "dlgconfirmbolus.h"
#include "ui_dlgconfirmbolus.h"

#define IS_FRONTEND
#include "bolus.h"

dlgConfirmBolus::dlgConfirmBolus(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgConfirmBolus)
{
    extern int currCalcBolus;
    pParent = parent;

    ui->setupUi(this);
    ui->spnBolusValue->setValue(currCalcBolus);
}

dlgConfirmBolus::~dlgConfirmBolus()
{
    delete ui;
}

void dlgConfirmBolus::on_dlgConfirmBolus_finished(int result)
{
//    sprintf(cmdBuffer,"~/bolus/bolus-cli -g%d -c%d -n", gParam.glucose, (int) (gParam.bread*12) );
//    sprintf(cmdLine, "~/bolus/bolus-cli -q -t%c -g%d", gParam.measType, gParam.glucose);
//    gParam.adjustType = 'o';
//    gParam.mealType = 'a';
//    gParam.noStore = ui->chkNoStore->isChecked();

    extern struct _bolus_param gParam;

    if( result == QDialog::Accepted )
    {
        char cmdBuffer[80];
        int retVal;

        if( gParam.noStore )
        {
            sprintf(cmdBuffer, "~/bolus/bolus-cli -g%d -c%d -a%c -m%c -n",
                    gParam.glucose, static_cast <int> (gParam.bread*12),
                    gParam.adjustType, gParam.mealType );
        }
        else
        {
            sprintf(cmdBuffer, "~/bolus/bolus-cli -g%d -c%d -a%c -m%c -n",
                    gParam.glucose,  static_cast <int> (gParam.bread*12),
                    gParam.adjustType, gParam.mealType );
        }
        retVal = system(cmdBuffer);
    }
}
