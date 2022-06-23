#ifndef DYNAMICSBASE_H_
#define DYNAMICSBASE_H_


#include <array>
#include <cmath>

#define EIGEN_MPL2_ONLY
#include "Eigen/Core"
#include "Eigen/Dense"

#include "Constants.h"
#include "rocket/rocket.h"
#include "environment.h"
#include "environment/coordinate.h"


class DynamicsBase {
public:
    DynamicsBase(Rocket* rocket, Environment* env) :p_rocket(rocket), p_env(env) {};
    DynamicsBase() = default;
    virtual ~DynamicsBase(){};

    using state = std::array<double,14>;

    virtual void operator()(const state& x, state& dx, const double t) = 0;

    Eigen::Vector3d AeroForce();
    Eigen::Vector3d GyroEffectMoment();
    Eigen::Vector3d ThrustMoment();
    Eigen::Vector3d AeroForceMoment();
    Eigen::Vector3d AeroDampingMoment();
    Eigen::Vector3d JetDampingMoment();

    Eigen::Matrix4d QuaternionDiff();

    void reset(Rocket* rocket, Environment* env);

protected:
    Rocket* p_rocket = nullptr;
    Environment* p_env = nullptr;
};


#endif

