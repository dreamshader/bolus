#include "dlghealthoptions.h"
#include "ui_dlghealthoptions.h"

dlgHealthOptions::dlgHealthOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgHealthOptions)
{
    FILE *sysFP;
    char dataBuff[80];

    pParent = parent;

    ui->setupUi(this);

    if((sysFP = popen("~/bolus/bolus-cli -q -f", "r")) != nullptr )
    {
        fgets(dataBuff, 79, sysFP);
        fclose(sysFP);

//           11111111112222
// 012345678901234567890123
// -010:-030:0025:0010:0000

        varSports1 = atoi(&dataBuff[0]);
        varSports2 = atoi(&dataBuff[5]);
        varStress = atoi(&dataBuff[10]);
        varIll = atoi(&dataBuff[15]);
        varFemale = atoi(&dataBuff[20]);

        ui->spnSport1Factor->setValue(varSports1);
        ui->spnSport2Factor->setValue(varSports2);
        ui->spnStressFactor->setValue(varStress);
        ui->spnIllnessFactor->setValue(varIll);
        ui->spnFemaleFactor->setValue(varFemale);
    }
}

dlgHealthOptions::~dlgHealthOptions()
{
    delete ui;
}
