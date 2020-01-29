#ifndef INPUTGLUCOVALUE_H
#define INPUTGLUCOVALUE_H

#include <QDialog>

namespace Ui {
class inputGlucoValue;
}

class inputGlucoValue : public QDialog
{
    Q_OBJECT

public:
    explicit inputGlucoValue(QWidget *parent = nullptr);
    ~inputGlucoValue();

private slots:
    void on_inputGlucoValue_finished(int result);

private:
    Ui::inputGlucoValue *ui;
    QWidget *pParent;

};

#endif // INPUTGLUCOVALUE_H
