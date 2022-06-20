#include "flightobserver.h"

#include <fstream>
#include <iomanip>

FlightObserver::FlightObserver(const Rocket *rocket) {
    p_rocket = rocket;

};


void FlightObserver::ReserveCapacity(int capacity) {
    capacity = capacity/10;
    countup_time.reserve(capacity);
    countup_burn_time.reserve(capacity);
    thrust.reserve(capacity);
    mdot_prop.reserve(capacity);
    burning.reserve(capacity);
    gimbal_angle_y_axis.reserve(capacity);
    gimbal_angle_z_axis.reserve(capacity);
    mass_prop.reserve(capacity);
    mass.reserve(capacity);
    length_CG.reserve(capacity);
    length_CP.reserve(capacity);
    inertia_tensor.reserve(capacity);
    CA.reserve(capacity);
    CNa.reserve(capacity);
    Cld.reserve(capacity);
    Clp.reserve(capacity);
    Cmq.reserve(capacity);
    position.reserve(capacity);
    velocity.reserve(capacity);
    dynamic_pressure.reserve(capacity);
    acceleration.reserve(capacity);
    force.reserve(capacity);
    attitude.reserve(capacity);
 //   quaternion_dot.reserve(capacity);
    angular_velocity.reserve(capacity);
    angular_acceleration.reserve(capacity);
    angle_of_attack.reserve(capacity);
    sideslip_angle.reserve(capacity);
    moment.reserve(capacity);

    vposition.resize(9);
    vvelocity.resize(9);
    vforce.resize(9);
    vacceleration.resize(6);
    for(int i = 0; i < 9; i++)
    {
        vposition[i].reserve(capacity);
        vvelocity[i].reserve(capacity);
        vforce[i].reserve(capacity);

    }
    for(int i = 0; i < 6; i++)
    {
        vacceleration[i].reserve(capacity);
    }


};


