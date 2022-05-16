#include "atmosphere.h"


//Конструктор класса Атмосфера
Atmosphere::Atmosphere(double currentAltitude)
{
    setAltitude(currentAltitude);
}

void Atmosphere::setAltitude(double currentAltitude)
{
    // Если текущая высота больше 86000 -> расчитывать по высоте 86000
    // Применяются формулы для низких высот
    if(currentAltitude > 860000)
    {
        currentAltitude = 86000.0;
    }
    altitude = currentAltitude;
    // Расчёт параметров атмосферы
    calcAtmosphere();
}

static double geometricToGeopotencial(double alt)
{
    return alt *6356766/ (6356766+alt);
}


void Atmosphere::calcAtmosphere()
{
    double geopot_H = geometricToGeopotencial(altitude);
    if(geopot_H <  0.00 || geopot_H > height_array[7])
        geopot_H = 0;

    size_t idx = 0;
    for (idx = 0; idx < height_array.size() - 1; idx++) {
            if ((geopot_H >= height_array[idx]) && (geopot_H < height_array[idx + 1]))
                break;
        }

    float Ps = pressure_array[idx];
    float Bm = temperature_gradient_array[idx];
    float Tm = temperature_array[idx];
    float Hb = height_array[idx];
    float lP = 0;

    if (Bm != 0.0) {
            lP = log10(Ps) - (Constants::g_earth / (Bm * kR)) * log10((Tm + Bm * (geopot_H - Hb)) / Tm);
        } else {
            lP = log10(Ps) - 0.434294 *(Constants::g_earth * (geopot_H - Hb)) / (kR * Tm);
        }

     pressure = pow(10, lP);


     temperature = Tm;
     if (Bm != 0.0){
         temperature += Bm * (geopot_H - Hb);
     }
     density = pressure / (kR * temperature);
      speedOfSound = sqrt((temperature / kTemperatureSeaLevel)) * kSpeedOfSoundSeaLevel;
}











