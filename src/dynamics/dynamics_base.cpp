#include "dynamics_base.h"


//#include "Vector3.h"


Eigen::Vector3d DynamicsBase::AeroForce() {

    Eigen::Vector3d force_aero;

    double force_axial = p_rocket->dynamic_pressure * p_rocket->CA * p_rocket->area;
    double force_normal = p_rocket->dynamic_pressure * p_rocket->CNa * p_rocket->area;
    double force_normal_y_axis = force_normal * p_rocket->sideslip_angle;
    double force_normal_z_axis = force_normal * p_rocket->angle_of_attack;
    force_aero << -force_axial, force_normal_y_axis, -force_normal_z_axis;

    return force_aero;
};


Eigen::Vector3d DynamicsBase::GyroEffectMoment() {

    Eigen::Vector3d gyro_effect;

    Eigen::Vector3d angular_momentum = p_rocket->getInertiaTensor() * p_rocket->angular_velocity;
    gyro_effect = p_rocket->angular_velocity.cross(angular_momentum);

    return -1.0 * gyro_effect;
};


Eigen::Vector3d DynamicsBase::ThrustMoment() {
    Eigen::Vector3d moment_thrust;

    Eigen::Vector3d moment_arm(p_rocket->length_CG - p_rocket->length_thrust, 0.0, 0.0);
    moment_thrust = p_rocket->force.thrust.cross(moment_arm);

    return moment_thrust;
};


Eigen::Vector3d DynamicsBase::AeroForceMoment() {
    Eigen::Vector3d moment_aero;

    Eigen::Vector3d moment_arm(p_rocket->length_CG - p_rocket->length_CP, 0.0, 0.0);
    moment_aero = p_rocket->force.aero.cross(moment_arm);
    moment_aero[0] = p_rocket->dynamic_pressure * p_rocket->Cld * p_rocket->area * p_rocket->length * p_rocket->cant_angle_fin * 4;

    return moment_aero;
};


Eigen::Vector3d DynamicsBase::AeroDampingMoment() {
    Eigen::Vector3d moment_aero_dumping;

    Eigen::Vector3d coefficient_aero_dumping(p_rocket->Clp, p_rocket->Cmq, p_rocket->Cnr);

    moment_aero_dumping = p_rocket->dynamic_pressure * coefficient_aero_dumping.array() * p_rocket->area * std::pow(p_rocket->length, 2)
                            / (2.0 * p_rocket->velocity.air_body.norm()) * p_rocket->angular_velocity.array();

    return moment_aero_dumping;
};


Eigen::Vector3d DynamicsBase::JetDampingMoment() {
    Eigen::Vector3d moment_jet_dumping;

    moment_jet_dumping << 0.0, 0.0, 0.0;

    return moment_jet_dumping;
};


Eigen::Matrix4d  DynamicsBase::QuaternionDiff() {
    double p = p_rocket->angular_velocity[0];
    double q = p_rocket->angular_velocity[1];
    double r = p_rocket->angular_velocity[2];
    Eigen::Matrix4d quat_dot;
    quat_dot << 0, r, -q, p,
                -r, 0, p, q,
                q, -p, 0, r,
                -p, -q, -r, 0;
    return quat_dot;
}

void DynamicsBase::reset(Rocket *rocket, Environment *env)
{
    p_rocket = rocket;
    p_env = env;
};


