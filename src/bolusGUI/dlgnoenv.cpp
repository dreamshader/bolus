#include "dlgnoenv.h"
#include "ui_dlgnoenv.h"

dlgNoEnv::dlgNoEnv(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgNoEnv)
{
    ui->setupUi(this);
}

dlgNoEnv::~dlgNoEnv()
{
    delete ui;
}
