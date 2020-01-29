#include "datadlg.h"
#include "ui_datadlg.h"

dataDlg::dataDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dataDlg)
{
    pParent = parent;

    ui->setupUi(this);
}

dataDlg::~dataDlg()
{
    delete ui;
}
