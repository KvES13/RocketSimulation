#include "stage.h"

#include <QElapsedTimer>
#include <boost/numeric/odeint.hpp>
#include "rocket/rocket.h"

#include "jsonwrapper.h"

RocketStage::RocketStage(int stageNumber, std::shared_ptr<Rocket> rocket)
{
    this->stageNumber = stageNumber;
    this->rocket = rocket;
}

RocketStage RocketStage::create(const int stage,
                                QString rocket_config_json_file,
                                QString engine_config_json_file,
                                QString sequence_of_event_json_file)
{
    RocketStage rs(stage, std::make_shared<Rocket>(rocket_config_json_file,engine_config_json_file));
    JsonWrapper js(sequence_of_event_json_file);
    rs.time_start = js.getDouble("Flight Start Time [s]");
    rs.time_ignittion = js.getDouble("Engine Ignittion Time [s]");

    rs.enable_launcher = js.getBool("Enable Rail-Launcher Launch");
    if (rs.enable_launcher) {
        rs.length_launcher_rail =  js.getSubItem("Rail Launcher").getDouble("Length [m]");
    }

    rs.enable_cutoff =  js.getBool("Enable Engine Cutoff");
    if (rs.enable_cutoff) {
        rs.time_cutoff = js.getSubItem("Cutoff").getDouble("Cutoff Time [s]");
        rs.rocket->engine.burn_duration = rs.time_cutoff;
    }

    rs.enable_separation =  js.getBool("Enable Stage Separation");
    if (rs.enable_separation) {
        auto jc_separation =  js.getSubItem("Upper Stage");
        rs.time_separation = jc_separation.getDouble("Stage Separation Time [s]");
        rs.mass_upper_stage = jc_separation.getDouble("Upper Stage Mass [kg]");
    }

    rs.enable_fairing_jettson = js.getBool("Enable Fairing Jettson");
    if (rs.enable_fairing_jettson) {
        auto jc_fairing_jettson = js.getSubItem("Fairing");
        rs.time_jettson_fairing = jc_fairing_jettson.getDouble("Jettson Time [s]");
        rs.mass_fairing = jc_fairing_jettson.getDouble("Mass [s]");
    }

    rs.enable_parachute_open = js.getBool("Enable Parachute Open");
    if (rs.enable_parachute_open) {
        auto jc_parachute =  js.getSubItem("Parachute");
        rs.enable_apogee_parachute_open = jc_parachute.getDouble("Enable Forced Apogee Open");
        rs.time_open_parachute = jc_parachute.getDouble("Open Time [s]");
    }

    rs.exist_second_parachute = js.getBool("Enable Secondary Parachute Open");
    if (rs.exist_second_parachute) {
        auto jc_secondary_parachute = js.getSubItem("Secondary Parachute");
        rs.time_open_second_parachute = jc_secondary_parachute.getDouble("Open Time [s]");
    }

    rs.time_end =  js.getDouble("Flight End Time [s]");
    rs.time_step = js.getDouble("Time Step [s]");

    return rs;


}

