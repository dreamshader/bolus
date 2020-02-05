#ifndef DLGHEALTHOPTIONS_H
#define DLGHEALTHOPTIONS_H

#include <QDialog>

namespace Ui {
class dlgHealthOptions;
}

class dlgHealthOptions : public QDialog
{
    Q_OBJECT

public:
    explicit dlgHealthOptions(QWidget *parent = nullptr);
    ~dlgHealthOptions();

    int varSober;
    int varSport1;
    int varStress;
    int varIllness;
    int varSport2;
    int varMenstruation;
    int varOther;


private slots:
    void on_dlgHealthOptions_finished(int result);

private:
    Ui::dlgHealthOptions *ui;
    QWidget *pParent;

};

#endif // DLGHEALTHOPTIONS_H
