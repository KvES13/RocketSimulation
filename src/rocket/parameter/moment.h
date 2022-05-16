#ifndef MOMENT_H_
#define MOMENT_H_

#define EIGEN_MPL2_ONLY
#include "Eigen/Core"

class Moment {

public:
    Eigen::Vector3d gyro;
    Eigen::Vector3d thrust;
    Eigen::Vector3d aero_force;
    Eigen::Vector3d aero_dumping;
    Eigen::Vector3d jet_dumping;

    Moment();
    Eigen::Vector3d Sum();
};


#endif
