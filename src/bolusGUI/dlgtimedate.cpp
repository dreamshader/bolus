#include "dlgtimedate.h"
#include "ui_dlgtimedate.h"

dlgTimeDate::dlgTimeDate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgTimeDate)
{
    pParent = parent;

    ui->setupUi(this);
}

dlgTimeDate::~dlgTimeDate()
{
    delete ui;
}
