#include "datadlg.h"
#include "ui_datadlg.h"
#include <QFileDialog>

dataDlg::dataDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dataDlg)
{
    pParent = parent;
    pShowDataDlg = nullptr;

    ui->setupUi(this);
}

dataDlg::~dataDlg()
{
    delete ui;
}

QString fileName;

void dataDlg::on_btnDataEdit_clicked()
{
    QString selectedFilter;

    fileName = QFileDialog::getOpenFileName(pParent,
        tr( "Datendatei auswählen" ), nullptr,
        tr( "Bolus Datendateien (bolus.data.*);; Alle Dateien (*.*)" ),
        &selectedFilter, QFileDialog::DontUseNativeDialog );


// "Images (*.png *.xpm *.jpg);;Text files (*.txt);;XML files (*.xml)"

    if( fileName.length() )
    {
        if( pShowDataDlg == nullptr )
        {
            pShowDataDlg = new dlgShowData(pParent);
            pShowDataDlg->show();

            pShowDataDlg->exec();
            pShowDataDlg = nullptr;
        }
    }

}
