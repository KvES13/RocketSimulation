#ifndef FLIGHTSETTINGS_H
#define FLIGHTSETTINGS_H

#include <QWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
//#include <chrono>

#define EIGEN_MPL2_ONLY
#include "Eigen/Core"

#include "jsonwrapper.h"
#include "Constants.h"
#include "environment.h"
#include "stage.h"
#include "wrocket.h"
//#include "dynamics/dynamics_base.h"
//#include "environment/sequence_clock.h"


namespace Ui {
class FlightSettings;
}

class FlightSettings : public QWidget
{
    Q_OBJECT

public:
    explicit FlightSettings(QWidget *parent = nullptr);
    ~FlightSettings();

signals:
    void startSim(std::vector<RocketStage>& stages,Environment* env);

private slots:
    ///
    /// \brief Обработка события нажатия на кнопку "Старт"
    ///
    void on_btnStartSimulation_clicked();

    ///
    /// \brief Обработка события нажатия на кнопку "Обзор" конфигурационного файла
    ///
    void on_btnSelectCfgFile_clicked();

    ///
    /// \brief Обработка события нажатия на кнопку "Загрузить"
    ///
    void on_btnLoadCfg_clicked();
    ///
    /// \brief Обработка события изменения состояния чекбокса
    /// "Сохранение симуляции в файл
    /// \param arg1 Состояние чекбокса
    ///
    void on_cbSaveSim_stateChanged(int arg1);
    ///
    /// \brief Обработка события нажатия на кнопку "Обзор" файла для сохранения
    ///
    void on_btnSelectSaveFile_clicked();
    ///
    /// \brief on_sbStagesNumber_valueChanged
    /// \param arg1
    ///
    void on_sbStagesNumber_valueChanged(int arg1);

    void on_leStartLongtitude_editingFinished();

    void on_leStartLatitude_editingFinished();

    void on_leStartAltitude_editingFinished();

    void on_leStartAzimuth_editingFinished();

    void on_leStartElevation_editingFinished();

private:
    std::vector<RocketStage> v_stages;
    std::shared_ptr<Environment> environment;
    QVector<WRocket*> v_wstages;
    Ui::FlightSettings *ui;
};

#endif // FLIGHTSETTINGS_H
