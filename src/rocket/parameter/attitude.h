#ifndef ATTITUDE_H_
#define ATTITUDE_H_

#include "environment/coordinate.h"

class Attitude {

public:
    Eigen::Vector3d euler_angle;
    Eigen::Vector4d quaternion;

    Attitude();

    void Initialize(const Eigen::Vector3d& euler_angle);
    void Update(const Eigen::Vector4d& quaternion, Coordinate& coordinate);

private:

};

#endif
