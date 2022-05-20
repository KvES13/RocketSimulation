#ifndef ATMOSPHERE_H
#define ATMOSPHERE_H

#include "Constants.h"
#include <array>

class Atmosphere
{
public:

    Atmosphere(double currentAltitude = 0);

    void calculate(double currentAltitude);
    double getTemperature() const {return temperature;};
    double getPressure() const {return pressure;};
    double getDensity() const {return density;};
    double getSpeedOfSound() const {return  speedOfSound;};

private:
    void calcLowerAtmosphere();
    void calcUpperAtmosphere();

    double temperature;
    double pressure;
    double density;
    double speedOfSound ;

    double altitude;

    static constexpr double kR = 287.05287;
    static constexpr double kTemperatureSeaLevel = 288.15;
    static constexpr double kSpeedOfSoundSeaLevel = 340.294;
    static constexpr double kDensitySeaLevel = 1.225;
    static constexpr double kPressureSeaLevel = 101325.0;


    double EvaluateCubic(double a, double fa, double fpa, double b, double fb, double fpb, double u);
    double KineticTemperature(double geometric_altitude);

};

#endif // ATMOSPHERE_H
