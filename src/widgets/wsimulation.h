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
   // void BackToSettings();
    void finish();
    void reset();
public slots:
    void init(std::vector<RocketStage>& stages, Environment* env);

private slots:
    void on_pbStartSim_clicked();
    void on_pbResults_clicked();

    void on_pushButton_clicked();

private:
    void run();

    int index = 0;


    Environment *environment;
    Ui::wSimulation *ui;
};

#endif // WSIMULATION_H
