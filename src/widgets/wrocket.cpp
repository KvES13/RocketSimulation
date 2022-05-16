#include "wrocket.h"
#include "ui_wrocket.h"

WRocket::WRocket(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WRocket)
{
    ui->setupUi(this);
    ui->gbParachute->setEnabled(false);
    ui->gbSeparation->setEnabled(false);
    ui->cbSecondParachute->setEnabled(false);
}

WRocket::~WRocket()
{
    delete ui;
}

void WRocket::fillWidget(const RocketStage& stage)
{
    auto rocket = stage.rocket.get();
    ui->leDiameter->setText(QString::number(rocket->diameter));
    ui->leLenght->setText(QString::number(rocket->length));
    ui->leMassInert->setText(QString::number(rocket->mass.inert));
    ui->lePropellant->setText(QString::number(rocket->mass.propellant));
    ui->leThrustVac->setText(QString::number(rocket->engine.thrust));
    ui->leThrustCoef->setText(QString::number(1));
    ui->leEngBurnDuration->setText(QString::number(rocket->engine.burn_duration));
    ui->leEngDiameter->setText(QString::number(rocket->engine.getDiameter()));
    ui->leISPVac->setText(QString::number(rocket->engine.getISPvac()));
    ui->leNozzleExpansionRatio->setText(QString::number(18));

    if(stage.enable_separation)
    {
        ui->gbSeparation->setEnabled(true);
        ui->leTimeSeparation->setText(QString::number(stage.time_separation));
        ui->leUpperStageMass->setText(QString::number(stage.mass_upper_stage));
    }
    if(stage.enable_parachute_open)
    {
        ui->gbParachute->setEnabled(true);
        ui->leParachuteOpen1->setText(QString::number(stage.time_open_parachute));
        bool second_parachute = stage.exist_second_parachute;
        ui->cbSecondParachute->setChecked(second_parachute);
        if(second_parachute)
        {
            ui->leParachuteOpen2->setText(QString::number(stage.time_open_second_parachute));
        }
        else
        {
            ui->leParachuteOpen2->setEnabled(false);
            ui->leParachuteOpen2->clear();
        }


    }


}
