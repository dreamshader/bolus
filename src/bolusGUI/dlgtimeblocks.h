#ifndef DLGTIMEBLOCKS_H
#define DLGTIMEBLOCKS_H

#include <QDialog>
#include <QListWidget>
#include "dlgtimeblockedit.h"

namespace Ui {
class dlgTimeBlocks;
}

class dlgTimeBlocks : public QDialog
{
    Q_OBJECT

public:
    explicit dlgTimeBlocks(QWidget *parent = nullptr);
    ~dlgTimeBlocks();
    dlgTimeBlockEdit *pDlgTimeBlockEdit;

private slots:
    void on_lstTimeBlocks_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::dlgTimeBlocks *ui;
    QWidget *pParent;

};

#endif // DLGTIMEBLOCKS_H
