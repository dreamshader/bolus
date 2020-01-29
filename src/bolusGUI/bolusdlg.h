#ifndef BOLUSDLG_H
#define BOLUSDLG_H

#include <QDialog>
#include "dlgconfirmbolus.h"

namespace Ui {
class bolusDlg;
}

class bolusDlg : public QDialog
{
    Q_OBJECT

public:
    explicit bolusDlg(QWidget *parent = nullptr);
    ~bolusDlg();

    dlgConfirmBolus *pDlgConfirmBolus;

private slots:
    void on_btnBolusCalc_clicked();

    void on_btnMainMenu_clicked();

private:
    Ui::bolusDlg *ui;
    QWidget *pParent;
};

#endif // BOLUSDLG_H
