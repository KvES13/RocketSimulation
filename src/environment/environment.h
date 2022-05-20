#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "atmosphere.h"
#include "sequence_clock.h"
#include "Constants.h"

class Environment
{
public:

    explicit Environment(){ /*atmosphere = new Atmosphere(0);*/};
 //   explicit Environment (Atmosphere* atmosphere) : atmosphere(atmosphere){};
  //  Environment (const Environment& env);

    ~Environment();

    Atmosphere /***/atmosphere;
    void updateAltitude(double altitude);
    SequenceClock masterClock;

    double getGravity(const double altitude);

private:

};


#endif // ENVIRONMENT_H
