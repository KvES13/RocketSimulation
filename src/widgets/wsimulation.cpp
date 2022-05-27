#include "wsimulation.h"
#include "ui_wsimulation.h"

wSimulation::wSimulation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wSimulation)
{
    ui->setupUi(this);
    ui->pbResults->setEnabled(false);
    ui->textBrowser->setFontPointSize(12);
}

wSimulation::~wSimulation()
{
    delete ui;
}

void wSimulation::init(std::vector<RocketStage> &stages, Environment *env) {
    if(&stages != &stageVector)
    {
        stageVector = stages;
        environment = env;
    }
    else
    {
        qDebug()<<" EEEEEEEEEEEEEEEEE";
    }
}


void wSimulation::on_pbStartSim_clicked()
{

    QString text =
            "Запуск симуляции"
            "\nВремя пуска: "+environment->masterClock.UTC_date_init.toString()+
            "\nКоординаты старта: " +
            printEigenVector(stageVector[0].rocket->position.LLH,QStringList{"Долгота","Широта","Высота"}) +
    "\nНачальная скорость: "+
    printEigenVector(stageVector[0].rocket->velocity.NED,QStringList{"X","Y","Z"}) +
    "\nВремя полёта: "+ QString::number(stageVector[0].time_end) +
    ", шаг интегрирования: " + QString::number(stageVector[0].time_step) +


    "\nКоличество ступеней: " + QString::number(stageVector.size());
    for(const auto& stage : stageVector)
    {
        text += "\n==========================================================="
         "\nПараметры " + QString::number(stage.stageNumber) + " ступени:"
                "\nМасса: " + QString::number(stage.rocket->mass.Sum()) +
                "кг ,длина " + QString::number(stage.rocket->length) +
                "м ,диаметр " + QString::number(stage.rocket->diameter) + "м";
    }


    ui->textBrowser->append(text);

    auto start = std::chrono::system_clock::now();
    run();

    auto end = std::chrono::system_clock::now();
    auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    if (msec > 10000)
        text = QString::number(msec/1000)+" сек";
    else
        text = QString::number(msec)+" мс";

    ui->textBrowser->append("\n==========================================================="
                            "\nСимуляция завершена\nВремя расчёта: "+text);

    for(const auto& stage : stageVector)
    {
    text =
            "\nМаксимальная высота: " + QString::number(stage.fdr.max_alt) +
            "м\nМаксимальная скорость: " + QString::number(stage.fdr.max_speed) +
            "м/с\nМаксимальное ускорение: " + QString::number(stage.fdr.max_accelerarion) +
            "м/с^2\nРасстояние от точки старта: " + QString::number(stage.fdr.max_downrage) + "м";
    }

    ui->textBrowser->append(text);
    ui->pbResults->setEnabled(true);
    ui->pbStartSim->setEnabled(false);

    emit finish();
}



void wSimulation::run()
{
        DynamicsBase::state x0;
        for(size_t i=0; i < stageVector.size();++i)
        {
           auto rocket = stageVector[i].rocket.get();

            x0 = {rocket->position.ECI[0],rocket->position.ECI[1],rocket->position.ECI[2],
                 rocket->velocity.ECI[0], rocket->velocity.ECI[1], rocket->velocity.ECI[2],
                 rocket->attitude.quaternion[0],rocket->attitude.quaternion[1],rocket->attitude.quaternion[2],
                  rocket->attitude.quaternion[3],rocket->angular_velocity[0],rocket->angular_velocity[1],
                  rocket->angular_velocity[2],
                 rocket->mass.propellant};

            // Simulation
            stageVector[i].FlightSequence(environment,x0);

            QString str =qApp->applicationDirPath()+Constants::OutputPath+
                    "/stage"+QString::number(i)+".csv";
            stageVector[i].fdr.DumpCsv(str.toStdString());

            // Update next stage
            if (stageVector.size() != i+1 && stageVector[i].enable_separation) {
                RocketStage& next_stage = stageVector[i+1];

                next_stage.time_start = stageVector[i].time_separation;

                next_stage.rocket->position.ECI[0] = x0[0];
                next_stage.rocket->position.ECI[1] = x0[1];
                next_stage.rocket->position.ECI[2] = x0[2];
                next_stage.rocket->velocity.ECI[0] = x0[3];
                next_stage.rocket->velocity.ECI[1] = x0[4];
                next_stage.rocket->velocity.ECI[2] = x0[5];
                next_stage.rocket->attitude.quaternion[0] = x0[6];
                next_stage.rocket->attitude.quaternion[1] = x0[7];
                next_stage.rocket->attitude.quaternion[2] = x0[8];
                next_stage.rocket->attitude.quaternion[3] = x0[9];
                next_stage.rocket->angular_velocity[0] = x0[10];
                next_stage.rocket->angular_velocity[1] = x0[11];
                next_stage.rocket->angular_velocity[2] = x0[12];
            }
        }
}


void wSimulation::on_pbResults_clicked()
{
   // ui->tabWidget->setCurrentWidget(ui->tabTables);
}


void wSimulation::on_pushButton_clicked()
{
    ui->textBrowser->clear();
    ui->pbStartSim->setEnabled(true);
    emit reset();

}

