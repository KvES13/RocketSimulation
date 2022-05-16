#ifndef STAGE_H
#define STAGE_H

#include <memory>

#include "rocket/flightobserver.h"
#include "dynamics/dynamics_base.h"
#include "dynamics/dynamics_6dof_aero.h"
#include "dynamics/dynamics_3dof_onlauncher.h"
#include "dynamics/dynamics_3dof_parachute.h"

class Rocket;

class RocketStage
{
public:
    RocketStage(int stageNumber, std::shared_ptr<Rocket> rocket);

    static RocketStage create(const int stage,
                              QString rocket_config_json_file,
                              QString engine_config_json_file,
                              QString sequence_of_event_json_file);


    void FlightSequence(Environment *env, DynamicsBase::state& x0);
//    void SwitchDynamics(const double time_start, DynamicsBase** dynamics,
//                        SequenceClock* master_clock, Environment* env);

    int stageNumber;
    std::shared_ptr<Rocket> rocket;
    FlightObserver fdr;

    ///////////////////////
    double time_start;
    double time_ignittion;

    bool enable_launcher;
    double length_launcher_rail;

    bool enable_cutoff;
    double time_cutoff;


    ///////////////////////
    bool enable_separation;
    double time_separation;
    double mass_upper_stage;

    bool enable_fairing_jettson;
    double time_jettson_fairing;
    double mass_fairing;
    ///////////////////////

    ///////////////////////
    bool enable_parachute_open;
    bool enable_apogee_parachute_open;
    double time_open_parachute;

    bool exist_second_parachute;
    double time_open_second_parachute;
    ///////////////////////

    double time_end;
    double time_step;

};

#endif // STAGE_H
