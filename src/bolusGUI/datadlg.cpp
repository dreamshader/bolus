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

    if( !fileName.isEmpty() )
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

void dataDlg::on_btnDataExport_clicked()
{
    QString selectedFilter;

    fileName = QFileDialog::getSaveFileName(pParent,
        tr( "Export nach" ), nullptr,
        tr( "csv-Datei (*.csv);; Alle Dateien (*.*)" ),
        &selectedFilter, QFileDialog::DontUseNativeDialog );

    if( !fileName.isEmpty() )
    {
    }
}

void dataDlg::on_btnDataImport_clicked()
{
    QString selectedFilter;

    fileName = QFileDialog::getOpenFileName(pParent,
        tr( "Datendatei auswählen" ), nullptr,
        tr( "csv-Dateien (*.csv);; Alle Dateien (*.*)" ),
        &selectedFilter, QFileDialog::DontUseNativeDialog );

    if( !fileName.isEmpty() )
    {
    }
}

void dataDlg::on_btnDataAnalyze_clicked()
{
    QString selectedFilter;

    fileName = QFileDialog::getOpenFileName(pParent,
        tr( "Datendatei auswählen" ), nullptr,
        tr( "csv-Dateien (*.csv);; Alle Dateien (*.*)" ),
        &selectedFilter, QFileDialog::DontUseNativeDialog );

    if( !fileName.isEmpty() )
    {
    }
}

void dataDlg::on_btnSettingsExport_clicked()
{
    QString selectedFilter;

    fileName = QFileDialog::getSaveFileName(pParent,
        tr( "Export nach" ), nullptr,
        tr( "csv-Datei (*.csv);; Alle Dateien (*.*)" ),
        &selectedFilter, QFileDialog::DontUseNativeDialog );

    if( !fileName.isEmpty() )
    {
    }
}

void dataDlg::on_btnSettingsImport_clicked()
{
    QString selectedFilter;

    fileName = QFileDialog::getOpenFileName(pParent,
        tr( "Datendatei auswählen" ), nullptr,
        tr( "csv-Dateien (*.csv);; Alle Dateien (*.*)" ),
        &selectedFilter, QFileDialog::DontUseNativeDialog );

    if( !fileName.isEmpty() )
    {
    }
}
