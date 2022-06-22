#include "wrocket.h"
#include "ui_wrocket.h"

WRocket::WRocket(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WRocket)
{
    ui->setupUi(this);
    ui->gbParachute->setEnabled(false);
    ui->gbSeparation->setEnabled(false);
  //  ui->cbSecondParachute->setEnabled(false);
    ui->gbRocketDynamic->setEnabled(false);
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
    ui->leEngBurnDuration->setText(QString::number(rocket->engine.burn_duration));
    ui->leEngDiameter->setText(QString::number(rocket->engine.getDiameter()));
    ui->leISPVac->setText(QString::number(rocket->engine.getISPvac()));


    // Ступень отделяется
    if(stage.enable_separation)
    {
        ui->gbSeparation->setEnabled(true);
        ui->gbSeparation->setChecked(true);
        ui->leTimeSeparation->setText(QString::number(stage.time_separation));
        ui->leUpperStageMass->setText(QString::number(stage.mass_upper_stage));
    }
    // Ступень имеет парашют
    if(stage.enable_parachute_open)
    {
        ui->gbParachute->setEnabled(true);
        ui->gbParachute->setChecked(true);
        ui->leParachuteTimeOpen1->setText(QString::number(stage.time_open_parachute));


        // Парашют срабатывает по высоте
        if(stage.parachute_open_height_trigger)
        {
            ui->rbHeightTrigger->setChecked(true);
            ui->leParachuteTimeOpen1->setEnabled(false);
            ui->leParachuteHeightOpen1->setText(QString::number(stage.parachute_height_open));
        }
        // Парашют срабатывает по времени
        else
        {
            ui->rbTimeTrigger->setChecked(true);
            ui->rbTimeTrigger->setEnabled(true);
            ui->leParachuteHeightOpen1->setEnabled(false);
            ui->leParachuteTimeOpen1->setText(QString::number(stage.time_open_parachute));
        }

        // Ступень имеет 2 парашют
        if ( stage.exist_second_parachute)
        {
            ui->cbSecondParachute->setChecked(true);
            ui->cbSecondParachute->setEnabled(true);
            // Парашют срабатывает по высоте
            if(stage.second_parachute_open_height_trigger)
            {
                ui->leParachuteTimeOpen2->setEnabled(false);
                ui->leParachuteHeightOpen2->setText(QString::number(stage.second_parachute_height_open));
            }
            // Парашют срабатывает по времени
            else
            {
                ui->leParachuteHeightOpen2->setEnabled(false);
                ui->leParachuteTimeOpen2->setText(QString::number(stage.time_open_second_parachute));
            }

        }
    }

}
