
#include "coordinate.h"

Coordinate::Coordinate() {
    dcm = DirectionCosineMatrix();
};

void Coordinate::setWind2Body(const double alpha, const double beta) {

    dcm.wind2body << std::cos(alpha) * std::cos(beta), std::cos(alpha) * std::sin(beta), -std::sin(alpha),
                    -std::sin(beta)                  , std::cos(beta)                  ,              0.0,
                    std::sin(alpha) * std::cos(beta) , std::sin(alpha) * std::sin(beta), std::cos(alpha);
    dcm.body2wind = dcm.wind2body.transpose();
};


void Coordinate::setNED2Body(const Eigen::Vector3d &euler_angle) {

    double azi = euler_angle(0);
    double elv = euler_angle(1);
    double rol = euler_angle(2);
    dcm.NED2body << cos(azi) * cos(elv)                                  ,  sin(azi) * cos(elv)                                 , -sin(elv),
                    -sin(azi) * cos(rol) + cos(azi) * sin(elv) * sin(rol),  cos(azi) * cos(rol) + sin(azi) * sin(elv) * sin(rol),  cos(elv) * sin(rol),
                    sin(azi) * sin(rol) + cos(azi) * sin(elv) * cos(rol) , -cos(azi) * sin(rol) + sin(azi) * sin(elv) * cos(rol),  cos(elv) * cos(rol);
    dcm.body2NED = dcm.NED2body.transpose();
};

void Coordinate::setNED2Body(const Eigen::Vector4d &quat) {
    double q0 = quat(0);
    double q1 = quat(1);
    double q2 = quat(2);
    double q3 = quat(3);
    dcm.NED2body << q0*q0 - q1*q1 - q2*q2 + q3*q3, 2.0 * (q0 * q1 + q2 * q3)    , 2.0 * (q0 * q2 - q1 * q3),
                    2.0 * (q0 * q1 - q2 * q3)    , q1*q1 - q0*q0 - q2*q2 + q3*q3, 2.0 * (q1 * q2 + q0 * q3),
                    2.0 * (q0 * q2 + q1 * q3)    , 2.0 * (q1 * q2 - q0 * q3)    , q2*q2 - q0*q0 - q1*q1 + q3*q3;
    dcm.body2NED = dcm.NED2body.transpose();
};


void Coordinate::setECI2ECEF(const double epoch_time) {
    double xi = Constants::w_earth * epoch_time;
    dcm.ECI2ECEF << std::cos(xi) , std::sin(xi), 0.0,
                    -std::sin(xi), std::cos(xi), 0.0,
                    0.0          ,          0.0, 1.0;
    dcm.ECEF2ECI = dcm.ECI2ECEF.transpose();
};


void Coordinate::setECEF2NED(const Eigen::Vector3d &pos_LLH) {

    double lat = deg2rad(pos_LLH(0));
    double lon = deg2rad(pos_LLH(1));

    dcm.ECEF2NED << -std::sin(lat) * std::cos(lon), -std::sin(lat) * std::sin(lon), std::cos(lat),
                    -std::sin(lon)                 ,                  std::cos(lon),             0,
                    -std::cos(lat) * std::cos(lon) , -std::cos(lat) * std::sin(lon), -std::sin(lat);
    dcm.NED2ECEF = dcm.ECEF2NED.transpose();
};


Eigen::Vector3d Coordinate::ECEF2LLH(const Eigen::Vector3d &pos_ECEF) {

    double x = pos_ECEF[0];
    double y = pos_ECEF[1];
    double z = pos_ECEF[2];
    double lat, lon, height;
    double p = std::sqrt(x * x + y * y);
    double theta = std::atan2(z *  Constants::a_earth, (p * Constants::b));
    double e_dash_square = ( Constants::a_earth *  Constants::a_earth - Constants::b * Constants::b) / std::pow(Constants::b, 2);
    lat = std::atan2(z + e_dash_square * Constants::b * std::pow(std::sin(theta), 3), p - Constants::e12 * Constants::a_earth * std::pow(std::cos(theta), 3));
    lon = std::atan2(y, x);
    double N = Constants::a_earth / std::sqrt(1 -Constants::e12 * std::pow(std::sin(lat), 2));
    height = (p / std::cos(lat) - N);
    return Eigen::Vector3d(rad2deg(lat), rad2deg(lon), height);
};

Eigen::Vector3d Coordinate::LLH2ECEF(const Eigen::Vector3d &pos_LLH) {

    double lat = deg2rad(pos_LLH[0]);
    double lon = deg2rad(pos_LLH[1]);
    double height = pos_LLH[2];
    double x, y ,z;
    double N = Constants::a_earth / std::sqrt(1 - Constants::e12 * std::pow(std::sin(lat), 2));
    x = (N + height) * std::cos(lat) * std::cos(lon);
    y = (N + height) * std::cos(lat) * std::sin(lon);
    z = (N * (1 - Constants::e12) + height) * std::sin(lat); //
    return Eigen::Vector3d(x, y, z);
};


