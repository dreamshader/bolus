#include "dlgtimeblockperiods.h"
#include "ui_dlgtimeblockperiods.h"
#include <QFile>
#include <QDir>
#include <QDebug>

dlgTimeBlockPeriods::dlgTimeBlockPeriods(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgTimeBlockPeriods)
{
    pParent = parent;

    ui->setupUi(this);


    int loopCount;
    int retVal;
    FILE *sysFP;
    char cmdBuff[40];
    char dataBuff[80];
    char newItem[40];
    int fromHour, fromMinute;
    int toHour, toMinute;

    QTime valueFrom, valueTo;

    pParent = parent;

    ui->setupUi(this);

//    ui->lstTimeBlocks->addItem("00:00 - 06:00");
//    ui->lstTimeBlocks->addItem("06:00 - 12:00");
//    ui->lstTimeBlocks->addItem("12:00 - 16:00");
//    ui->lstTimeBlocks->addItem("16:00 - 22:00");
//    ui->lstTimeBlocks->addItem("22:00 - 00:00");

    retVal = system("~/bolus/bolus-cli -qT -T#");

    if( retVal > 128 )
    {
        retVal = retVal >> 8;
    }

    if( retVal > 128 )
    {
        this->close();
    }


    for( loopCount = 0; loopCount < retVal; loopCount++ )
    {
        sprintf(cmdBuff, "~/bolus/bolus-cli -qT -T%d", loopCount);
        if((sysFP = popen(cmdBuff, "r")) != nullptr )
        {
            fgets(dataBuff, 79, sysFP);
            fclose(sysFP);
            sscanf(dataBuff, "%2d:%2d:%2d:%2d", &fromHour, &fromMinute, &toHour, &toMinute);

            valueFrom.setHMS(fromHour, fromMinute, 0, 0);
            valueTo.setHMS(toHour, toMinute, 0, 0);

            switch(loopCount)
            {
                case 0:
                    ui->edTmFrom_0->setTime(valueFrom);
                    ui->edTmTo_0->setTime(valueTo);
                    break;
                case 1:
                    ui->edTmFrom_1->setTime(valueFrom);
                    ui->edTmTo_1->setTime(valueTo);
                    break;
                case 2:
                    ui->edTmFrom_2->setTime(valueFrom);
                    ui->edTmTo_2->setTime(valueTo);
                    break;
                case 3:
                    ui->edTmFrom_3->setTime(valueFrom);
                    ui->edTmTo_3->setTime(valueTo);
                    break;
                case 4:
                    ui->edTmFrom_4->setTime(valueFrom);
                    ui->edTmTo_4->setTime(valueTo);
                    break;
                case 5:
                    ui->edTmFrom_5->setTime(valueFrom);
                    ui->edTmTo_5->setTime(valueTo);
                    break;
                case 6:
                    ui->edTmFrom_6->setTime(valueFrom);
                    ui->edTmTo_6->setTime(valueTo);
                    break;
                case 7:
                    ui->edTmFrom_7->setTime(valueFrom);
                    ui->edTmTo_7->setTime(valueTo);
                    break;
                case 8:
                    ui->edTmFrom_8->setTime(valueFrom);
                    ui->edTmTo_8->setTime(valueTo);
                    break;
                case 9:
                    ui->edTmFrom_9->setTime(valueFrom);
                    ui->edTmTo_9->setTime(valueTo);
                    break;
                default:
                    break;
            }
        }
    }
}


dlgTimeBlockPeriods::~dlgTimeBlockPeriods()
{
    delete ui;
}

