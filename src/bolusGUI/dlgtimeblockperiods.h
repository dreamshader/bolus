#ifndef DLGTIMEBLOCKPERIODS_H
#define DLGTIMEBLOCKPERIODS_H

#include <QDialog>

namespace Ui {
class dlgTimeBlockPeriods;
}

class dlgTimeBlockPeriods : public QDialog
{
    Q_OBJECT

public:
    explicit dlgTimeBlockPeriods(QWidget *parent = nullptr);
    ~dlgTimeBlockPeriods();

private:
    Ui::dlgTimeBlockPeriods *ui;
    QWidget *pParent;

};

#endif // DLGTIMEBLOCKPERIODS_H
