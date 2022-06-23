#ifndef ENGINE_H_
#define ENGINE_H_

#include <vector>
#include "rocket/parameter/interpolate_parameter.h"
#include "Constants.h"
#include "jsonwrapper.h"
///
/// \brief The Engine class
///  Класс, описывающий двигетели
class Engine {

public:

    bool burning = false;              ///< Флаг работы двигетеля
    double burn_duration = 0.0;        ///< Время работы

    double thrust = 0.0;               ///< Тяга
    double mdot_prop = 0.0;            ///< Расхож топлива
    double total_impulse = 0.0;        ///< Импульс

    double gimbal_angle_y_axis = 0.0;
    double gimbal_angle_z_axis = 0.0;

    /// \brief Конструктор по умолчанию
    Engine() {};

    ///
    /// \brief Engine
    /// \param burn_duration Время работы
    /// \param thrust_const  Постоянная тяга
    /// \param mdot_prop_const Постоянный расход
    /// \param area_exit  Площадь
    ///
    Engine(const double burn_duration, const double thrust_const,
           const double thrust_coef,const double ISP,const double diameter_exit,
           const double area_exit);

    Engine(const std::vector<double> &time_vector,
           const std::vector<double> &thrust_vacuum_vector,
           const double area_exit,const double diameter_exit, const double isp);

    ///
    /// \brief Статическая функция создания объекта Двигателя
    /// \param filepath Путь к конфигурационному файлу
    /// \return Экземпляр класса
    ///
    static Engine create(JsonWrapper& json);

    ///
    /// \brief Обновление параметров тяги двигателя
    /// \param t время работы двигателя
    /// \param pressure давление
    /// \param mass_prop секундный расхож топлива
    ///
    void Update(const double t, const double pressure);
     ///
    /// \brief Функция включения двигателя
    ///
    void Ignittion();
    ///
    /// \brief Функция отключения двигателя
    ///
    void Cutoff();

    ///
    /// \brief Возвращает тягу двигателя в вакууме
    /// \return Тяга двигателя в вакууме
    ///
    double getThrustVacuum()  {return thrust_vacuum_src(0.0);}
    void setThrust(double value) {thrust_vacuum_src = InterpolateParameter(value);}

    double getDiameter() const {return diameter;}

    double getISPvac() const {return ISPvac;}

    void setAreaExit(double diameter_exit);

private:
    ///
    /// \brief Сбрасывает все параметры
    ///
    void Reset();
    ///
    /// \brief getTotalImpulse
    /// \param thrust тяга двигателя
    /// \param burn_duration продолжительность работы
    ///
    void getTotalImpulse(const double thrust, const double burn_duration);
    void getTotalImpulse(const std::vector<double> &thrust,
                     const double burn_duration);

    double area_exit = 0.0;
    double diameter = 0.0;
    double mis_alignment_angle_y_axis = 0.0;
    double mis_alignment_angle_z_axis = 0.0;

    bool enable_gimbal = false;

    double ISPvac = 303;
    double thrust_coefficient = 1;
    double nozzle_expansion_ratio = 18.86;

    InterpolateParameter thrust_vacuum_src;

};


#endif
