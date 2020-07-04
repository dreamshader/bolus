#include "settingsdlg.h"
#include "ui_settingsdlg.h"

settingsDlg::settingsDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settingsDlg)
{
    pSettingsBolus = nullptr;
    pTimeDate = nullptr;
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
#ifdef NEVERDEF
    else
    {
        pSettingsBolus->done(0);
        pSettingsBolus = nullptr;
    }
#endif //NEVERDEF
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

void settingsDlg::on_btnDateTime_clicked()
{
    if( pTimeDate == nullptr )
    {
        pTimeDate = new dlgTimeDate(pParent);
        pTimeDate->show();

        pTimeDate->exec();
        pTimeDate = nullptr;
    }
}
