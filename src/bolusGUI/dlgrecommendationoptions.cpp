#include "dlgrecommendationoptions.h"
#include "ui_dlgrecommendationoptions.h"

dlgRecommendationOptions::dlgRecommendationOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgRecommendationOptions)
{
    pParent = parent;

    ui->setupUi(this);
}

dlgRecommendationOptions::~dlgRecommendationOptions()
{
    delete ui;
}
