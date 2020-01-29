#ifndef DATADLG_H
#define DATADLG_H

#include <QDialog>

namespace Ui {
class dataDlg;
}

class dataDlg : public QDialog
{
    Q_OBJECT

public:
    explicit dataDlg(QWidget *parent = nullptr);
    ~dataDlg();

private:
    Ui::dataDlg *ui;
    QWidget *pParent;
};

#endif // DATADLG_H
