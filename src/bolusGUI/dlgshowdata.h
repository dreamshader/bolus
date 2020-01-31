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
    void displayDataRecord( void );

private slots:
    void on_btnPrevRec_clicked();

    void on_btnNextRec_clicked();

private:
    Ui::dlgShowData *ui;
    QWidget *pParent;

    int currRecno;
    bool eofReached;

};

#endif // DLGSHOWDATA_H
