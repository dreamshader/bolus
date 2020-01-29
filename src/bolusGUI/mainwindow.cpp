#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bolus.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    pBolusDialog = nullptr;
    pDataDialog = nullptr;
    pSettingsDialog = nullptr;
    pInputGlucoValue = nullptr;


    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnBolus_clicked()
{
    extern struct _bolus_param gParam;

    if( pInputGlucoValue == nullptr )
    {
        pInputGlucoValue = new inputGlucoValue(ui->centralWidget);
        pInputGlucoValue->show();

        if( pInputGlucoValue->exec() == QDialog::Accepted )
        {

int f = gParam.glucose;
char c = gParam.measType;

            if( pBolusDialog == nullptr )
            {
                pBolusDialog = new bolusDlg(ui->centralWidget);
                pBolusDialog->show();
                pBolusDialog->exec();
                pBolusDialog = nullptr;
            }
        }
        pInputGlucoValue = nullptr;
    }

#ifdef NEVERDEF
    else
    {

    }

    if( pBolusDialog == nullptr )
    {
        pBolusDialog = new bolusDlg(ui->centralWidget);
        pBolusDialog->show();

        pBolusDialog->exec();
        pBolusDialog = nullptr;
    }
    else
    {
        pBolusDialog->done(0);
        pBolusDialog = nullptr;
    }
#endif //NEVERDEF
}


void MainWindow::on_btnData_clicked()
{
    if( pDataDialog == nullptr )
    {
        pDataDialog = new dataDlg(ui->centralWidget);
        pDataDialog->show();

        pDataDialog->exec();
        pDataDialog = nullptr;
    }
#ifdef NEVERDEF
    else
    {
        pDataDialog->done(0);
        pDataDialog = nullptr;
    }
#endif //NEVERDEF

}

void MainWindow::on_btnSettings_clicked()
{
    if( pSettingsDialog == nullptr )
    {
        pSettingsDialog = new settingsDlg(ui->centralWidget);
        pSettingsDialog->show();

        pSettingsDialog->exec();
        pSettingsDialog = nullptr;
    }
#ifdef NEVERDEF
    else
    {
        pSettingsDialog->done(0);
        pSettingsDialog = nullptr;
    }
#endif //NEVERDEF

}

void MainWindow::on_action_Beenden_triggered()
{
    this->close();
}
