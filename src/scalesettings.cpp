#include "scalesettings.h"
#include "ui_scalesettings.h"


#include <QDebug>
ScaleSettings::ScaleSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScaleSettings)
{
    ui->setupUi(this);
    lastBtnChecked = getSelected();
    lastValue = getValue();

    ui->doubleSpinBox->setMaximum(std::numeric_limits<double>::max());

    QAbstractButton* btn = ui->scaleButtonGroup->button(BTN_LINEAR);
    connect(btn,SIGNAL(clicked(bool)),this,SLOT(enableApplyBtn(bool)));

    btn = ui->scaleButtonGroup->button(BTN_POWER);
    connect(btn,SIGNAL(clicked(bool)),this,SLOT(enableApplyBtn(bool)));

    btn = ui->scaleButtonGroup->button(BTN_GEOMETRIC);
    connect(btn,SIGNAL(clicked(bool)),this,SLOT(enableApplyBtn(bool)));

    btn = ui->scaleButtonGroup->button(BTN_LOGARITHMIC);
    connect(btn,SIGNAL(clicked(bool)),this,SLOT(enableApplyBtn(bool)));

    btn = ui->scaleButtonGroup->button(BTN_ROOT);
    connect(btn,SIGNAL(clicked(bool)),this,SLOT(enableApplyBtn(bool)));

    btn = ui->scaleButtonGroup->button(BTN_ASYMPTOTICAL);
    connect(btn,SIGNAL(clicked(bool)),this,SLOT(disableSpinBox(bool)));

    btn = ui->scaleButtonGroup->button(BTN_INVERSE);
    connect(btn,SIGNAL(clicked(bool)),this,SLOT(disableSpinBox(bool)));

    btn = ui->scaleButtonGroup->button(BTN_BALANCED);
    connect(btn,SIGNAL(clicked(bool)),this,SLOT(disableSpinBox(bool)));
}

ScaleSettings::~ScaleSettings()
{
    delete ui;
}

int ScaleSettings::getSelected()
{
    return ui->scaleButtonGroup->checkedId();
}

double ScaleSettings::getValue()
{
    return ui->doubleSpinBox->value();
}

void ScaleSettings::on_okBtn_clicked()
{
    on_applyBtn_clicked();
    done(getSelected());
}

void ScaleSettings::on_cancelBtn_clicked()
{
    ui->scaleButtonGroup->button(lastBtnChecked)->setChecked(true);
    ui->doubleSpinBox->setValue(lastValue);
    done(getSelected());
}

void ScaleSettings::on_applyBtn_clicked()
{
    lastBtnChecked = getSelected();
    lastValue = getValue();
    ui->applyBtn->setDisabled(true);
}

void ScaleSettings::reject()
{
    on_cancelBtn_clicked();
}

void ScaleSettings::enableApplyBtn(bool checked)
{
    if(ui->scaleButtonGroup->checkedId() == BTN_LINEAR){
        ui->doubleSpinBox->setMinimum(0.0001);
    }else{
        ui->doubleSpinBox->setMinimum(1.0001);
        if(ui->doubleSpinBox->minimum() == ui->doubleSpinBox->value()){
            ui->doubleSpinBox->setValue(2);
        }
    }
    ui->doubleSpinBox->setEnabled(true);
    ui->applyBtn->setEnabled(true);
}

void ScaleSettings::disableSpinBox(bool checked)
{
    enableApplyBtn(checked);
    ui->doubleSpinBox->setDisabled(true);
}

void ScaleSettings::on_doubleSpinBox_valueChanged(double arg1)
{
    ui->applyBtn->setEnabled(true);
}