void RocketStage::FlightSequence(Environment *env, DynamicsBase::state &x0)
{
    namespace odeint = boost::numeric::odeint;

    // Sterpper Select
    ////////////////////////////////////
    double eps_abs = 1.0e-6;//1.0e-12;
    double eps_rel =  1.0e-6;//1.0e-7;
    // 4th Order Runge-Kutta Method
    // odeint::runge_kutta4<forrocket::DynamicsBase::state> stepper;

    // 5th Order Runge-Kutta-Dormand-Prince Method : Controled : Dense output : Internal info
    using base_stepper_type = odeint::runge_kutta_dopri5<DynamicsBase::state>;
   // auto stepper = make_controlled( eps_abs , eps_rel , base_stepper_type());
    auto stepper = make_dense_output(eps_abs, eps_rel, 1.0, base_stepper_type());
    // 8th Order Runge-Kutta-Fehlberg Method : Controled
    // using base_stepper_type = odeint::runge_kutta_fehlberg78<forrocket::DynamicsBase::state>;
    // auto stepper = make_controlled( eps_abs , eps_rel , base_stepper_type());

    // Dynamics6dofAero dynamics_6dof_aero(&rocket, master_clock, env);
    // Dynamics6dofProgramRate dynamics_6dof_programrate(&rocket, master_clock, env);
    DynamicsBase* p_dynamics;
 //   auto env = new Environment(new Atmosphere()); /// @todo MEMORY LEAK
    p_dynamics = new Dynamics6dofAero(rocket.get(), env);

    double start, end=10;
    double time_step_on_launcher = 0.1;  // 1000 Hz
    double time_step_decent_parachute = 0.1;  // 10 Hz
    fdr.ReserveCapacity(static_cast<int>((time_end - time_start) / time_step) * 1.3);

    DynamicsBase::state x0_in_stage = x0;

    if (time_ignittion <= time_start) {
        rocket->IgnitionEngine(env->masterClock.UTC_date_init, env->masterClock.countup_time);
        start = time_start;
    } else {
        p_dynamics = new Dynamics6dofAero(rocket.get(), env);
    //    SwitchDynamics(time_start, &p_dynamics, master_clock, env);
        odeint::integrate_const(stepper, std::ref(*p_dynamics), x0_in_stage, time_start, time_ignittion, time_step, std::ref(fdr));
        start = time_ignittion;
        std::cout<<"=========================start = "<<time_ignittion<<std::endl;
        rocket->IgnitionEngine(env->masterClock.UTC_date_init, env->masterClock.countup_time);
    }

    if (enable_launcher) {
        Rocket rocket_on_launcher = *rocket.get();
        Environment env_launch = *env;
      //  SequenceClock clock_on_launcher = env->masterClock.; // copy
        DynamicsBase::state x0_on_launcher = x0;
        FlightObserver fdr_on_launcher(&rocket_on_launcher);
        p_dynamics = new Dynamics3dofOnLauncher(&rocket_on_launcher, &env_launch);
        odeint::integrate_const(stepper, std::ref(*p_dynamics), x0_on_launcher, start, start+10, time_step_on_launcher, std::ref(fdr_on_launcher));
        std::cout<<"=========================enable_launcher "<<std::endl;

        fdr_on_launcher.DumpCsv("test_estimate_launcher.csv");


        for (int i=0; i < fdr_on_launcher.countup_burn_time.size(); ++i) {
            double distance = (fdr_on_launcher.position[i].LLH(2) - fdr_on_launcher.position[0].LLH(2)) / sin(fdr_on_launcher.attitude[i].euler_angle(1));
            if (distance >= length_launcher_rail) {
                end = time_step_on_launcher * i;
                break;
            }
        }
        p_dynamics = new Dynamics3dofOnLauncher(rocket.get(),env);
        //stepper.initialize(std::ref(*p_dynamics), x0_in_stage, start);
        odeint::integrate_const(stepper, std::ref(*p_dynamics), x0_in_stage, start, start + end, time_step_on_launcher, std::ref(fdr));
        // odeint::integrate_const(stepper, std::ref(*p_dynamics), x0_in_stage, start, start + end, time_step_on_launcher, std::ref(fdr));
        start = start + end;

        fdr.DumpCsv("debug_onlauncher_log.csv");

    }
    // до сих пор было выполнено воспламенение от инерционного полета или чистого полета.
    //  Включение/выключение двигателя
    if (enable_cutoff) {
        std::cout<<"=========================enable_cutoff "<<std::endl;
        p_dynamics = new Dynamics6dofAero(rocket.get(), env);
       // SwitchDynamics(start, &p_dynamics, master_clock, env);
       // stepper.initialize(std::ref(*p_dynamics), x0_in_stage, start);
        odeint::integrate_const(stepper, std::ref(*p_dynamics), x0_in_stage, start, time_cutoff, time_step, std::ref(fdr));
        start = time_cutoff;
        rocket->CutoffEngine();
    }

    if (enable_fairing_jettson) {
        std::cout<<"enable_fairing_jettson "<<std::endl;
       // SwitchDynamics(start, &p_dynamics, master_clock, env);
        p_dynamics = new Dynamics6dofAero(rocket.get(), env);
       // stepper.initialize(std::ref(*p_dynamics), x0_in_stage, start);
        odeint::integrate_const(stepper, std::ref(*p_dynamics), x0_in_stage, start, time_jettson_fairing, time_step, std::ref(fdr));
        start = time_jettson_fairing;
        rocket->JettsonFairing(mass_fairing);
    }

    if (enable_separation) {
        std::cout<<"=========================enable_sepation "<<std::endl;
        // SwitchDynamics(start, &p_dynamics, master_clock, env);
         p_dynamics = new Dynamics6dofAero(rocket.get(),env);
     //   stepper.initialize(std::ref(*p_dynamics), x0_in_stage, start);
        odeint::integrate_const(stepper, std::ref(*p_dynamics), x0_in_stage, start, time_separation, time_step, std::ref(fdr));
        start = time_separation;
        rocket->SeparateUpperStage(mass_upper_stage);
        x0 = x0_in_stage;
    }

    if (!enable_parachute_open) {
        QElapsedTimer t;
        t.start();


        // SwitchDynamics(start, &p_dynamics, master_clock, env);
         p_dynamics = new Dynamics6dofAero(rocket.get(), env);
    //    stepper.initialize(std::ref(*p_dynamics), x0_in_stage, start);
        odeint::integrate_const(stepper, std::ref(*p_dynamics), x0_in_stage, start, time_end, time_step, std::ref(fdr));
         std::cout<<t.elapsed()<<"======================= e!enable_parachute_open "/*<<((Dynamics6dofAero*)p_dynamics)->count*/<<std::endl;
    } else {
        QElapsedTimer t;
        t.start();
        // Парашютный зонт
        if (enable_apogee_parachute_open) {
            std::cout<<"=========================enable_apogee_parachute_open "<<std::endl;
            // Поворот с копией для определения времени вершины
            Rocket rocket_apogee_estimate = *rocket.get();
            Environment env_apogee = *env;
            DynamicsBase::state x0_apogee_estimate = x0;
            FlightObserver fdr_apogee_estimate(&rocket_apogee_estimate);
            p_dynamics = new Dynamics6dofAero(rocket.get(), &env_apogee);
            odeint::integrate_const(stepper, std::ref(*p_dynamics), x0_apogee_estimate, start, time_end, time_step, std::ref(fdr_apogee_estimate));
            double max_alt = 0.0;
            for (int i=0; i < fdr_apogee_estimate.position.size(); ++i) {
                double alt = fdr_apogee_estimate.position[i].LLH(2);
                if (max_alt < alt) {
                    max_alt = alt;
                } else {
                    end = fdr_apogee_estimate.countup_time[i];
                    break;
                }
            }
        } else {
            end = time_open_parachute;
        }
        // Инерциальная летная книжка до открытия зонтика
        std::cout<<"========================= MAIN INTEGRATE"<<std::endl;
        std::cout<<start<<" <--> "<<end;
        // SwitchDynamics(start, &p_dynamics, master_clock, env);
         p_dynamics = new Dynamics6dofAero(rocket.get(),env);
    //    stepper.initialize(std::ref(*p_dynamics), x0_in_stage, start);
        odeint::integrate_const(stepper, std::ref(*p_dynamics), x0_in_stage, start, end, time_step, std::ref(fdr));
        start = end;

        // до сих пор он работал до вершины или времени открытия.
        odeint::runge_kutta4<DynamicsBase::state> stepper;
        rocket->OpenParachute();
        Dynamics3dofParachute dynamics_3dof_parachute(rocket.get(), env);

        if (exist_second_parachute) {

            odeint::integrate_const(stepper, dynamics_3dof_parachute, x0_in_stage, start, time_open_second_parachute, time_step_decent_parachute, std::ref(fdr));
            rocket->OpenParachute();
            odeint::integrate_const(stepper, dynamics_3dof_parachute, x0_in_stage, time_open_second_parachute, time_end, time_step_decent_parachute, std::ref(fdr));
        } else {
            QElapsedTimer tp;
            tp.start();
            odeint::integrate_const(stepper, dynamics_3dof_parachute, x0_in_stage, start, time_end, time_step_decent_parachute, std::ref(fdr));
            std::cout<<tp.elapsed()<<" ======================= exist_parashute_parachute "<<std::endl;
        }
        std::cout<<t.elapsed()<<" ======================= enable_parachute_open "<<std::endl;
    }
}