Eigen::Vector4d Coordinate::Quaternion(const Eigen::Vector3d &euler_angle) {

    setNED2Body(euler_angle);
    Eigen::Vector4d q;
    q(0) = 0.5 * std::sqrt(1.0 + dcm.NED2body(0, 0) - dcm.NED2body(1, 1) - dcm.NED2body(2, 2));
    q(1) = 0.5 * std::sqrt(1.0 - dcm.NED2body(0, 0) + dcm.NED2body(1, 1) - dcm.NED2body(2, 2));
    q(2) = 0.5 * std::sqrt(1.0 - dcm.NED2body(0, 0) - dcm.NED2body(1, 1) + dcm.NED2body(2, 2));
    q(3) = 0.5 * std::sqrt(1.0 + dcm.NED2body(0, 0) + dcm.NED2body(1, 1) + dcm.NED2body(2, 2));

    Eigen::VectorXd::Index index_quat_max;
    /*double quat_max =*/q.maxCoeff(&index_quat_max);
    switch(index_quat_max) {
        case 0:
            q(0) = 0.5 * sqrt(1.0 + dcm.NED2body(0, 0) - dcm.NED2body(1,1) - dcm.NED2body(2,2));
            q(1) = (dcm.NED2body(0, 1) + dcm.NED2body(1, 0)) / (4.0 * q(0));
            q(2) = (dcm.NED2body(2, 0) + dcm.NED2body(0, 2)) / (4.0 * q(0));
            q(3) = (dcm.NED2body(1, 2) - dcm.NED2body(2, 1)) / (4.0 * q(0));
            break;
        case 1:
            q(1) = 0.5 * sqrt(1.0 - dcm.NED2body(0, 0) + dcm.NED2body(1,1) - dcm.NED2body(2,2));
            q(0) = (dcm.NED2body(0, 1) + dcm.NED2body(1, 0)) / (4.0 * q(1));
            q(2) = (dcm.NED2body(1, 2) + dcm.NED2body(2, 1)) / (4.0 * q(1));
            q(3) = (dcm.NED2body(2, 0) - dcm.NED2body(0, 2)) / (4.0 * q(1));
            break;
        case 2:
            q(2) = 0.5 * sqrt(1.0 - dcm.NED2body(0, 0) - dcm.NED2body(1,1) + dcm.NED2body(2,2));
            q(0) = (dcm.NED2body(2, 0) + dcm.NED2body(0, 2)) / (4.0 * q(2));
            q(1) = (dcm.NED2body(1, 2) + dcm.NED2body(2, 1)) / (4.0 * q(2));
            q(3) = (dcm.NED2body(0, 1) - dcm.NED2body(1, 0)) / (4.0 * q(2));
            break;
        case 3:
            q(3) = 0.5 * sqrt(1.0 + dcm.NED2body(0, 0) + dcm.NED2body(1,1) + dcm.NED2body(2,2));
            q(0) = (dcm.NED2body(1, 2) - dcm.NED2body(2, 1)) / (4.0 * q(3));
            q(1) = (dcm.NED2body(2, 0) - dcm.NED2body(0, 2)) / (4.0 * q(3));
            q(2) = (dcm.NED2body(0, 1) - dcm.NED2body(1, 0)) / (4.0 * q(3));
            break;
        default:
            break;
    }
    q.normalize();
    return q;
};


Eigen::Vector3d Coordinate::EulerAngle() {

    double azimuth = std::atan2(dcm.NED2body(0, 1), dcm.NED2body(0, 0));
    double elevation = -std::asin(dcm.NED2body(0, 2));
    double roll = std::atan2(dcm.NED2body(1, 2), dcm.NED2body(2, 2));
    Eigen::Vector3d euler;
    euler << azimuth, elevation, roll;
    return euler;
};



double Coordinate::distance_surface(const Eigen::Vector3d& pos0_LLH_,
                        const Eigen::Vector3d& pos1_LLH_){
    double const earth_radius = 6378137;

    Eigen::Vector3d pos0_ECEF_ = LLH2ECEF(pos0_LLH_);
    Eigen::Vector3d pos1_ECEF_ = LLH2ECEF(pos1_LLH_);
    double theta = acos(pos0_ECEF_.dot(pos1_ECEF_) /
                        pos0_ECEF_.norm() / pos1_ECEF_.norm());
    return earth_radius * theta;
}













