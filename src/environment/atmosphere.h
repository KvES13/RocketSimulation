#ifndef ATMOSPHERE_H
#define ATMOSPHERE_H

#include "Constants.h"
#include <array>

class Atmosphere
{
public:
    Atmosphere(double currentAltitude = 0);

    void setAltitude(double currentAltitude);
    double getTemperature() const {return temperature;};
    double getPressure() const {return pressure;};
    double getDensity() const {return density;};
    double getSpeedOfSound() const {return  speedOfSound;};

private:
    void calcAtmosphere();

    double temperature;
    double pressure;
    double density;
    double speedOfSound ;

    double altitude;

    using Layer = std::array<double, 8>;
    // [km] Уровни высот
    Layer height_array = {0.0, 11000, 20000, 32000, 47000, 51000, 71000, 84852};
    // [K] Температура на высоте
    Layer temperature_array = {288.15, 216.65, 216.65, 228.65, 270.65, 270.65, 214.65, 186.946};

    // [K/km] Температурный градиент
    Layer temperature_gradient_array = {-0.0065, 0.0, 0.0010, 0.0028, 0.0, -0.0028, -0.0020, 0.0};

    Layer pressure_array = {101325.0, 22632.0, 5474.87, 868.0146, 110.9056, 66.939, 3.9564, 0.3734};  // [Pa]



    static constexpr double kR = 287.05287;
    static constexpr double kTemperatureSeaLevel = 288.15;
    static constexpr double kSpeedOfSoundSeaLevel = 340.294;

};

#endif // ATMOSPHERE_H
