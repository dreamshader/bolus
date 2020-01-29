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

    int varSports1;
    int varSports2;
    int varStress;
    int varIll;
    int varFemale;

private:
    Ui::dlgHealthOptions *ui;
    QWidget *pParent;

};

#endif // DLGHEALTHOPTIONS_H
