#include "dlgrecommendationoptions.h"
#include "ui_dlgrecommendationoptions.h"

dlgRecommendationOptions::dlgRecommendationOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgRecommendationOptions)
{
    FILE *sysFP;
    char dataBuff[80];
    QTime tmAct, tmDelay;

    pParent = parent;

    ui->setupUi(this);


    if((sysFP = popen("~/bolus/bolus-cli -q -G", "r")) != nullptr )
    {
        fgets(dataBuff, 79, sysFP);
        fclose(sysFP);

//           111111111122222222223333333333444
// 0123456789012345678901234567890123456789012
// 00000000:0005:0050:0010:0180:0090:1380:0030

        varVersion = atoi(&dataBuff[0]);
        varTimeBlocksActive = atoi(&dataBuff[9]);
        varIncreaseLevel = atoi(&dataBuff[14]);
        varSnacksize10BE = atoi(&dataBuff[19]) / 10;
        varActTime = atoi(&dataBuff[24]);
        varDelayTime = atoi(&dataBuff[29]);
        varBasalActTime = atoi(&dataBuff[34]);
        varBasalDelayTime = atoi(&dataBuff[39]);

        ui->spnGlucoRaise->setValue(varIncreaseLevel);
        ui->spnSnackSize->setValue(varSnacksize10BE);
        tmAct.setHMS(varActTime / 60,varActTime % 60,0);
        tmDelay.setHMS(varDelayTime / 60,varDelayTime % 60,0);

        ui->tmDuration->setTime(tmAct);
        ui->tmDelay->setTime(tmDelay);
    }
}

dlgRecommendationOptions::~dlgRecommendationOptions()
{
    delete ui;
}

void dlgRecommendationOptions::on_dlgRecommendationOptions_finished(int result)
{
    if( result == QDialog::Accepted )
    {
        varIncreaseLevel = ui->spnGlucoRaise->value();
        varSnacksize10BE = ui->spnSnackSize->value() * 10;

        varActTime = ui->tmDuration->time().hour() * 60 + ui->tmDuration->time().minute();
        varDelayTime = ui->tmDelay->time().hour() * 60 + ui->tmDelay->time().minute();
    }
}
