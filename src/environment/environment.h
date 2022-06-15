#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "atmosphere.h"
#include "sequence_clock.h"
#include "Constants.h"

///
/// \brief Класс Окружение
///
class Environment
{
public:

    ///
    /// \brief Конструктор класса
    ///
    explicit Environment(){ /*atmosphere = new Atmosphere(0);*/};
 //   explicit Environment (Atmosphere* atmosphere) : atmosphere(atmosphere){};
  //  Environment (const Environment& env);

    /// \brief Деструктор класса
    ~Environment();

    ///
    /// \brief Атмосфера стандартная
    ///
    Atmosphere /***/atmosphere;
    ///
    /// \brief Метод для обновления текущей высота
    /// \param altitude Высота
    ///
    void updateAltitude(double altitude);

    ///
    /// \brief Метод для расчёта гравитации
    /// \param altitude Высота
    /// \return Гравитация
    ///
    double getGravity(const double altitude);

    /// \brief Время
    SequenceClock masterClock;

private:

};


#endif // ENVIRONMENT_H
