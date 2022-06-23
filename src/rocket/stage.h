#ifndef STAGE_H
#define STAGE_H

#include <memory>

#include "rocket/flightobserver.h"
#include "dynamics/dynamics_base.h"
#include "dynamics/dynamics_6dof_aero.h"
#include "dynamics/dynamics_3dof_onlauncher.h"
#include "dynamics/dynamics_3dof_parachute.h"
#include "jsonwrapper.h"

#include "dynamics/dynamictest.h"
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

    ///
    /// \brief Метод для моделирования
    /// \param env Окружение
    /// \param x0 Массив состояния
    ///
    void FlightSequence(Environment *env, DynamicsBase::state& x0);


    double tempIntegrate(DynamicsBase *base,Environment env, DynamicsBase::state x0,double start,double height);
    double timeSearchByHeight(const FlightObserver& obs, double height);

    int stageNumber;
    std::shared_ptr<Rocket> rocket;
    FlightObserver fdr;

    ///////////////////////
    double time_start = 0;
    double time_ignittion= 0;

    bool enable_launcher= false;
    double length_launcher_rail= 0;

    ///////////////////////
    bool enable_separation;
    double time_separation= 0;
    double mass_upper_stage= 0;

    bool enable_fairing_jettson= false;
    double time_jettson_fairing= 0;
    double mass_fairing= 0;
    ///////////////////////

    ///////////////////////
    bool enable_rocket_dynamic = false;
    bool enable_parachute_open= false;
    bool parachute_open_height_trigger = false;
    double time_open_parachute= 0;
    double parachute_height_open = 0;

    bool exist_second_parachute = false;
    double time_open_second_parachute= 0;
    bool second_parachute_open_height_trigger = false;
    double second_parachute_height_open = 0;
    ///////////////////////

    double time_end= 0;
    double time_step= 0;

};

#endif // STAGE_H
