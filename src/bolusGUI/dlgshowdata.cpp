#include "dlgshowdata.h"
#include "ui_dlgshowdata.h"
#include <time.h>

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
            char timeBuffer[20];
            char dateBuffer[20];

            eofReached = false;

//            1549916355:0234:n:0000:0000:0001:0000
            time_t timestamp = static_cast <time_t> (atol(dataBuff));
            struct tm *pDataTime = localtime(&timestamp);

            // int tm_sec;         /* seconds */
            // int tm_min;         /* minutes */
            // int tm_hour;        /* hours */
            sprintf(timeBuffer, "%02d:%02d.%02d",pDataTime->tm_hour, pDataTime->tm_min, pDataTime->tm_sec );
            sprintf(dateBuffer, "%02d.%02d. %d", pDataTime->tm_mday, pDataTime->tm_mon+1, pDataTime->tm_year+1900);

            ui->lblTime->setText(timeBuffer);
            ui->lblDate->setText(dateBuffer);
            // int tm_mday;        /* day of the month */
            // int tm_mon;         /* month */
            // int tm_year;        /* year */

            ui->lblGlucoValue->setNum(atoi(&dataBuff[11]));

            switch(dataBuff[16] )
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

            ui->lblCarbValue->setNum(atoi(&dataBuff[18]));
            ui->lblHealthnessValue->setNum(atoi(&dataBuff[23]));
            ui->lblBolusValue->setNum(atoi(&dataBuff[28]));
            ui->lblBasalValue->setNum(atoi(&dataBuff[33]));
        }
        else
        {
            eofReached = true;
            if( currRecno > 1 )
            {
                ui->lblTextEOF->setText("Datei-Ende erreicht!");
                currRecno--;
            }
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
}
