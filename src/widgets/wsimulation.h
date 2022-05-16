#ifndef WSIMULATION_H
#define WSIMULATION_H

#include <QWidget>
#include <QDebug>

#include "Constants.h"
//#include "environment.h"
#include "stage.h"
//#include "dynamics/dynamics_base.h"
//#include "environment/sequence_clock.h"
#include "utils/utils.h"

namespace Ui {
class wSimulation;
}

class wSimulation : public QWidget
{
    Q_OBJECT

public:
    explicit wSimulation(QWidget *parent = nullptr);
    ~wSimulation();
    std::vector<RocketStage> stageVector;
signals:
    void BackToSettings();
    void finish();
public slots:
    void init(std::vector<RocketStage>& stages, Environment* env) {
        if(&stages != &stageVector)
        {
         qDebug()<<"proverka";
            stageVector = stages;
            environment = env;
        }
            else
            qDebug()<<"ODINAKOV";};

private slots:
    void on_pbBackToSettings_clicked();
    void on_pbStartSim_clicked();
    void on_pbResults_clicked();

private:
    void run();
    void clear();

    int index = 0;


    Environment *environment;
    //SimulationGL *ogl;
    Ui::wSimulation *ui;
};

#endif // WSIMULATION_H
