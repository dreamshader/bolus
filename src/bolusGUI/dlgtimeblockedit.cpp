#include "dlgtimeblockedit.h"
#include "ui_dlgtimeblockedit.h"

dlgTimeBlockEdit::dlgTimeBlockEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgTimeBlockEdit)
{
    extern int currSelTimeBlock;

    FILE *sysFP;
    char cmdBuff[40];
    char dataBuff[80];
    char tmLblFrom[6];
    char tmLblTo[6];
    int tgtGlucoFrom;
    int tgtGlucoTo;
    double unitsPerBE;
    int sensitivity;

    pParent = parent;

    ui->setupUi(this);

    sprintf(cmdBuff, "~/bolus/bolus-cli -q -T%d", currSelTimeBlock);
    if((sysFP = popen(cmdBuff, "r")) != nullptr )
    {
        fgets(dataBuff, 79, sysFP);
        fclose(sysFP);
        sprintf(tmLblFrom, "%c%c%c%c%c",
                dataBuff[0],dataBuff[1],dataBuff[2],dataBuff[3],dataBuff[4]);
        sprintf(tmLblTo, "%c%c%c%c%c",
                dataBuff[6],dataBuff[7],dataBuff[8],dataBuff[9],dataBuff[10] );


        //                  1111111111222222222
        //        01234567890123456789012345678
        //        11:00:16:00:0080:0160:0013:70

        tgtGlucoFrom = atoi(&dataBuff[12]);
        tgtGlucoTo = atoi(&dataBuff[17]);
        unitsPerBE = atof(&dataBuff[22]) / 10.0;
        sensitivity = atoi(&dataBuff[27]);


        ui->lblTmBlkFrom->setText(tmLblFrom);
        ui->lblTmBlkTo->setText(tmLblTo);

        ui->spnTgtFrom->setValue(tgtGlucoFrom);
        ui->spnTgtTo->setValue(tgtGlucoTo);

        ui->spnInsulinUnits->setValue(unitsPerBE);
        ui->spnBreadUnits->setValue(1.0);

        ui->spnInsulinUnits_2->setValue(1.0);
        ui->spnGlucoValue->setValue(sensitivity);

    }

}

dlgTimeBlockEdit::~dlgTimeBlockEdit()
{
    delete ui;
}
