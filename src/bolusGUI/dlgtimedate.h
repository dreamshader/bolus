#ifndef DLGTIMEDATE_H
#define DLGTIMEDATE_H

#include <QDialog>

namespace Ui {
class dlgTimeDate;
}

class dlgTimeDate : public QDialog
{
    Q_OBJECT

public:
    explicit dlgTimeDate(QWidget *parent = nullptr);
    ~dlgTimeDate();

private:
    Ui::dlgTimeDate *ui;
    QWidget *pParent;

};

#endif // DLGTIMEDATE_H
