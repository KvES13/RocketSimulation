#ifndef COORDINATE_HPP_
#define COORDINATE_HPP_

#include <cmath>
#include "utils/utils.h"


class DirectionCosineMatrix {
public:
    Eigen::Matrix3d ECI2ECEF;
    Eigen::Matrix3d ECEF2NED;
    Eigen::Matrix3d NED2body;
    Eigen::Matrix3d wind2body;

    Eigen::Matrix3d body2NED;
    Eigen::Matrix3d NED2ECEF;
    Eigen::Matrix3d ECEF2ECI;
    Eigen::Matrix3d body2wind;

    Eigen::Matrix3d EarthRotate;

    DirectionCosineMatrix(){
        EarthRotate << 0.0, -Constants::w_earth, 0.0,
                        Constants::w_earth, 0.0, 0.0,
                        0.0, 0.0, 0.0;
    };
};


class Coordinate {
    public:
        Coordinate();

        DirectionCosineMatrix dcm;

        void setWind2Body(const double alpha, const double beta);

        void setNED2Body(const Eigen::Vector3d& euler_angle);
        void setNED2Body(const Eigen::Vector4d& quaternion);

        void setECI2ECEF(const double epoch_time);

        void setECEF2NED(const Eigen::Vector3d& pos_LLH);
        
        Eigen::Vector3d ECEF2LLH(const Eigen::Vector3d& pos_ECEF);
        Eigen::Vector3d LLH2ECEF(const Eigen::Vector3d& pos_LLH);

        Eigen::Vector4d Quaternion(const Eigen::Vector3d& euler_angle);
        Eigen::Vector3d EulerAngle();

        double distance_surface(const Eigen::Vector3d& pos0_LLH_,
                                const Eigen::Vector3d& pos1_LLH_);
};

#endif
