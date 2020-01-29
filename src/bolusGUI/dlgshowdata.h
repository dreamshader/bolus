#ifndef DLGSHOWDATA_H
#define DLGSHOWDATA_H

#include <QDialog>

namespace Ui {
class dlgShowData;
}

class dlgShowData : public QDialog
{
    Q_OBJECT

public:
    explicit dlgShowData(QWidget *parent = nullptr);
    ~dlgShowData();

private:
    Ui::dlgShowData *ui;
    QWidget *pParent;

};

#endif // DLGSHOWDATA_H
