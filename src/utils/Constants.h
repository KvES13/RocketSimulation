#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cmath>
#include <QString>

///
/// Пространство имён Константы
///
namespace Constants
{

    const QString CfgPath   = "/../RocketSimulation/res/config";
    const QString OutputPath   = "/../RocketSimulation/output";
    /// \brief  Число Пи
    inline constexpr double pi     = 3.14159265358979323846;
    inline constexpr double twopi  = 2.0*3.14159265358979323846;
    inline constexpr double halfpi = 0.5*3.14159265358979323846;
    inline constexpr double pi2    = 3.14159265358979323846*3.14159265358979323846;

    //Универсальные физические постоянные
    /// \brief Гравитационная постоянная (m^3/kg/s^2)
    inline constexpr double G = 6.667259e-11;
    /// \brief geocentric gravitational constant (m^3/s^2)
    inline constexpr double GM = 3.986004418e14;

    //Фундаментальные геодезические постоянные
    /// \brief Геоцентрическая графитационная постоянная (m^3/s^2)
    inline constexpr double fM_earth    = 398600.4418e9; // !
    /// \brief Угловая скорость вращения Земли (рад/с)
    inline constexpr double w_earth     = 7.292115e-5;
    /// \brief Большая полуось общеземного эллипсоида (м)
    inline constexpr double a_earth     = 6'378'136;
    /// \brief Сжатие общеземного эллипсоида
    inline constexpr double alpha_earth = 1/298.25784;
    /// \brief Средний радиус Земли (м)
    inline constexpr double radius_earth = 6'371'000;
    /// \brief Среднее по Земле ускорение нормальной силы тяжести (м/c^2)
    inline constexpr double g_earth = 9.806230;

    // Геодезические постоянные
    // Геометрические постоянные
    /// \brief Малая полуось (м)
    inline constexpr double b   = 6'356'751.3618;
    /// \brief квадрат экцентриситета ОЗЭ
    inline constexpr double e12  = 0.0066943662;
    /// \brief квадрат второго экцентриситета ОЗЭ
    inline constexpr double e22 = 0.0067394828;

}


#endif // CONSTANTS_H
