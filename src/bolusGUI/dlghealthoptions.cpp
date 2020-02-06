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

    if((sysFP = popen("~/bolus/bolus-cli -qA", "r")) != nullptr )
    {
        fgets(dataBuff, 79, sysFP);
        fclose(sysFP);

        varSober = atoi(&dataBuff[0]);
        varSport1 = atoi(&dataBuff[5]);
        varStress = atoi(&dataBuff[10]);
        varIllness = atoi(&dataBuff[15]);
        varSport2 = atoi(&dataBuff[20]);
        varMenstruation = atoi(&dataBuff[25]);
        varOther = atoi(&dataBuff[30]);

        ui->spnSoberFactor->setValue(varSober);
        ui->spnSport1Factor->setValue(varSport1);
        ui->spnStressFactor->setValue(varStress);
        ui->spnIllnessFactor->setValue(varIllness);
        ui->spnSport2Factor->setValue(varSport2);
        ui->spnFemaleFactor->setValue(varMenstruation);
        ui->spnOtherFactor->setValue(varOther);

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
        varSober = ui->spnSoberFactor->value();
        varSport1 = ui->spnSport1Factor->value();
        varStress = ui->spnStressFactor->value();
        varIllness = ui->spnIllnessFactor->value();
        varSport2 = ui->spnSport2Factor->value();
        varMenstruation = ui->spnFemaleFactor->value();
        varOther = ui->spnOtherFactor->value();

        QDir dir;
        QString path("/home/dirk/tmp/");

        // We create the directory if needed
        if (!dir.exists(path))
            dir.mkpath(path); // You can check the success if needed

        // Werte speichern
        QFile file(path + "bolus-export-health.csv");
        if( !file.open(QIODevice::ReadWrite | QIODevice::Text) )
        {
            qDebug() << "FAIL TO CREATE FILE / FILE NOT EXIT***";
        }
        else
        {
            char dataBuffer[80];
            int retVal;
            sprintf(dataBuffer, "%d;%d;%d;%d;%d;%d;%d\n", varSober, varSport1, varStress, varIllness, varSport2, varMenstruation, varOther );
            file.write(dataBuffer, qstrlen(dataBuffer));
            file.close();
            path.append("bolus-export-health.csv");
            sprintf(dataBuffer, "~/bolus/bolus-cli -D -v2 -I%s -iA",path.toStdString().c_str() );
            retVal = system(dataBuffer);

        }
    }
}
