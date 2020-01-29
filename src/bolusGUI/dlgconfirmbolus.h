#ifndef DLGCONFIRMBOLUS_H
#define DLGCONFIRMBOLUS_H

#include <QDialog>

namespace Ui {
class dlgConfirmBolus;
}

class dlgConfirmBolus : public QDialog
{
    Q_OBJECT

public:
    explicit dlgConfirmBolus(QWidget *parent = nullptr);
    ~dlgConfirmBolus();

private:
    Ui::dlgConfirmBolus *ui;
    QWidget *pParent;

};

#endif // DLGCONFIRMBOLUS_H
