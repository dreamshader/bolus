#include "dlghealthoptions.h"
#include "ui_dlghealthoptions.h"
#include <QFile>
#include <QDir>
#include <QDebug>

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

void dlgHealthOptions::on_dlgHealthOptions_finished(int result)
{
    if( result == QDialog::Accepted )
    {
        varSports1 = ui->spnSport1Factor->value();
        varSports2 = ui->spnSport2Factor->value();
        varStress = ui->spnStressFactor->value();
        varIll = ui->spnIllnessFactor->value();
        varIll = ui->spnFemaleFactor->value();

        QDir dir;
        QString path("/home/dirk/tmp/");

        // We create the directory if needed
        if (!dir.exists(path))
            dir.mkpath(path); // You can check the success if needed

        // Werte speichern
        QFile file(path + "bolus-export.csv");
        if( !file.open(QIODevice::ReadWrite | QIODevice::Text) )
        {
            qDebug() << "FAIL TO CREATE FILE / FILE NOT EXIT***";
        }
        else
        {
            char dataBuffer[80];
            int retVal;
            sprintf(dataBuffer, "%s;%s;%s;%s;%s\n","sports1","sports1","stress","ill","female");
            file.write(dataBuffer, qstrlen(dataBuffer));
            sprintf(dataBuffer, "%d;%d;%d;%d;%d\n",varSports1,varSports2,varStress,varIll,varIll);
            file.write(dataBuffer, qstrlen(dataBuffer));
            file.close();
            path.append("bolus-export.csv");
            sprintf(dataBuffer, "~/bolus/bolus-cli -I%s",path.toStdString().c_str() );
            retVal = system(dataBuffer);

        }
    }
}
