#include "environment.h"


Environment::~Environment()
{
    delete atmosphere;
}

void Environment::updateAltitude(double altitude)
{
    atmosphere->calculate(altitude);
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



