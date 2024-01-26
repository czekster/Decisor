#ifndef SCALESETTINGS_H
#define SCALESETTINGS_H

#include <QDialog>

namespace Ui {
class ScaleSettings;
}

class ScaleSettings : public QDialog
{
    Q_OBJECT

public:
    explicit ScaleSettings(QWidget *parent = 0);
    ~ScaleSettings();
    enum BTN_ID{BTN_LINEAR = -4, BTN_POWER = -5, BTN_GEOMETRIC = -6, BTN_LOGARITHMIC = -2,
                BTN_ROOT = -3, BTN_ASYMPTOTICAL = -7, BTN_INVERSE = -8, BTN_BALANCED = -9};
    int getSelected();
    double getValue();

private slots:
    void on_okBtn_clicked();
    void on_cancelBtn_clicked();
    void on_applyBtn_clicked();
    void reject();
    void enableApplyBtn(bool checked);
    void disableSpinBox(bool checked);
    void on_doubleSpinBox_valueChanged(double arg1);

private:
    Ui::ScaleSettings *ui;
    int lastBtnChecked;
    double lastValue;

};

#endif // SCALESETTINGS_H