void FlightObserver::operator()(const DynamicsBase::state& x, const double t)
{
    counter++;
    if (counter <10 && t > 1.0)
    {
        counter++;
        return;
    }

    counter = 0;

    if (p_rocket->position.LLH[2] >= 0.0 && t < 510.0) {
        countup_time.push_back(t);
        countup_burn_time.push_back(p_rocket->burn_clock.countup_time);
        thrust.push_back(p_rocket->engine.thrust);
        mdot_prop.push_back(p_rocket->engine.mdot_prop);
        burning.push_back(p_rocket->engine.burning);
        gimbal_angle_y_axis.push_back(p_rocket->engine.gimbal_angle_y_axis);
        gimbal_angle_z_axis.push_back(p_rocket->engine.gimbal_angle_z_axis);
        mass_prop.push_back(p_rocket->mass.propellant);
        mass.push_back(p_rocket->mass.inert + p_rocket->mass.propellant);
        length_CG.push_back(p_rocket->length_CG);
        length_CP.push_back(p_rocket->length_CP);
        inertia_tensor.push_back(p_rocket->inertia_tensor);
        CA.push_back(p_rocket->CA);
        CNa.push_back(p_rocket->CNa);
        Cld.push_back(p_rocket->Cld);
        Clp.push_back(p_rocket->Clp);
        Cmq.push_back(p_rocket->Cmq);
        position.push_back(p_rocket->position);
        velocity.push_back(p_rocket->velocity);
        dynamic_pressure.push_back(p_rocket->dynamic_pressure);
        acceleration.push_back(p_rocket->acceleration);
        force.push_back(p_rocket->force);
        attitude.push_back(p_rocket->attitude);
      //  quaternion_dot.push_back(p_rocket->quaternion_dot);
        angular_velocity.push_back(p_rocket->angular_velocity);
        angular_acceleration.push_back(p_rocket->angular_acceleration);
        angle_of_attack.push_back(p_rocket->angle_of_attack);
        sideslip_angle.push_back(p_rocket->sideslip_angle);
        moment.push_back(p_rocket->moment);

        vposition[0].append(p_rocket->position.ECEF[0]/1e3);
        vposition[1].append(p_rocket->position.ECEF[1]/1e3);
        vposition[2].append(p_rocket->position.ECEF[2]/1e3);
        vposition[3].append(p_rocket->position.ECI[0]/1e3);
        vposition[4].append(p_rocket->position.ECI[1]/1e3);
        vposition[5].append(p_rocket->position.ECI[2]/1e3);
        vposition[6].append(p_rocket->position.LLH[0]);
        vposition[7].append(p_rocket->position.LLH[1]);
        vposition[8].append(p_rocket->position.LLH[2]);

        vvelocity[0].append(p_rocket->velocity.ECEF[0]);
        vvelocity[1].append(p_rocket->velocity.ECEF[1]);
        vvelocity[2].append(p_rocket->velocity.ECEF[2]);
        vvelocity[3].append(p_rocket->velocity.ECI[0]);
        vvelocity[4].append(p_rocket->velocity.ECI[1]);
        vvelocity[5].append(p_rocket->velocity.ECI[2]);
        vvelocity[6].append(p_rocket->velocity.NED[0]);
        vvelocity[7].append(p_rocket->velocity.NED[1]);
        vvelocity[8].append(p_rocket->velocity.NED[2]);

        vacceleration[0].append(p_rocket->acceleration.ECI[0]);
        vacceleration[1].append(p_rocket->acceleration.ECI[1]);
        vacceleration[2].append(p_rocket->acceleration.ECI[2]);
        vacceleration[3].append(p_rocket->acceleration.body[0]);
        vacceleration[4].append(p_rocket->acceleration.body[1]);
        vacceleration[5].append(p_rocket->acceleration.body[2]);

        vforce[0].append(p_rocket->force.thrust[0]);
        vforce[1].append(p_rocket->force.thrust[1]);
        vforce[2].append(p_rocket->force.thrust[2]);
        vforce[3].append(p_rocket->force.aero[0]);
        vforce[4].append(p_rocket->force.aero[1]);
        vforce[5].append(p_rocket->force.aero[2]);
        vforce[6].append(p_rocket->force.gravity[0]);
        vforce[7].append(p_rocket->force.gravity[1]);
        vforce[8].append(p_rocket->force.gravity[2]);

        mach_number.append(p_rocket->velocity.mach_number);

        if(p_rocket->position.LLH[2] > max_alt)
            max_alt = p_rocket->position.LLH[2];

        double range = crd.distance_surface(
               position[0].LLH,p_rocket->position.LLH);
        downrage.append(range);
        if (max_downrage < range)
            max_downrage = range;

        double speed = p_rocket->velocity.ECI.norm();
        if(speed > max_speed)
            max_speed = speed;
        sum_velocity.append(speed);

        double acc = p_rocket->acceleration.ECI.norm();
        if(acc > max_accelerarion)
            max_accelerarion = acc;
    }
};


