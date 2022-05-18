#ifndef DYNAMICS6DOFAERO_HPP_
#define DYNAMICS6DOFAERO_HPP_

#include "dynamics/dynamics_base.h"

class Dynamics6dofAero : public DynamicsBase {

public:
    Dynamics6dofAero(Rocket* rocket, Environment* env): DynamicsBase(rocket,env){};


    void operator()(const state& x, state& dx, const double t);

private:

};


#endif
