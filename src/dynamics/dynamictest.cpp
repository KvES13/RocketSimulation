#include "dynamictest.h"

void DynamicTest::operator()(const state& x, state& dx, const double t) {
    Coordinate coordinate;

    // Countup Time
    p_env->masterClock.SyncSolverTime(t);
    p_rocket->burn_clock.SyncSolverTime(t);

    // Update Flight Infomation
    coordinate.setECI2ECEF(t);

    p_rocket->position.ECI  << x[0],x[1],x[2];
    p_rocket->position.ECEF = coordinate.dcm.ECI2ECEF * p_rocket->position.ECI;
    p_rocket->position.LLH = coordinate.ECEF2LLH(p_rocket->position.ECEF);

    coordinate.setECEF2NED(p_rocket->position.LLH);

    p_rocket->velocity.ECI << x[3],x[4],x[5];
    p_rocket->velocity.ECEF = coordinate.dcm.ECI2ECEF *
            (p_rocket->velocity.ECI - coordinate.dcm.EarthRotate * p_rocket->position.ECI);
    p_rocket->velocity.NED = coordinate.dcm.ECEF2NED * p_rocket->velocity.ECEF;

    double decent_velosity = p_rocket->velocity.NED(2);

    // Update Environment
    double altitude = p_rocket->position.LLH[2];
    p_env->updateAltitude(altitude);

    p_rocket->velocity.air_body = coordinate.dcm.NED2body * (p_rocket->velocity.NED);
    p_rocket->velocity.mach_number = p_rocket->velocity.air_body.norm() /
            p_env->atmosphere.getSpeedOfSound();

    Eigen::Vector3d gravity_NED(0.0, 0.0, p_env->getGravity(altitude));

    p_rocket->dynamic_pressure = 0.5 * p_env->atmosphere.getDensity() *
            std::pow(p_rocket->velocity.air_body.norm(), 2);

//    Eigen::Vector3d drag_NED(0.0, 0.0, -0.5 * p_env->atmosphere.getDensity()
//                             * decent_velosity * abs(decent_velosity)
//                             * p_rocket->CdS_parachute);
//    Eigen::Vector3d acceleration_NED = drag_NED / (p_rocket->mass.Sum()) + gravity_NED;
//    p_rocket->acceleration.ECI = coordinate.dcm.ECEF2ECI * (coordinate.dcm.NED2ECEF * acceleration_NED);


    // Calculate Force
    p_rocket->force.thrust = p_rocket->getThrust(p_env->atmosphere.getPressure());
    p_rocket->force.aero = AeroForce();
    p_rocket->force.aero[0] = -p_rocket->force.aero[0];
    p_rocket->force.aero[1] = 0.0; p_rocket->force.aero[2] = 0;
    p_rocket->force.gravity = (coordinate.dcm.NED2body * gravity_NED) *
            p_rocket->mass.Sum();

    // Calculate Acceleration
    p_rocket->acceleration.body = p_rocket->force.Sum() / p_rocket->mass.Sum();
    p_rocket->acceleration.ECI = coordinate.dcm.ECEF2ECI *
            (coordinate.dcm.NED2ECEF * (coordinate.dcm.body2NED * p_rocket->acceleration.body));


    //Eigen::Vector3d wind_NED = p_wind->getNED(altitude);
    //p_rocket->velocity.NED += wind_NED;
    p_rocket->velocity.ECI = coordinate.dcm.ECEF2ECI *
            (coordinate.dcm.NED2ECEF * p_rocket->velocity.NED)
            + coordinate.dcm.EarthRotate * p_rocket->position.ECI;

    dx[0] = p_rocket->velocity.ECI[0];  // vel_ECI => pos_ECI
    dx[1] = p_rocket->velocity.ECI[1];  //
    dx[2] = p_rocket->velocity.ECI[2];  //
    dx[3] = p_rocket->acceleration.ECI[0];  // acc_ECI => vel_ECI
    dx[4] = p_rocket->acceleration.ECI[1];  //
    dx[5] = p_rocket->acceleration.ECI[2];  //
    dx[6] = 0.0;  // quatdot => quat
    dx[7] = 0.0;  //
    dx[8] = 0.0;  //
    dx[9] = 0.0;  //
    dx[10] = 0.0;  // angular_acceleration => angular_velocity
    dx[11] = 0.0;  //
    dx[12] = 0.0;  //
    dx[13] = 0.0;  // massdot => mass_prop
};
