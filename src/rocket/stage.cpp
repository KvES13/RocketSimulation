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

RocketStage RocketStage::create(int stage,JsonWrapper &json)
{
    RocketStage rs(stage, std::make_shared<Rocket>(json));

    auto js = json.getSubItem("Events");

    rs.time_start = js.getDouble("Flight Start Time [s]");
    rs.time_ignittion = js.getDouble("Engine Ignittion Time [s]");

    rs.enable_launcher = js.getBool("Enable Rail-Launcher Launch");
    if (rs.enable_launcher) {
        rs.length_launcher_rail =  js.getSubItem("Rail Launcher").getDouble("Length [m]");
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

    rs.enable_rocket_dynamic = js.getBool("Enable Rocket Dynamic Method");

    rs.enable_parachute_open = js.getBool("Enable Parachute Open");
    if (rs.enable_parachute_open) {
        auto jc_parachute =  js.getSubItem("Parachute");      
        auto trigger = jc_parachute.getString("Trigger");
        if (trigger == "time")
        {
            rs.parachute_open_height_trigger = false;
            rs.time_open_parachute = jc_parachute.getDouble("Open Time [s]");
        }
        else if(trigger == "height")
        {
            rs.parachute_open_height_trigger = true;
            rs.parachute_height_open =  jc_parachute.getDouble("Open Height [m]");
        }
        else
        {
            rs.enable_parachute_open = false;
        }

    }

    rs.exist_second_parachute = js.getBool("Enable Secondary Parachute Open");
    if (rs.exist_second_parachute) {
        auto jc_secondary_parachute = js.getSubItem("Secondary Parachute");

        auto trigger = jc_secondary_parachute.getString("Trigger");
        if (trigger == "time")
        {
            rs.second_parachute_open_height_trigger = false;
            rs.time_open_second_parachute = jc_secondary_parachute.getDouble("Open Time [s]");
        }
        else if(trigger == "height")
        {
            rs.second_parachute_open_height_trigger = true;
            rs.second_parachute_height_open =  jc_secondary_parachute.getDouble("Open Height [m]");
        }
        else
        {
            rs.exist_second_parachute = false;
        }
    }

    rs.time_end =  js.getDouble("Flight End Time [s]");
    rs.time_step = js.getDouble("Time Step [s]");

    return rs;


}

void RocketStage::FlightSequence(Environment *env, DynamicsBase::state &x0)
{
    namespace odeint = boost::numeric::odeint;

    // Stepper Select
    double eps_abs = 1.0e-6;
    double eps_rel =  1.0e-6;

    // 5th Order Runge-Kutta-Dormand-Prince Method : Controled : Dense output : Internal info
    using base_stepper_type = odeint::runge_kutta_dopri5<DynamicsBase::state>;
    auto stepper = make_dense_output(eps_abs, eps_rel, 1.0, base_stepper_type());

    double start, end=10;

    fdr.ReserveCapacity(static_cast<int>((time_end - time_start) / time_step) * 1.3);

    DynamicsBase::state x0_in_stage = x0;

    if (time_ignittion <= time_start) {
        rocket->IgnitionEngine(env->masterClock.UTC_date_init,
                               env->masterClock.countup_time);
        start = time_start;
    } else {
        Dynamics6dofAero aero(rocket.get(), env);
        odeint::integrate_const(stepper, std::ref(aero), x0_in_stage,
                                time_start, time_ignittion, time_step, std::ref(fdr));
        start = time_ignittion;
        rocket->IgnitionEngine(env->masterClock.UTC_date_init,
                               env->masterClock.countup_time);
    }

    if (enable_launcher) {
        double time_step_on_launcher = 0.1;
        Rocket rocket_on_launcher = *rocket.get();
        Environment env_launch = *env;
        DynamicsBase::state x0_on_launcher = x0;
        FlightObserver fdr_on_launcher(&rocket_on_launcher);
        Dynamics3dofOnLauncher launcher(&rocket_on_launcher, &env_launch);
        odeint::integrate_const(stepper, std::ref(launcher), x0_on_launcher,
                                start, start+10, time_step_on_launcher,
                                std::ref(fdr_on_launcher));

        fdr_on_launcher.DumpCsv("test_estimate_launcher.csv");

        for (int i=0; i < fdr_on_launcher.countup_burn_time.size(); ++i) {
            double distance = (fdr_on_launcher.position[i].LLH(2) -
                               fdr_on_launcher.position[0].LLH(2)) /
                    sin(fdr_on_launcher.attitude[i].euler_angle(1));

            if (distance >= length_launcher_rail) {
                end = time_step_on_launcher * i;
                break;
            }
        }
        launcher.reset(rocket.get(),env);
        odeint::integrate_const(stepper, std::ref(launcher), x0_in_stage,
                                start, start + end, time_step_on_launcher,
                                std::ref(fdr));
        start = start + end;

        fdr.DumpCsv("debug_onlauncher_log.csv");

    }

    Dynamics6dofAero aero(rocket.get(), env);


    if (enable_fairing_jettson) {
        odeint::integrate_const(stepper, std::ref(aero), x0_in_stage, start,
                                time_jettson_fairing, time_step, std::ref(fdr));
        start = time_jettson_fairing;
        rocket->JettsonFairing(mass_fairing);
    }

    if (enable_separation) {
        odeint::integrate_const(stepper, std::ref(aero), x0_in_stage, start,
                                time_separation, time_step, std::ref(fdr));
        start = time_separation;
        rocket->SeparateUpperStage(mass_upper_stage);
        x0 = x0_in_stage;

//        Eigen::Vector3d euler {99,0, 0};
//        euler = euler / 180.0 * Constants::pi;
//        rocket->attitude.Initialize(euler);
    }


    if (!enable_parachute_open && !enable_rocket_dynamic) {
        odeint::integrate_const(stepper, std::ref(aero), x0_in_stage, start,
                                time_end, time_step, std::ref(fdr));
    }
    else if(enable_rocket_dynamic)
    {
        odeint::integrate_const(stepper, std::ref(aero), x0_in_stage, start,
                                time_end, time_step, std::ref(fdr));
//        // 1 Включение
//        double t =tempIntegrate(&aero,*env,x0_in_stage,start,70000);
//        qDebug()<<"t" <<t;
//        aero.reset(rocket.get(), env);

//        odeint::integrate_const(stepper, std::ref(aero), x0_in_stage, start,
//                                t, time_step, std::ref(fdr));

//        double burn = t+30;
//        rocket->engine.burn_duration = burn;
//   //     rocket->mass.inert +=50000;
//        rocket->engine.setThrust(4274233); //2742333 3274233

//       // DynamicTest launcher(rocket.get(),env);
//        odeint::integrate_const(stepper, std::ref(aero), x0_in_stage, t,
//                                burn, time_step, std::ref(fdr));

//        // 2 Включение
//        double t2 =tempIntegrate(&aero,*env,x0_in_stage,burn,10000);
//        aero.reset(rocket.get(), env);
//           qDebug()<<"t2" <<t2;


//        odeint::integrate_const(stepper, std::ref(aero), x0_in_stage, burn,
//                                t2, time_step, std::ref(fdr));

//        rocket->engine.burn_duration = t2+20;
//        rocket->engine.setThrust(3000000); //2742333


//        odeint::integrate_const(stepper, std::ref(aero), x0_in_stage, t2,
//                                time_end, time_step, std::ref(fdr));

    }
    else {
        // Если парашют срабатывает на заданной высоте
        if (parachute_open_height_trigger) {
            time_open_parachute = 556;//tempIntegrate(&aero,*env,x0_in_stage,start,parachute_height_open);
        }

        // Интегрирование до t срабатывания парашюта
     //   aero.reset(rocket.get(), env);
        odeint::integrate_const(stepper, std::ref(aero), x0_in_stage, start,
                                time_open_parachute, time_step, std::ref(fdr));
        start = time_open_parachute;

        rocket->OpenFirstParachute();
        double time_step_decent_parachute = 0.1;
        odeint::runge_kutta4<DynamicsBase::state> stepper;
        Dynamics3dofParachute parachute(rocket.get(), env);

        // Если существует 2 парашют
        if (exist_second_parachute) {
            // Если парашют срабатывает на заданной высоте
            if(second_parachute_height_open)
            {
                time_open_second_parachute = tempIntegrate(&parachute,*env,x0_in_stage,start,second_parachute_height_open);
                parachute.reset(rocket.get(), env);
            }

            // Интегрирование до t срабатывания 2 парашюта
            odeint::integrate_const(stepper, parachute, x0_in_stage, start,
                                    time_open_second_parachute,
                                    time_step_decent_parachute, std::ref(fdr));
            rocket->OpenSecondParachute();
            // Интегрирование до конца
            odeint::integrate_const(stepper, parachute, x0_in_stage,
                                    time_open_second_parachute, time_end,
                                    time_step_decent_parachute, std::ref(fdr));
        }

        else
        {
            odeint::integrate_const(stepper, parachute, x0_in_stage, start,
                                    time_end, time_step_decent_parachute,
                                    std::ref(fdr));
        }
    }
}


double RocketStage::tempIntegrate(DynamicsBase *base,Environment env,
                                  DynamicsBase::state x0,double start,
                                  double height)
{
    Rocket r = *rocket.get();
    FlightObserver fdr_apogee_estimate(&r);
    base->reset(&r, &env);
    using namespace boost::numeric::odeint;
    using base_stepper_type = runge_kutta_dopri5<DynamicsBase::state>;
    auto stepper = make_dense_output(1.0e-6, 1.0e-6, 1.0, base_stepper_type());
    integrate_const(stepper, std::ref(*base), x0, start, time_end,
                    time_step,std::ref(fdr_apogee_estimate));

    return timeSearchByHeight(fdr_apogee_estimate,height);
}

double RocketStage::timeSearchByHeight(const FlightObserver& obs,double height)
{
    for (int i=0; i < obs.position.size(); ++i) {
        double alt = obs.position[i].LLH(2);
        if (height >= alt) {
             return obs.countup_time[i];
        }
    }
    qDebug()<<"Отсутствует такая высота";
    return time_end;
}
