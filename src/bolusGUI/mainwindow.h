#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "bolusdlg.h"
#include "datadlg.h"
#include "settingsdlg.h"
#include "inputglucovalue.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bolusDlg *pBolusDialog;
    dataDlg *pDataDialog;
    settingsDlg *pSettingsDialog;
    inputGlucoValue *pInputGlucoValue;

private slots:
    void on_btnBolus_clicked();

    void on_btnData_clicked();

    void on_btnSettings_clicked();

    void on_action_Beenden_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