std::pair<double, Eigen::Vector3d> IIP(Eigen::Vector3d& pos_ECI, Eigen::Vector3d& vel_ECI)
{


    Eigen::Vector3d ir0;  // initial posint unit vector
    Eigen::Vector3d iv0;  // initial velocity unit vector
    double r0;  // initial position norm
    double v0;  // initial velocity norm
    double gamma0;  // inertia flight path angle

    Eigen::Vector3d ip;
    double rp = 0;  // 地球中心とIIP間距離
    double rp_init;  // 収束計算用

    double vc;  // circular orbital velocity
    double lambda;  // squared speed ratio of current to circular orbital

    double c[3];
    double c_squr[3];

    double sin_phi;
    double cos_phi;
    double phi;

    double k[2];

    double lat_IIP_ECI;
    double lon_IIP_ECI;

    double lat_IIP_ECEF;
    double lon_IIP_ECEF;

    double tf1, tf1h, tf1l1, tf1l2;
    double tf2, tf21, tf2h, tf2l;
    double tf;

    Eigen::Vector3d IIP_LLH;

    // =================
    r0 = pos_ECI.norm();
    v0 = vel_ECI.norm();
    ir0 = pos_ECI.normalized();
    iv0 = vel_ECI.normalized(); /// @todo

    vc = std::sqrt(Constants::GM / r0);
    gamma0 = std::asin(ir0.dot(iv0));  // [rad]
    lambda = std::pow(v0 / vc, 2);

    const double eps = 1e-6;
    const double relaxation = 0.1;
    rp_init = (Constants::a_earth + Constants::b) * 0.5;
    // rp_init = wgs84.b;
    unsigned int counter = 0;
    while (std::abs(rp - rp_init) > eps) {
        rp = rp_init;

        c[0] = -std::tan(gamma0);
        c[1] = 1.0 - 1.0 / (lambda * std::pow(std::cos(gamma0), 2));
        c[2] = r0 / rp - 1.0 / (lambda * std::pow(std::cos(gamma0), 2));  // rp
        for (int i=0; i < 3; i++) {
            c_squr[i] = c[i] * c[i];
        }

        sin_phi = (c[0] * c[2]
                + std::sqrt(c_squr[0] * c_squr[2] - (c_squr[0] + c_squr[1]) * (c_squr[2] - c_squr[1])))
                / (c_squr[0] + c_squr[1]);
        cos_phi = std::sqrt(1.0 - sin_phi);
        phi = std::asin(sin_phi);  // [rad]

        k[0] = std::cos(gamma0 + phi) / std::cos(gamma0);
        k[1] = sin_phi / std::cos(gamma0);

        ip = ir0 * k[0] + iv0 * k[1];

        lat_IIP_ECI = std::asin(ip[2]);
        lon_IIP_ECI = std::atan2(ip[1], ip[0]);

        tf1h = std::tan(gamma0) * (1.0 - cos_phi) + (1.0 - lambda) * sin_phi;
        tf1l1 = (1.0 - cos_phi) / (lambda * std::pow(std::cos(gamma0), 2));
        tf1l2 = std::cos(gamma0 + phi) / std::cos(gamma0);
        tf1 = tf1h / ((2.0 - lambda) * (tf1l1 + tf1l2));

        tf21 = 2.0 * std::cos(gamma0) / (lambda * std::pow(2.0 / lambda - 1.0, 1.5));
        tf2h = std::sqrt(2.0 / lambda - 1.0);
        tf2l = std::cos(gamma0) * (1.0 / std::tan(phi * 0.5)) - std::sin(gamma0);
        tf2 = tf21 * std::atan2(tf2h , tf2l);

        tf = r0 / (v0 * std::cos(gamma0)) * (tf1 + tf2);

        lat_IIP_ECEF = lat_IIP_ECI;
        lon_IIP_ECEF = lon_IIP_ECI - Constants::w_earth * tf;

        rp = Constants::a_earth * std::sqrt(1.0 - std::pow(Constants::e12 * std::sin(lat_IIP_ECI), 2));

        counter++;
        double delta = rp - rp_init;
        if (rp > rp_init) {
            rp_init = rp_init + delta * relaxation;
        }
        else {
            rp_init = rp_init - delta * relaxation;
        }

    }
        c[2] = r0 / rp - 1.0 / (lambda * std::pow(std::cos(gamma0), 2));  // rp

    IIP_LLH[0] = rad2deg(lat_IIP_ECEF);
    IIP_LLH[1] = rad2deg(lon_IIP_ECEF);
    IIP_LLH[2] = 0.0;

    return std::make_pair(tf, IIP_LLH);
};
