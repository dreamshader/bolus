#include "settingsdlg.h"
#include "ui_settingsdlg.h"

settingsDlg::settingsDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settingsDlg)
{
    pSettingsBolus = nullptr;
    pDeviceSettings = nullptr;

    pParent = parent;
    ui->setupUi(this);
}

settingsDlg::~settingsDlg()
{
    delete ui;
}

void settingsDlg::on_btnBolus_clicked()
{
    if( pSettingsBolus == nullptr )
    {
        pSettingsBolus = new dlgSettingsBolus(pParent);
        pSettingsBolus->show();

        pSettingsBolus->exec();
        pSettingsBolus = nullptr;
    }
}

void settingsDlg::on_btnDevice_clicked()
{
    if( pDeviceSettings == nullptr )
    {
        pDeviceSettings = new dlgDeviceSettings(pParent);
        pDeviceSettings->show();

        pDeviceSettings->exec();
        pDeviceSettings = nullptr;
    }
}

