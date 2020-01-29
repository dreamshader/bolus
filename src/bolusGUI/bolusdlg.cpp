#include "bolusdlg.h"
#include "ui_bolusdlg.h"
#include "bolus.h"
#include <QTime>

bolusDlg::bolusDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::bolusDlg)
{
    extern struct _bolus_param gParam;
    extern QTime timeOfEntry;


    char cmdLine[40];
    pParent = parent;
    pDlgConfirmBolus = nullptr;
    char glucoValue[10];
    char timeLabel[10];


    ui->setupUi(this);

    ui->mealTimeSelect->addItem("Kein Eintrag");
    ui->mealTimeSelect->addItem("Vor Mahlzeit");
    ui->mealTimeSelect->addItem("Nach Mahlzeit");
    ui->mealTimeSelect->addItem("Schlafenszeit");
    ui->mealTimeSelect->addItem("Andere");

    ui->healthSelect->addItem("Kein Eintrag");
    ui->healthSelect->addItem("Nüchtern");
    ui->healthSelect->addItem("Sport 1");
    ui->healthSelect->addItem("Stress");
    ui->healthSelect->addItem("Krankheit");
    ui->healthSelect->addItem("Sport 2");
    ui->healthSelect->addItem("Menstruation");
    ui->healthSelect->addItem("Andere");



//    gParam.glucose
//    gParam.measType = 'f';

    sprintf(cmdLine, "~/bolus/bolus-cli -q -g%d", gParam.glucose);

    int retVal = system(cmdLine);
    if( retVal > 128 )
        retVal = retVal / 256;
    switch( retVal )
    {
        case 0:
            ui->glucoseStatus->setStyleSheet("background: green");
            break;
        case 1:
            ui->glucoseStatus->setStyleSheet("background: yellow");
            break;
        case 2:
            ui->glucoseStatus->setStyleSheet("background: red");
            break;
    }
    sprintf(glucoValue, "%d", gParam.glucose );
    ui->lblGlucoseValue->setText(glucoValue);
    timeOfEntry = QTime::currentTime();
    sprintf(timeLabel, "%d:%d", timeOfEntry.hour(), timeOfEntry.minute() );
    ui->lblCurrTime->setText(timeLabel);

}

bolusDlg::~bolusDlg()
{
    delete ui;
}

void bolusDlg::on_btnBolusCalc_clicked()
{
    extern int currCalcBolus;
    extern struct _bolus_param gParam;

    int bolus;
    char cmdBuffer[40];

    // Daten übernehmen
    int x = ui->mealTimeSelect->currentIndex();
    int y = ui->healthSelect->currentIndex();
    double broteinheiten = ui->breadUnits->value();

    sprintf(cmdBuffer,"~/bolus/bolus-cli -g%d -c%d -n", gParam.glucose, (int) (broteinheiten*12) );
    bolus = system(cmdBuffer);

    if( bolus > 0 )
    {
        currCalcBolus = bolus / 256;

        if( currCalcBolus < 128 )
        {
            if( pDlgConfirmBolus == nullptr )
            {
                pDlgConfirmBolus = new dlgConfirmBolus(pParent);
                pDlgConfirmBolus->show();
                if( pDlgConfirmBolus->exec() == QDialog::Accepted )
                {
                    // save settings
                }

                pDlgConfirmBolus = nullptr;
            }
        }
    }
}

void bolusDlg::on_btnMainMenu_clicked()
{
    // cancel
   // int x = ui->mealTimeSelect->currentIndex();
}