void dlgTimeBlockPeriods::on_buttonBox_accepted()
{
    QTime valueFrom, valueTo;
    QDir dir;
    QString path("/home/dirk/tmp/");
    char dataBuffer[80];
    FILE *sysFP;
    char cmdBuff[80];

    // We create the directory if needed
    if (!dir.exists(path))
        dir.mkpath(path); // You can check the success if needed

    // Werte speichern
    QFile file(path + "new_timeblocks.def");
    if( !file.open(QIODevice::ReadWrite | QIODevice::Text) )
    {
        qDebug() << "FAIL TO CREATE FILE / FILE NOT EXIST ***";
    }



    valueFrom = ui->edTmFrom_0->time();
    valueTo = ui->edTmTo_0->time();
    sprintf(dataBuffer, "%d:%d %d:%d\n", valueFrom.hour(), valueFrom.minute(), valueTo.hour(), valueTo.minute() );
    file.write(dataBuffer, qstrlen(dataBuffer));

    valueFrom = ui->edTmFrom_1->time();
    valueTo = ui->edTmTo_1->time();
    sprintf(dataBuffer, "%d:%d %d:%d\n", valueFrom.hour(), valueFrom.minute(), valueTo.hour(), valueTo.minute() );
    file.write(dataBuffer, qstrlen(dataBuffer));

    valueFrom = ui->edTmFrom_2->time();
    valueTo = ui->edTmTo_2->time();
    sprintf(dataBuffer, "%d:%d %d:%d\n", valueFrom.hour(), valueFrom.minute(), valueTo.hour(), valueTo.minute() );
    file.write(dataBuffer, qstrlen(dataBuffer));

    valueFrom = ui->edTmFrom_3->time();
    valueTo = ui->edTmTo_3->time();
    sprintf(dataBuffer, "%d:%d %d:%d\n", valueFrom.hour(), valueFrom.minute(), valueTo.hour(), valueTo.minute() );
    file.write(dataBuffer, qstrlen(dataBuffer));

    valueFrom = ui->edTmFrom_4->time();
    valueTo = ui->edTmTo_4->time();
    sprintf(dataBuffer, "%d:%d %d:%d\n", valueFrom.hour(), valueFrom.minute(), valueTo.hour(), valueTo.minute() );
    file.write(dataBuffer, qstrlen(dataBuffer));

    valueFrom = ui->edTmFrom_5->time();
    valueTo = ui->edTmTo_5->time();
    sprintf(dataBuffer, "%d:%d %d:%d\n", valueFrom.hour(), valueFrom.minute(), valueTo.hour(), valueTo.minute() );
    file.write(dataBuffer, qstrlen(dataBuffer));

    valueFrom = ui->edTmFrom_6->time();
    valueTo = ui->edTmTo_6->time();
    sprintf(dataBuffer, "%d:%d %d:%d\n", valueFrom.hour(), valueFrom.minute(), valueTo.hour(), valueTo.minute() );
    file.write(dataBuffer, qstrlen(dataBuffer));

    valueFrom = ui->edTmFrom_7->time();
    valueTo = ui->edTmTo_7->time();
    sprintf(dataBuffer, "%d:%d %d:%d\n", valueFrom.hour(), valueFrom.minute(), valueTo.hour(), valueTo.minute() );
    file.write(dataBuffer, qstrlen(dataBuffer));

    valueFrom = ui->edTmFrom_8->time();
    valueTo = ui->edTmTo_8->time();
    sprintf(dataBuffer, "%d:%d %d:%d\n", valueFrom.hour(), valueFrom.minute(), valueTo.hour(), valueTo.minute() );
    file.write(dataBuffer, qstrlen(dataBuffer));

    valueFrom = ui->edTmFrom_9->time();
    valueTo = ui->edTmTo_9->time();
    sprintf(dataBuffer, "%d:%d %d:%d\n", valueFrom.hour(), valueFrom.minute(), valueTo.hour(), valueTo.minute() );
    file.write(dataBuffer, qstrlen(dataBuffer));

    file.close();

    sprintf(cmdBuff, "~/bolus/bolus-cli -iT -I%s", file.fileName().toStdString().c_str() );
    if((sysFP = popen(cmdBuff, "r")) != nullptr )
    {
        fgets(dataBuffer, 79, sysFP);
        fclose(sysFP);
    }
}
