#include "force.h"


Force::Force() {
    thrust << 0.0, 0.0, 0.0;
    aero << 0.0, 0.0, 0.0;
    gravity << 0.0, 0.0, 0.0;
};

Eigen::Vector3d Force::Sum() {
    Eigen::Vector3d sum;
    sum = thrust + aero + gravity;
    return sum;
};

