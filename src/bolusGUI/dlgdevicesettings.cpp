#include "dlgdevicesettings.h"
#include "ui_dlgdevicesettings.h"

dlgDeviceSettings::dlgDeviceSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgDeviceSettings)
{
    pParent = parent;

    ui->setupUi(this);
}

dlgDeviceSettings::~dlgDeviceSettings()
{
    delete ui;
}
