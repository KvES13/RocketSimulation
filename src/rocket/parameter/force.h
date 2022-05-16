#ifndef FORCE_HPP_
#define FORCE_HPP_

#define EIGEN_MPL2_ONLY
#include "Eigen/Core"

class Force {
    public:
    Eigen::Vector3d thrust;
    Eigen::Vector3d aero;
    Eigen::Vector3d gravity;

    Force();
    Eigen::Vector3d Sum();

};


#endif
