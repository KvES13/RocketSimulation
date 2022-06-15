#ifndef ATMOSPHERE_H
#define ATMOSPHERE_H

#include "Constants.h"
#include <array>

///
/// \brief Класс Атмосфера
///
class Atmosphere
{
public:

    ///
    /// \brief Конструктор класса
    /// \param Высота [м]
    ///
    explicit Atmosphere(double currentAltitude = 0);

    ///
    /// \brief Метод для расчёта параметров атмосферы
    /// \param Высота [м]
    ///
    void calculate(double currentAltitude);
    ///
    /// \brief Возвращает текущую температуру
    /// \return Температура
    ///
    double getTemperature() const {return temperature;};
    ///
    /// \brief Возвращает текущее давление
    /// \return Давление
    ///
    double getPressure() const {return pressure;};
    ///
    /// \brief Возвращает текущую плотность воздуха
    /// \return Плотность воздуха
    ///
    double getDensity() const {return density;};
    ///
    /// \brief Возвращает текущую скорость звука
    /// \return Скорость звука
    ///
    double getSpeedOfSound() const {return  speedOfSound;};

private:
    ///
    /// \brief Метод для расчёта атмосферы на высотах до 86 км
    ///
    void calcLowerAtmosphere();
    ///
    /// \brief Метод для расчёта атмосферы на высотах от 86 км
    ///
    void calcUpperAtmosphere();

    double temperature; ///<- Температура
    double pressure;    ///<- Давление
    double density;     ///<- Плотность
    double speedOfSound ; ///<- Скорость звука

    double altitude; ///<- Высота


    /// \brief Дж/(кг·К) – удельная газовая постоянная для. воздуха
    static constexpr double kR = 287.05287;
    /// Константы на уровне моря
    static constexpr double kTemperatureSeaLevel = 288.15;
    static constexpr double kSpeedOfSoundSeaLevel = 340.294;
    static constexpr double kDensitySeaLevel = 1.225;
    static constexpr double kPressureSeaLevel = 101325.0;


    double EvaluateCubic(double a, double fa, double fpa, double b, double fb, double fpb, double u);
    double KineticTemperature(double geometric_altitude);

};

#endif // ATMOSPHERE_H
