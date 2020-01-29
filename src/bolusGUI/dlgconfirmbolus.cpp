#include "dlgconfirmbolus.h"
#include "ui_dlgconfirmbolus.h"

dlgConfirmBolus::dlgConfirmBolus(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgConfirmBolus)
{
    extern int currCalcBolus;
    pParent = parent;

    ui->setupUi(this);
    ui->spnBolusValue->setValue(currCalcBolus);
}

dlgConfirmBolus::~dlgConfirmBolus()
{
    delete ui;
}
