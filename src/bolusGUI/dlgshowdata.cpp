#include "dlgshowdata.h"
#include "ui_dlgshowdata.h"

dlgShowData::dlgShowData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgShowData)
{
    pParent = parent;
    ui->setupUi(this);

    ui->lblTextEOF->setText("");
    currRecno = 1;
    eofReached = false;
    displayDataRecord();
}


void dlgShowData::displayDataRecord()

{
    extern QString fileName;

    FILE *sysFP;
    char cmdBuff[80];
    char dataBuff[80];

    sprintf(cmdBuff, "~/bolus/bolus-cli -D -v2 -R%s -r%d", fileName.toStdString().c_str(), currRecno);
    if((sysFP = popen(cmdBuff, "r")) != nullptr )
    {
        if( fgets(dataBuff, 79, sysFP) )
        {
            ui->lblGlucoValue->setNum(atoi(&dataBuff[0]));

            switch(dataBuff[5] )
            {
                case 'b':
                case 'B':
                    ui->lblMealTimeValue->setText("Vor Mahlzeit");
                    break;
                case 'a':
                case 'A':
                    ui->lblMealTimeValue->setText("Nach Mahlzeit");
                    break;
                case 'n':
                case 'N':
                    ui->lblMealTimeValue->setText("Kein Eintrag");
                    break;
                case 's':
                case 'S':
                    ui->lblMealTimeValue->setText("SchlafenszeitEintrag");
                    break;
                case 'x':
                case 'X':
                    ui->lblMealTimeValue->setText("Anderes");
                    break;
                default:
                    ui->lblMealTimeValue->setText("Kein Eintrag");
                    break;
            }

            ui->lblCarbValue->setNum(atoi(&dataBuff[7]));
            ui->lblHealthnessValue->setNum(atoi(&dataBuff[12]));
            ui->lblBolusValue->setNum(atoi(&dataBuff[17]));
            ui->lblBasalValue->setNum(atoi(&dataBuff[22]));
        }
        else
        {
            eofReached = true;
        }
        fclose(sysFP);
    }
}






dlgShowData::~dlgShowData()
{
    delete ui;
}

void dlgShowData::on_btnPrevRec_clicked()
{
    if( currRecno > 1 )
    {
        ui->lblTextEOF->setText("");
        currRecno--;
        displayDataRecord();
    }
    else
    {
        ui->lblTextEOF->setText("Datei-Anfang erreicht!");
    }
}

void dlgShowData::on_btnNextRec_clicked()
{
    if( !eofReached )
    {
        ui->lblTextEOF->setText("");
        currRecno++;
        displayDataRecord();
    }
    else
    {
        ui->lblTextEOF->setText("Datei-Ende erreicht!");
    }
}
