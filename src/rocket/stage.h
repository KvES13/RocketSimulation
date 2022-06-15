#ifndef STAGE_H
#define STAGE_H

#include <memory>

#include "rocket/flightobserver.h"
#include "dynamics/dynamics_base.h"
#include "dynamics/dynamics_6dof_aero.h"
#include "dynamics/dynamics_3dof_onlauncher.h"
#include "dynamics/dynamics_3dof_parachute.h"
#include "jsonwrapper.h"

class Rocket;

///
/// \brief Класс, описывающий ступень ракеты
///
class RocketStage
{
public:
    ///
    /// \brief Конструктор класса
    /// \param stageNumber Номер ступени
    /// \param rocket Указатель на ракету
    ///
    RocketStage(int stageNumber, std::shared_ptr<Rocket> rocket);

    ///
    /// \brief Статический метод для создания экземпляра класса
    /// \param stage Номер ступни
    /// \param json Класс для работы с json
    /// \return Экземпляр класса
    ///
    static RocketStage create(int stage, JsonWrapper &json);

//    RocketStage(const RocketStage& stage)
//    {

//        rocket = std::make_shared<Rocket>(*stage.rocket);
//        stageNumber = stage.stageNumber;
//        fdr = stage.fdr;
//        time_start = stage.time_start;
//        time_ignittion = stage.time_ignittion;
//        enable_launcher = stage.enable_launcher;
//        length_launcher_rail = stage.length_launcher_rail;
//        enable_cutoff = stage.enable_cutoff;
//        time_cutoff = stage.time_cutoff;
//        enable_separation =stage.enable_separation;
//        time_separation = stage.time_separation;
//        mass_upper_stage = stage.mass_upper_stage;
//        enable_fairing_jettson = stage.enable_fairing_jettson;
//        time_jettson_fairing = stage.time_jettson_fairing;
//        mass_fairing = stage.mass_fairing;
//        enable_parachute_open = stage.enable_parachute_open;
//        enable_apogee_parachute_open = stage.enable_apogee_parachute_open;
//        time_open_parachute = stage.time_open_parachute;
//        exist_second_parachute = stage.exist_second_parachute;
//        time_open_second_parachute = stage.time_open_second_parachute;
//        time_end = stage.time_end;
//        time_step = stage.time_step;

//    }
    ///
    /// \brief Метод для моделирования
    /// \param env Окружение
    /// \param x0 Массив состояния
    ///
    void FlightSequence(Environment *env, DynamicsBase::state& x0);

    int stageNumber;
    std::shared_ptr<Rocket> rocket;
    FlightObserver fdr;

    ///////////////////////
    double time_start = 0;
    double time_ignittion= 0;

    bool enable_launcher= false;
    double length_launcher_rail= 0;

    bool enable_cutoff= false;
    double time_cutoff= 0;


    ///////////////////////
    bool enable_separation;
    double time_separation= 0;
    double mass_upper_stage= 0;

    bool enable_fairing_jettson= false;
    double time_jettson_fairing= 0;
    double mass_fairing= 0;
    ///////////////////////

    ///////////////////////
    bool enable_parachute_open= false;
    bool enable_apogee_parachute_open = false;
    double time_open_parachute= 0;

    bool exist_second_parachute;
    double time_open_second_parachute= 0;
    ///////////////////////

    double time_end= 0;
    double time_step= 0;

};

#endif // STAGE_H
