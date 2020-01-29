#include "inputglucovalue.h"
#include "ui_inputglucovalue.h"
#include "bolus.h"

inputGlucoValue::inputGlucoValue(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::inputGlucoValue)
{
    pParent = parent;

    ui->setupUi(this);
}

inputGlucoValue::~inputGlucoValue()
{
    delete ui;
}

void inputGlucoValue::on_inputGlucoValue_finished(int result)
{
    extern struct _bolus_param gParam;

    if( result == QDialog::Accepted )
    {
        gParam.glucose = ui->txtGlucoValue->toPlainText().toInt();

        if( ui->radioFreestyle->isChecked() )
        {
            // -t f --type=(f)reestyle
            gParam.measType = 'f';
        }
        else
        {
            // -t a --type=(a)cucheck
            gParam.measType = 'a';
        }
    }
}
