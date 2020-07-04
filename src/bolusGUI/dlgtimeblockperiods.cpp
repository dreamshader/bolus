#include "dlgtimeblockperiods.h"
#include "ui_dlgtimeblockperiods.h"

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
