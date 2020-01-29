#include "dlgshowdata.h"
#include "ui_dlgshowdata.h"

dlgShowData::dlgShowData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgShowData)
{
    pParent = parent;

    ui->setupUi(this);
}

dlgShowData::~dlgShowData()
{
    delete ui;
}
