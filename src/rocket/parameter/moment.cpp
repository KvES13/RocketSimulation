#include "moment.h"


Moment::Moment() {
    gyro << 0.0, 0.0, 0.0;
    thrust << 0.0, 0.0, 0.0;
    aero_force << 0.0, 0.0, 0.0;
    aero_dumping << 0.0, 0.0, 0.0;
    jet_dumping << 0.0, 0.0, 0.0;
};

Eigen::Vector3d Moment::Sum() {
    Eigen::Vector3d sum;
    sum = gyro + thrust + aero_force + aero_dumping + jet_dumping;
    return sum;
};
