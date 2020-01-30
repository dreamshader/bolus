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

private slots:
    void on_dlgConfirmBolus_finished(int result);

private:
    Ui::dlgConfirmBolus *ui;
    QWidget *pParent;

};

#endif // DLGCONFIRMBOLUS_H
