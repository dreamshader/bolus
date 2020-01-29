#include "dlghealthoptions.h"
#include "ui_dlghealthoptions.h"

dlgHealthOptions::dlgHealthOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgHealthOptions)
{
    pParent = parent;

    ui->setupUi(this);
}

dlgHealthOptions::~dlgHealthOptions()
{
    delete ui;
}
