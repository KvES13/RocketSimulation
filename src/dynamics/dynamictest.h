#ifndef DYNAMICTEST_H
#define DYNAMICTEST_H

#include "dynamics/dynamics_base.h"

class DynamicTest : public DynamicsBase
{
public:
    DynamicTest(Rocket* rocket, Environment* env) : DynamicsBase(rocket,env){};

    void operator()(const state& x, state& dx, const double t);

private:

};

#endif // DYNAMICTEST_H
