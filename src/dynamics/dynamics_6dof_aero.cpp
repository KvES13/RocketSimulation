#include "dynamics_6dof_aero.h"


void Dynamics6dofAero::operator()(const state& x, state& dx, const double t)
{
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

    p_rocket->attitude.quaternion = Eigen::Vector4d{x[6],x[7],x[8],x[9]}.normalized();

    coordinate.setNED2Body(p_rocket->attitude.quaternion);

    p_rocket->attitude.euler_angle = coordinate.EulerAngle();

    p_rocket->angular_velocity = Eigen::Map<Eigen::Vector3d>(std::vector<double>(x.begin()+10, x.begin()+13).data());

    p_rocket->mass.propellant = x[13];



    // Update Environment
    double altitude = p_rocket->position.LLH[2];
    p_env->updateAltitude(altitude);

   Eigen::Vector3d gravity_NED(0.0, 0.0, p_env->getGravity(altitude));

    // Update Airspeed
    p_rocket->velocity.air_body = coordinate.dcm.NED2body * (p_rocket->velocity.NED /*- p_wind->getNED(altitude)*/);
    p_rocket->dynamic_pressure = 0.5 * p_env->atmosphere.getDensity() *
            std::pow(p_rocket->velocity.air_body.norm(), 2);
    p_rocket->velocity.mach_number = p_rocket->velocity.air_body.norm() /
            p_env->atmosphere.getSpeedOfSound();

    // Update time and mach parameter
    p_rocket->inertia_tensor = p_rocket->getInertiaTensor();

    p_rocket->length_CG = p_rocket->getLengthCG();    
    p_rocket->length_CP = p_rocket->getLengthCP(p_rocket->velocity.mach_number);
    p_rocket->CA = p_rocket->getCA(p_rocket->velocity.mach_number);
    p_rocket->CNa = p_rocket->getCNa(p_rocket->velocity.mach_number);
    p_rocket->Cld = p_rocket->getCld(p_rocket->velocity.mach_number);
    p_rocket->Clp = p_rocket->getClp(p_rocket->velocity.mach_number);
    p_rocket->Cmq = p_rocket->getCmq(p_rocket->velocity.mach_number);
    p_rocket->Cnr = p_rocket->getCnr(p_rocket->velocity.mach_number);

    // Calculate AoA
    // p_rocket->angle_of_attack = std::atan2(p_rocket->velocity.air_body[2], p_rocket->velocity.air_body[0]);
    if (p_rocket->velocity.air_body.norm() <= 0.0) {
        p_rocket->angle_of_attack = 0.0;
        p_rocket->sideslip_angle = 0.0;
    } else {
        p_rocket->angle_of_attack = std::asin(p_rocket->velocity.air_body[2] /
                p_rocket->velocity.air_body.norm());
        p_rocket->sideslip_angle = std::asin(-p_rocket->velocity.air_body[1] /
                p_rocket->velocity.air_body.norm());
    }


    // Calculate Force
    p_rocket->force.thrust = p_rocket->getThrust(p_env->atmosphere.getPressure());
    p_rocket->force.aero = AeroForce();
    p_rocket->force.gravity = (coordinate.dcm.NED2body * gravity_NED) *
            p_rocket->mass.Sum();

   // qDebug()<<" "<<p_rocket->mass.Sum();

    // Calculate Acceleration
    p_rocket->acceleration.body = p_rocket->force.Sum() / p_rocket->mass.Sum();
    p_rocket->acceleration.ECI = coordinate.dcm.ECEF2ECI *
            (coordinate.dcm.NED2ECEF * (coordinate.dcm.body2NED * p_rocket->acceleration.body));

    // Calculate Moment
//    p_rocket->moment.gyro = GyroEffectMoment(p_rocket);
//    p_rocket->moment.thrust = ThrustMoment(p_rocket);
//    p_rocket->moment.aero_force = AeroForceMoment(p_rocket);
//    p_rocket->moment.aero_dumping = AeroDampingMoment(p_rocket);
//    p_rocket->moment.jet_dumping = JetDampingMoment(p_rocket);
    
//    // Calculate Angle Velocity
//    p_rocket->angular_acceleration = p_rocket->inertia_tensor.inverse() * p_rocket->moment.Sum();

//    // Calculate Quaternion
//    p_rocket->quaternion_dot =  0.5 *(QuaternionDiff(p_rocket) * p_rocket->attitude.quaternion) ;


 //   p_rocket->mass.inert -= p_rocket->engine.mdot_prop;

 //   qDebug()<<p_rocket->mass.inert<<" "<<p_rocket->engine.mdot_prop;



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
    dx[13] = -p_rocket->engine.mdot_prop;  // massdot => mass_prop
//    dx[6] = p_rocket->quaternion_dot[0];  // quatdot => quat
//    dx[7] = p_rocket->quaternion_dot[1];  //
//    dx[8] = p_rocket->quaternion_dot[2];  //
//    dx[9] = p_rocket->quaternion_dot[3];  //
//    dx[10] = p_rocket->angular_acceleration[0];  // angular_acceleration => angular_velocity
//    dx[11] = p_rocket->angular_acceleration[1];  //
//    dx[12] = p_rocket->angular_acceleration[2];  //
//    dx[13] = -p_rocket->engine.mdot_prop;  // massdot => mass_prop

};

