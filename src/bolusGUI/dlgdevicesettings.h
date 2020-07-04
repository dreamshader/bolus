#ifndef DLGDEVICESETTINGS_H
#define DLGDEVICESETTINGS_H

#include <QDialog>

namespace Ui {
class dlgDeviceSettings;
}

class dlgDeviceSettings : public QDialog
{
    Q_OBJECT

public:
    explicit dlgDeviceSettings(QWidget *parent = nullptr);
    ~dlgDeviceSettings();

private:
    Ui::dlgDeviceSettings *ui;
    QWidget *pParent;

};

#endif // DLGDEVICESETTINGS_H
