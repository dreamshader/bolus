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
