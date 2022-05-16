#ifndef WSIMULATION_H
#define WSIMULATION_H

#include <QWidget>
#include <QDebug>

#include "Constants.h"
#include "stage.h"
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
            stageVector = stages;
            environment = env;
        }
            };

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
