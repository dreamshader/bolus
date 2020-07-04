#include "dlgsettingsbolus.h"
#include "ui_dlgsettingsbolus.h"
#include "ui_dlgtimeblockperiods.h"

dlgSettingsBolus::dlgSettingsBolus(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgSettingsBolus)
{
    pParent = parent;

    ptimeBlocks = nullptr;
    phealthOptions = nullptr;
    precommendationOptions = nullptr;
    pDlgTimeBlockPeriods = nullptr;

    ui->setupUi(this);
}

dlgSettingsBolus::~dlgSettingsBolus()
{
    delete ui;
}

void dlgSettingsBolus::on_btnTimeBlocks_clicked()
{
    if( ptimeBlocks == nullptr )
    {
        ptimeBlocks = new dlgTimeBlocks(pParent);
        ptimeBlocks->show();
        ptimeBlocks->exec();
        ptimeBlocks = nullptr;
    }
}

void dlgSettingsBolus::on_btnHealthEvents_clicked()
{

    if( phealthOptions == nullptr )
    {
        phealthOptions = new dlgHealthOptions(pParent);
        phealthOptions->show();
        phealthOptions->exec();
        phealthOptions = nullptr;
    }
}

void dlgSettingsBolus::on_btnBolusOptions_clicked()
{
    if( precommendationOptions == nullptr )
    {
        precommendationOptions = new dlgRecommendationOptions(pParent);
        precommendationOptions->show();
        precommendationOptions->exec();
        precommendationOptions = nullptr;
    }
}

void dlgSettingsBolus::on_btnTimeBlocksDefine_clicked()
{
    if( pDlgTimeBlockPeriods == nullptr )
    {
        pDlgTimeBlockPeriods = new dlgTimeBlockPeriods();
        pDlgTimeBlockPeriods->show();
        pDlgTimeBlockPeriods->update();

        if( pDlgTimeBlockPeriods->exec() == QDialog::Accepted )
        {

        }

        pDlgTimeBlockPeriods = nullptr;
    }
}
