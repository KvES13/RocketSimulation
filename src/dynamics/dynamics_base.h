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
        // DynamicsBase() {};
    virtual ~DynamicsBase(){};

        using state = std::array<double,14>;

        virtual void operator()(const state& x, state& dx, const double t) = 0;

        Eigen::Vector3d AeroForce(Rocket* p_rocket);
        Eigen::Vector3d GyroEffectMoment(Rocket* p_rocket);
        Eigen::Vector3d ThrustMoment(Rocket* p_rocket);
        Eigen::Vector3d AeroForceMoment(Rocket* p_rocket);
        Eigen::Vector3d AeroDampingMoment(Rocket* p_rocket);
        Eigen::Vector3d JetDampingMoment(Rocket* p_rocket);
        
        Eigen::Matrix4d QuaternionDiff(Rocket* p_rocket);

    private:
};


std::pair<double, Eigen::Vector3d> IIP(Eigen::Vector3d& pos_ECI, Eigen::Vector3d& vel_ECI);


#endif

