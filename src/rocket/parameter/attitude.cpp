
#include "attitude.h"

Attitude::Attitude() {
    euler_angle << 0.0, 0.0, 0.0;
    quaternion << 0.0, 0.0, 0.0, 0.0;
};


void Attitude::Initialize(const Eigen::Vector3d& euler_angle) {
    Coordinate coordinate;
    this->euler_angle = euler_angle;
    this->quaternion = coordinate.Quaternion(this->euler_angle);
};


void Attitude::Update(const Eigen::Vector4d& quaternion, Coordinate& coordinate) {
    this->quaternion = quaternion.normalized();
    this->euler_angle = coordinate.EulerAngle();
};


