#ifndef DYNAMICS3DOFPARACHUTE_HPP_
#define DYNAMICS3DOFPARACHUTE_HPP_

#include "dynamics/dynamics_base.h"

class Dynamics3dofParachute : public DynamicsBase {
public:
    Dynamics3dofParachute(Rocket* rocket, Environment* env);

    void operator()(const state& x, state& dx, const double t);

private:
    Rocket* p_rocket;
    Environment* p_env;

};

#endif
