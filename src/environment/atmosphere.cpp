#include "atmosphere.h"


//Конструктор класса Атмосфера
Atmosphere::Atmosphere(double currentAltitude)
{
    calculate(currentAltitude);
}

void Atmosphere::calculate(double currentAltitude)
{
    // Если текущая высота больше 86000
    // Применяются формулы для низких высот
    // ГОСТ Стандартная атмосфера
    altitude = currentAltitude;
    if(altitude < 86000.0)
    {
        // Высота измеряется в метрах
        calcLowerAtmosphere();
    }
    else
    {
        // Высота измеряется в километрах
        calcUpperAtmosphere();
    }

    speedOfSound = sqrt((temperature / kTemperatureSeaLevel)) * kSpeedOfSoundSeaLevel;

}

static double geometricToGeopotencial(double alt)
{
    return alt *6356766/ (6356766+alt);
}


void Atmosphere::calcLowerAtmosphere()
{
    using Layer = std::array<double, 8>;
    // [km] Уровни высот
    Layer height_array = {0.0, 11000, 20000, 32000, 47000, 51000, 71000, 86000};
    // [K] Температура на высоте
    Layer temperature_array = {288.15, 216.65, 216.65, 228.65, 270.65, 270.65, 214.65, 186.946};

    // [K/km] Температурный градиент
    Layer temperature_gradient_array = {-0.0065, 0.0, 0.0010, 0.0028, 0.0, -0.0028, -0.0020, 0.0};

    Layer pressure_array = {101325.0, 22632.0, 5474.87, 868.0146, 110.9056, 66.939, 3.9564, 0.3734};  // [Pa]

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
}

void Atmosphere::calcUpperAtmosphere()
{
    using Layer = std::array<double, 25>;
    Layer height_array = {86.0, 93.0, 100.0, 107.0, 114.0, 121.0, 128.0, 135.0, 142.0, 150.0,
                            160.0, 170.0, 180.0, 190.0, 200.0, 220.0, 260.0, 300.0, 400.0,
                            500.0, 600.0, 700.0, 800.0, 900.0, 1000.0};
    Layer pressure_ratio_array = {3.6850e-6, 1.0660e-6, 3.1593e-7, 1.0611e-7, 4.3892e-8,
                                        2.3095e-8, 1.3997e-8, 9.2345e-9, 6.4440e-9, 4.4828e-9,
                                        2.9997e-9, 2.0933e-9, 1.5072e-9, 1.1118e-9, 8.3628e-10,
                                        4.9494e-10, 1.9634e-10, 8.6557e-11, 1.4328e-11, 2.9840e-12,
                                        8.1056e-13, 3.1491e-13, 1.6813e-13, 1.0731e-13, 7.4155e-14};
    Layer density_ratio_array = {5.680E-6, 1.632E-6, 4.575E-7, 1.341E-7, 4.061E-8,
                                    1.614e-8, 7.932e-9, 4.461e-9, 2.741e-9, 1.694e-9,
                                    1.007e-9, 6.380e-10, 4.240e-10, 2.923e-10, 2.074e-10,
                                    1.116e-10, 3.871e-11, 1.564e-11, 2.288e-12, 4.257e-13,
                                    9.279e-14, 2.506e-14, 9.272e-15, 4.701e-15, 2.907e-15};

    Layer log_pressure_ratio_array;
    Layer log_density_ratio_array;
    for (int i=0; i < 25; ++i) {
        log_pressure_ratio_array[i] = std::log(pressure_ratio_array[i]);
        log_density_ratio_array[i] = std::log(density_ratio_array[i]);
    }

    Layer delta_log_pressure_ratio_array = {-0.174061, -0.177924, -0.167029, -0.142755, -0.107859,
                                            -0.079322, -0.064664, -0.054879, -0.048260, -0.042767,
                                            -0.037854, -0.034270, -0.031543, -0.029384, -0.027632,
                                            -0.024980, -0.021559, -0.019557, -0.016735, -0.014530,
                                            -0.011314, -0.007677, -0.005169, -0.003944, -0.003612};

    Layer delta_log_density_ratio_array = {-0.172421, -0.182258, -0.178090, -0.176372, -0.154322,
                                            -0.113750, -0.090582, -0.075033, -0.064679, -0.056067,
                                            -0.048461, -0.043042, -0.038869, -0.035648, -0.033063,
                                            -0.029164, -0.024220, -0.021336, -0.017686, -0.016035,
                                            -0.014327, -0.011631, -0.008248, -0.005580, -0.004227};


    double geometric_altitude = altitude/1e3;
    if (geometric_altitude > height_array.back()) {
        density = density_ratio_array.back() * kDensitySeaLevel;
        pressure = pressure_ratio_array.back() * kPressureSeaLevel;
        temperature = 1000.0;
    }

    int i = 0;
    int j = 25;
    for (;i < 25;) {
        int k = (i + j) / 2;
        if (geometric_altitude < height_array[k]) {
            j = k;
        } else {
            i = k;
        }
        if (j <= i + 1) {
            break;
        }
    }

    double delta = std::exp(EvaluateCubic(height_array[i],
                                          log_pressure_ratio_array[i],
                                          delta_log_pressure_ratio_array[i],
                                          height_array[i+1],
                                          log_pressure_ratio_array[i+1],
                                          delta_log_pressure_ratio_array[i+1],
                                          geometric_altitude));

    double sigma = std::exp(EvaluateCubic(height_array[i],
                                          log_density_ratio_array[i],
                                          delta_log_density_ratio_array[i],
                                          height_array[i+1],
                                          log_density_ratio_array[i+1],
                                          delta_log_density_ratio_array[i+1],
                                          geometric_altitude));

    density = sigma * kDensitySeaLevel;
    pressure = delta * kPressureSeaLevel;
    temperature = KineticTemperature(geometric_altitude);
}


double Atmosphere::EvaluateCubic(double a, double fa, double fpa, double b,
                                 double fb, double fpb, double u)
{
    double d = (fb - fa) / (b - a);
    double t = (u - a) / (b - a);
    double p = 1.0 - t;
    double fu = p * fa + t * fb - p * t * (b - a) * (p * (d - fpa) - t * (d - fpb));
    return fu;
}

double Atmosphere::KineticTemperature(double geometric_altitude)
{
    double z = geometric_altitude;

    double C1 = -76.3232;
    double C2 = 19.9429;
    double C3 = 12.0;
    double C4 = 0.01875;
    double TC = 263.1905;
    double T7 = 186.8673;
    double Z8 = 91.0;
    double Z9 = 110.0;
    double T9 = 240.0;
    double Z10 = 120.0;
    double T10 = 360.0;
    double T12 = 1000.0;

    double t;
    double xx, yy;

    if (z <= Z8) {
        t = T7;
    } else if (z < Z9) {
        xx = (z - Z8) / C2;
        yy = std::sqrt(1.0 - xx * xx);
        t = TC + C1 * yy;
    } else if (z < Z10) {
        t = T9 + C3 * (z - Z9);
    } else {
        xx = (6356.7523 + Z10) / (6356.7523 + z);
        yy = (T12 - T10) * std::exp(-C4 * (z - Z10) * xx);
        t = T12 - yy;
    }
    return t;
}











