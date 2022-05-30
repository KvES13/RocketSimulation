#ifndef DYNAMICS3DOFONLAUNCHER_HPP_
#define DYNAMICS3DOFONLAUNCHER_HPP_

#include "dynamics/dynamics_base.h"

class Dynamics3dofOnLauncher : public DynamicsBase {
    public:

public:
    Dynamics3dofOnLauncher(Rocket* rocket, Environment* env) :
        DynamicsBase(rocket,env){};

    void operator()(const state& x, state& dx, const double t);

    Eigen::Vector3d AeroForce(Rocket* p_rocket);

private:


};

#endif
