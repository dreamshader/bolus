#ifndef DLGSETTINGSBOLUS_H
#define DLGSETTINGSBOLUS_H

#include <QDialog>
#include "dlgtimeblocks.h"
#include "dlghealthoptions.h"
#include "dlgrecommendationoptions.h"
#include "dlgtimeblockperiods.h"


namespace Ui {
class dlgSettingsBolus;
}

class dlgSettingsBolus : public QDialog
{
    Q_OBJECT

public:
    explicit dlgSettingsBolus(QWidget *parent = nullptr);
    ~dlgSettingsBolus();

    dlgHealthOptions *phealthOptions;
    dlgTimeBlocks *ptimeBlocks;
    dlgRecommendationOptions *precommendationOptions;
    dlgTimeBlockPeriods *pDlgTimeBlockPeriods;

private slots:
    void on_btnTimeBlocks_clicked();

    void on_btnHealthEvents_clicked();

    void on_btnBolusOptions_clicked();

    void on_btnTimeBlocksDefine_clicked();

private:
    Ui::dlgSettingsBolus *ui;
    QWidget *pParent;

};

#endif // DLGSETTINGSBOLUS_H
