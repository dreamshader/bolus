#ifndef DLGNOENV_H
#define DLGNOENV_H

#include <QDialog>

namespace Ui {
class dlgNoEnv;
}

class dlgNoEnv : public QDialog
{
    Q_OBJECT

public:
    explicit dlgNoEnv(QWidget *parent = nullptr);
    ~dlgNoEnv();

private:
    Ui::dlgNoEnv *ui;
};

#endif // DLGNOENV_H
