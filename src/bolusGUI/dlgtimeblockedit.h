#ifndef DLGTIMEBLOCKEDIT_H
#define DLGTIMEBLOCKEDIT_H

#include <QDialog>

namespace Ui {
class dlgTimeBlockEdit;
}

class dlgTimeBlockEdit : public QDialog
{
    Q_OBJECT

public:
    explicit dlgTimeBlockEdit(QWidget *parent = nullptr);
    ~dlgTimeBlockEdit();

    int currTimeBlockIndex;

    void setTimeBlockIndex( int newValue )   { currTimeBlockIndex = newValue; }



private slots:
    void on_dlgTimeBlockEdit_accepted();

private:
    Ui::dlgTimeBlockEdit *ui;
    QWidget *pParent;

};

#endif // DLGTIMEBLOCKEDIT_H
