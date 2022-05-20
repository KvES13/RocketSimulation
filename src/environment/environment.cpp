#include "environment.h"


//Environment::Environment(const Environment &env)
//{
//    if(atmosphere != nullptr)
//        delete atmosphere;
//    masterClock = env.masterClock;
//    atmosphere = new Atmosphere(0);
//    atmosphere = env.atmosphere;
//}

Environment::~Environment()
{
 //   delete atmosphere;
}

void Environment::updateAltitude(double altitude)
{
    atmosphere.calculate(altitude);
}

double Environment::getGravity(const double altitude)
{
    double geocentric_height;
    if (altitude < 0.0)
        geocentric_height = Constants::a_earth;
    else
        geocentric_height = altitude + Constants::a_earth;
    return Constants::GM / std::pow(geocentric_height, 2);
}



