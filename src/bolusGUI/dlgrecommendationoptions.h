#ifndef DLGRECOMMENDATIONOPTIONS_H
#define DLGRECOMMENDATIONOPTIONS_H

#include <QDialog>

namespace Ui {
class dlgRecommendationOptions;
}

class dlgRecommendationOptions : public QDialog
{
    Q_OBJECT

public:
    explicit dlgRecommendationOptions(QWidget *parent = nullptr);
    ~dlgRecommendationOptions();

    unsigned int varVersion;
    int varTimeBlocksActive;
    int varIncreaseLevel;
    int varSnacksize10BE;
    int varActTime;
    int varDelayTime;
    int varBasalActTime;
    int varBasalDelayTime;


private slots:
    void on_dlgRecommendationOptions_finished(int result);

private:
    Ui::dlgRecommendationOptions *ui;
    QWidget *pParent;

};

#endif // DLGRECOMMENDATIONOPTIONS_H
