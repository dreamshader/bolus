#ifndef SETTINGSDLG_H
#define SETTINGSDLG_H

#include <QDialog>
#include "dlgsettingsbolus.h"
#include "dlgtimedate.h"
#include "dlgdevicesettings.h"


namespace Ui {
class settingsDlg;
}

class settingsDlg : public QDialog
{
    Q_OBJECT

public:
    explicit settingsDlg(QWidget *parent = nullptr);
    ~settingsDlg();
    dlgSettingsBolus *pSettingsBolus;
    dlgTimeDate *pTimeDate;
    dlgDeviceSettings *pDeviceSettings;

private slots:
    void on_btnBolus_clicked();

    void on_btnDevice_clicked();

    void on_btnDateTime_clicked();

private:
    Ui::settingsDlg *ui;
    QWidget *pParent;

};

#endif // SETTINGSDLG_H
