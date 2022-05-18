#include "engine.h"

#include "csvreader.h"
#include "jsonwrapper.h"


void Engine::Reset() {
    burning = false;

    thrust = 0.0;
    mdot_prop = 0.0;
    gimbal_angle_y_axis = 0.0;
    gimbal_angle_z_axis = 0.0;
    mis_alignment_angle_y_axis = 0.0;
    mis_alignment_angle_z_axis = 0.0;

    enable_gimbal = false;

    area_exit = 0.0;
    burn_duration = 0.0;
};

void Engine::getTotalImpulse(const double thrust, const double burn_duration) {
    total_impulse = thrust * burn_duration;
};

void Engine::getTotalImpulse(const std::vector<double> &thrust,
                             const double burn_duration) {
    total_impulse = std::accumulate(thrust.begin(), thrust.end(), 0.0) / thrust.size() * burn_duration;
};


Engine::Engine(const double burn_duration,
              const double thrust_const,
              const double thrust_coef, const double ISP,const double  diameter_exit,
              const double area_exit) {
    Reset();
    enable_gimbal = false;

    this->area_exit = area_exit;
    this->diameter = diameter_exit;
    this->burn_duration = burn_duration;
    thrust_vacuum_src = InterpolateParameter(thrust_const);
    this->thrust_coefficient = thrust_coef;
    this->thrust = thrust_const;
    this->ISPvac = ISP;

    getTotalImpulse(thrust_const, burn_duration);
};



Engine::Engine(const std::vector<double> &time_vector,
               const std::vector<double> &thrust_vacuum_vector,
               const double area_exit, const double  diameter_exit,const double isp) {
    Reset();
    enable_gimbal = false;

    this->diameter = diameter_exit;
    this->area_exit = area_exit;
    this->burn_duration = time_vector[time_vector.size()-1];
    thrust_vacuum_src = InterpolateParameter(time_vector, thrust_vacuum_vector, "zero");

    ISPvac = isp;
    getTotalImpulse(thrust_vacuum_vector, time_vector.back());
};


void Engine::Update(const double t, const double pressure) {
    if (t <= burn_duration) {
        Ignittion();
    } else {
        Cutoff();
    }

    if (burning) {
        this->thrust = thrust_vacuum_src(t) - (pressure * area_exit);//(pressure *0.3);// area_exit);
        mdot_prop = thrust/ISPvac/Constants::g_earth;
    } else {
        thrust = 0.0;
        mdot_prop = 0.0;
    }

    gimbal_angle_y_axis = mis_alignment_angle_y_axis;
    gimbal_angle_z_axis = mis_alignment_angle_z_axis;
};

void Engine::Ignittion() {
    burning = true;
};

void Engine::Cutoff() {
    burning = false;
}

void Engine::setAreaExit(double diameter_exit)
{
    diameter = diameter_exit;
    area_exit = 0.25 * std::pow(diameter_exit, 2) * Constants::pi;
};

Engine Engine::create(JsonWrapper &json)
{
    JsonWrapper jsEngine(json);
    double diameter_exit = json.getDouble("Throat diameter[m]");
    double isp_vac =json.getDouble("Const Isp vac[s]");
    double nozzle_expansion_ratio = json.getDouble("nozzle expansion ratio[-]");

    double area_exit = 0.25 * std::pow(diameter_exit, 2) * Constants::pi;

    if (json.getBool("Enable Thrust File")) {
        JsonWrapper jc_file =  json.getSubItem("Thrust File");
        auto thrust_log = LoadCsvLog(jc_file.getString("Thrust at vacuum File Path").toStdString());
        return Engine(thrust_log[0], thrust_log[1], diameter_exit,area_exit,isp_vac);
        }
    else {
        auto jc_const = json.getSubItem("Constant Thrust");
        double thrust_coef = jc_const.getDouble("Thrust coefficient[-]");
        double thrust = jc_const.getDouble("Thrust at vacuum [N]");
   //     double mdot_p = jc_const.getDouble("Propellant Mass Flow Rate [kg/s]");
        double burn_duration = jc_const.getDouble("Burn Duration [sec]");
       return Engine(burn_duration, thrust,thrust_coef,isp_vac, diameter_exit,area_exit);
       }
};



