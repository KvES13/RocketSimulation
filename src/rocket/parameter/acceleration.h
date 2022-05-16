#ifndef ACCELERATION_H_
#define ACCELERATION_H_

#define EIGEN_MPL2_ONLY
#include "Eigen/Core"

class Acceleration {
    public:
    Eigen::Vector3d ECI;
    Eigen::Vector3d body;

        Acceleration();
};

#endif
