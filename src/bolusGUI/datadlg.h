#ifndef DATADLG_H
#define DATADLG_H

#include <QDialog>
#include "dlgshowdata.h"

namespace Ui {
class dataDlg;
}

class dataDlg : public QDialog
{
    Q_OBJECT

public:
    explicit dataDlg(QWidget *parent = nullptr);
    ~dataDlg();
    dlgShowData *pShowDataDlg;

private slots:
    void on_btnDataEdit_clicked();

    void on_btnDataExport_clicked();

    void on_btnDataImport_clicked();

    void on_btnDataAnalyze_clicked();

    void on_btnSettingsExport_clicked();

    void on_btnSettingsImport_clicked();

private:
    Ui::dataDlg *ui;
    QWidget *pParent;
};

#endif // DATADLG_H