void FlightObserver::DumpCsv(const std::string &file_name, bool full_dump) {
    std::ofstream ofs(file_name, std::ios::out);

    ofs << "Time [s],";
    ofs << "Burn Time [s],";
    if (full_dump) {
        ofs << "distance surface,";
        ofs << "AirDensity [kg/m3],";
        ofs << "AirPressure [kPa],";
        ofs << "AirTemprature [K],";
        ofs << "SpeedOfSound [m/s],";
        ofs << "Propellant Mass [kg],";
        ofs << "Mass [kg],";
//        ofs << "X-C.G. [%],";
//        ofs << "X-C.P. [%],";
//        ofs << "StaticMargin [%],";
//        ofs << "xx_InertiaTensor [kg-m2],";
//        ofs << "xy_InertiaTensor [kg-m2],";
//        ofs << "xz_InertiaTensor [kg-m2],";
//        ofs << "yx_InertiaTensor [kg-m2],";
//        ofs << "yy_InertiaTensor [kg-m2],";
//        ofs << "yz_InertiaTensor [kg-m2],";
//        ofs << "zx_InertiaTensor [kg-m2],";
//        ofs << "zy_InertiaTensor [kg-m2],";
//        ofs << "zz_InertiaTensor [kg-m2],";
        ofs << "Thrust [N],";
//        ofs << "Mdot [kg/s],";
//        ofs << "Burning [0/1],";
//        ofs << "y-Gimbal [deg],";
//        ofs << "z-Gimbal [deg],";
//        ofs << "CA [-],";
//        ofs << "CNa [-],";
//        ofs << "Cld [-],";
//        ofs << "Clp [-],";
//        ofs << "Cmq [-],";
        ofs << "AoA [deg],";
        ofs << "AoS [deg],";
        ofs << "Fx-aero [N],";
        ofs << "Fy-aero [N],";
        ofs << "Fz-aero [N],";
        ofs << "Fx-thrust [N],";
        ofs << "Fy-thrust [N],";
        ofs << "Fz-thrust [N],";
        ofs << "Fx-gravity [N],";
        ofs << "Fy-gravity [N],";
        ofs << "Fz-gravity [N],";
        ofs << "Accx-body [m/s2],";
        ofs << "Accy-body [m/s2],";
        ofs << "Accz-body [m/s2],";
        ofs << "Gccx-body [G],";
        ofs << "Gccy-body [G],";
        ofs << "Gccz-body [G],";
        ofs << "Accx-ECI [m/s2],";
        ofs << "Accy-ECI [m/s2],";
        ofs << "Accz-ECI [m/s2],";
        ofs << "Vx-body [m/s],";
        ofs << "Vy-body [m/s],";
        ofs << "Vz-body [m/s],";
        ofs << "MachNumber [-],";
        ofs << "DynamicPressure [kPa],";
        ofs << "Vx-NED [m/s],";
        ofs << "Vy-NED [m/s],";
        ofs << "Vz-NED [m/s],";
        ofs << "Vx-ECEF [m/s],";
        ofs << "Vy-ECEF [m/s],";
        ofs << "Vz-ECEF [m/s],";
        ofs << "Vx-ECI [m/s],";
        ofs << "Vy-ECI [m/s],";
        ofs << "Vz-ECI [m/s],";
        ofs << "X-ECEF [km],";
        ofs << "Y-ECEF [km],";
        ofs << "Z-ECEF [km],";
        ofs << "X-ECI [km],";
        ofs << "Y-ECI [km],";
        ofs << "Z-ECI [km],";
    }
    ofs << "Latitude [deg],";
    ofs << "Longitude [deg],";
    ofs << "Altitude [m],";
 //   ofs << "Downrange [m],";
    if (full_dump) {
//        ofs << "Mx-thrust [Nm],";
//        ofs << "My-thrust [Nm],";
//        ofs << "Mz-thrust [Nm],";
//        ofs << "Mx-aero [Nm],";
//        ofs << "My-aero [Nm],";
//        ofs << "Mz-aero [Nm],";
//        ofs << "Mx-aerodump [Nm],";
//        ofs << "My-aerodump [Nm],";
//        ofs << "Mz-aerodump [Nm],";
//        ofs << "Mx-jetdump [Nm],";
//        ofs << "My-jetdump [Nm],";
//        ofs << "Mz-jetdump [Nm],";
//        ofs << "Mx-gyroeffect [Nm],";
//        ofs << "My-gyroeffect [Nm],";
//        ofs << "Mz-gyroeffect [Nm],";
//        ofs << "AngleAccx [rad/s2],";
//        ofs << "AngleAccy [rad/s2],";
//        ofs << "AngleAccz [rad/s2],";
//        ofs << "AngleVelx [deg/s],";
//        ofs << "AngleVely [deg/s],";
//        ofs << "AngleVelz [deg/s],";
//        ofs << "q1 [-],";
//        ofs << "q2 [-],";
//        ofs << "q3 [-],";
//        ofs << "q4 [-],";
        ofs << "Azimuth [deg],";
        ofs << "Elvation [deg],";
        ofs << "Roll [deg],";
    }

    ofs << std::endl;
    Environment env;
    double g0 = env.getGravity(0);
    ofs << std::fixed;
    for (int i=0; i < countup_burn_time.size(); ++i) {
    //    double t = countup_burn_time[i];
        double alt = position[i].LLH[2];
    //    EnvironmentAir air(alt);
        env.updateAltitude(alt);
        ofs << std::setprecision(4) << countup_time[i] << ",";
        ofs << std::setprecision(4) << countup_burn_time[i] << ",";
        if (full_dump) {

            ofs << std::setprecision(8) << downrage[i]<<",";//distance_surface(position[0].LLH,position[i].LLH)<< ",";
            ofs << std::setprecision(6) << env.atmosphere.getDensity() << ",";
            ofs << std::setprecision(6) << env.atmosphere.getPressure() / 1e3 << ",";  // [kPa]
            ofs << std::setprecision(6) << env.atmosphere.getTemperature() << ",";  // [K]
            ofs << std::setprecision(6) << env.atmosphere.getSpeedOfSound() << ",";
            ofs << std::setprecision(8) << mass_prop[i] << ",";
            ofs << std::setprecision(8) << mass[i] << ",";
//            ofs << std::setprecision(2) << length_CG[i] / p_rocket->length * 100.0 << ",";  // [%]
//            ofs << std::setprecision(2) << length_CP[i] / p_rocket->length * 100.0 << ",";  // [%]
//            ofs << std::setprecision(2) << (length_CG[i] - length_CP[i]) / p_rocket->length * 100.0 << ",";  // Fst [%]
//            ofs << std::setprecision(8) << inertia_tensor[i](0, 0) << ",";
//            ofs << std::setprecision(8) << inertia_tensor[i](0, 1) << ",";
//            ofs << std::setprecision(8) << inertia_tensor[i](0, 2) << ",";
//            ofs << std::setprecision(8) << inertia_tensor[i](1, 0) << ",";
//            ofs << std::setprecision(8) << inertia_tensor[i](1, 1) << ",";
//            ofs << std::setprecision(8) << inertia_tensor[i](1, 2) << ",";
//            ofs << std::setprecision(8) << inertia_tensor[i](2, 0) << ",";
//            ofs << std::setprecision(8) << inertia_tensor[i](2, 1) << ",";
//            ofs << std::setprecision(8) << inertia_tensor[i](2, 2) << ",";
            ofs << std::setprecision(8) << thrust[i] << ",";
//            ofs << std::setprecision(8) << mdot_prop[i] << ",";
//            ofs << burning[i] << ",";
//            ofs << std::setprecision(8) << rad2deg(gimbal_angle_y_axis[i]) << ",";  // [deg]
//            ofs << std::setprecision(8) << rad2deg(gimbal_angle_z_axis[i]) << ",";  // [deg]
//            ofs << std::setprecision(4) << CA[i] << ",";
//            ofs << std::setprecision(4) << CNa[i] << ",";
//            ofs << std::setprecision(4) << Cld[i] << ",";
//            ofs << std::setprecision(4) << Clp[i] << ",";
//            ofs << std::setprecision(4) << Cmq[i] << ",";
            ofs << std::setprecision(8) << rad2deg(angle_of_attack[i]) << ",";  // [deg]
            ofs << std::setprecision(8) << rad2deg(sideslip_angle[i]) << ",";  // [deg]


            ofs << std::setprecision(8) << force[i].aero [0] << ",";
            ofs << std::setprecision(8) << force[i].aero [1] << ",";
            ofs << std::setprecision(8) << force[i].aero [2] << ",";
            ofs << std::setprecision(8) << force[i].thrust [0] << ",";
            ofs << std::setprecision(8) << force[i].thrust [1] << ",";
            ofs << std::setprecision(8) << force[i].thrust [2] << ",";
            ofs << std::setprecision(8) << force[i].gravity [0] << ",";
            ofs << std::setprecision(8) << force[i].gravity [1] << ",";
            ofs << std::setprecision(8) << force[i].gravity [2] << ",";
            ofs << std::setprecision(8) << acceleration[i].body [0] << ",";  // [m/s2]
            ofs << std::setprecision(8) << acceleration[i].body [1] << ",";  // [m/s2]
            ofs << std::setprecision(8) << acceleration[i].body [2] << ",";  // [m/s2]
            ofs << std::setprecision(8) << acceleration[i].body [0] / g0 << ",";  // [G]
            ofs << std::setprecision(8) << acceleration[i].body [1] / g0 << ",";  // [G]
            ofs << std::setprecision(8) << acceleration[i].body [2] / g0 << ",";  // [G]
            ofs << std::setprecision(8) << acceleration[i].ECI [0] << ",";
            ofs << std::setprecision(8) << acceleration[i].ECI [1] << ",";
            ofs << std::setprecision(8) << acceleration[i].ECI [2] << ",";
            ofs << std::setprecision(8) << velocity[i].air_body [0] << ",";
            ofs << std::setprecision(8) << velocity[i].air_body [1] << ",";
            ofs << std::setprecision(8) << velocity[i].air_body [2] << ",";
            ofs << std::setprecision(8) << velocity[i].mach_number << ",";
            ofs << std::setprecision(8) << dynamic_pressure[i] / 1e3 << ",";  // [kPa]
            ofs << std::setprecision(8) << velocity[i].NED [0] << ",";
            ofs << std::setprecision(8) << velocity[i].NED [1] << ",";
            ofs << std::setprecision(8) << velocity[i].NED [2] << ",";
            ofs << std::setprecision(8) << velocity[i].ECEF [0] << ",";
            ofs << std::setprecision(8) << velocity[i].ECEF [1] << ",";
            ofs << std::setprecision(8) << velocity[i].ECEF [2] << ",";
            ofs << std::setprecision(8) << velocity[i].ECI [0] << ",";
            ofs << std::setprecision(8) << velocity[i].ECI [1] << ",";
            ofs << std::setprecision(8) << velocity[i].ECI [2] << ",";
            ofs << std::setprecision(10) << position[i].ECEF [0] / 1e3 << ",";  // [km]
            ofs << std::setprecision(10) << position[i].ECEF [1] / 1e3 << ",";  // [km]
            ofs << std::setprecision(10) << position[i].ECEF [2] / 1e3 << ",";  // [km]
            ofs << std::setprecision(10) << position[i].ECI [0] / 1e3 << ",";  // [km]
            ofs << std::setprecision(10) << position[i].ECI [1] / 1e3 << ",";  // [km]
            ofs << std::setprecision(10) << position[i].ECI [2] / 1e3 << ",";  // [km]
        }
        ofs << std::setprecision(8) << position[i].LLH [0] << ",";
        ofs << std::setprecision(8) << position[i].LLH [1] << ",";
        ofs << std::setprecision(8) << position[i].LLH [2] << ",";

        if (full_dump) {
//            ofs << std::setprecision(8) << moment[i].thrust [0] << ",";
//            ofs << std::setprecision(8) << moment[i].thrust [1] << ",";
//            ofs << std::setprecision(8) << moment[i].thrust [2] << ",";
//            ofs << std::setprecision(8) << moment[i].aero_force [0] << ",";
//            ofs << std::setprecision(8) << moment[i].aero_force [1] << ",";
//            ofs << std::setprecision(8) << moment[i].aero_force [2] << ",";
//            ofs << std::setprecision(8) << moment[i].aero_dumping [0] << ",";
//            ofs << std::setprecision(8) << moment[i].aero_dumping [1] << ",";
//            ofs << std::setprecision(8) << moment[i].aero_dumping [2] << ",";
//            ofs << std::setprecision(8) << moment[i].jet_dumping [0] << ",";
//            ofs << std::setprecision(8) << moment[i].jet_dumping [1] << ",";
//            ofs << std::setprecision(8) << moment[i].jet_dumping [2] << ",";
//            ofs << std::setprecision(8) << moment[i].gyro [0] << ",";
//            ofs << std::setprecision(8) << moment[i].gyro [1] << ",";
//            ofs << std::setprecision(8) << moment[i].gyro [2] << ",";
//            ofs << std::setprecision(8) << angular_acceleration[i] [0] << ",";  // [rad/s2]
//            ofs << std::setprecision(8) << angular_acceleration[i] [1] << ",";  // [rad/s2]
//            ofs << std::setprecision(8) << angular_acceleration[i] [2] << ",";  // [rad/s2]
//            ofs << std::setprecision(8) << rad2deg(angular_velocity[i] [0]) << ",";  // [deg/s]
//            ofs << std::setprecision(8) << rad2deg(angular_velocity[i] [1]) << ",";  // [deg/s]
//            ofs << std::setprecision(8) << rad2deg(angular_velocity[i] [2]) << ",";  // [deg/s]
//            ofs << std::setprecision(8) << attitude[i].quaternion [0] << ",";
//            ofs << std::setprecision(8) << attitude[i].quaternion [1] << ",";
//            ofs << std::setprecision(8) << attitude[i].quaternion [2] << ",";
//            ofs << std::setprecision(8) << attitude[i].quaternion [3] << ",";
            ofs << std::setprecision(8) << rad2deg(attitude[i].euler_angle [0]) << ",";  // [deg]
            ofs << std::setprecision(8) << rad2deg(attitude[i].euler_angle [1]) << ",";  // [deg]
            ofs << std::setprecision(8) << rad2deg(attitude[i].euler_angle [2]) << ",";  // [deg]
        }
        ofs << std::endl;
    }
    ofs.close();
};





