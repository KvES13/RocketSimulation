#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "atmosphere.h"
#include "sequence_clock.h"
#include "Constants.h"
///
/// \brief The Environment class
///
class Environment
{
public:
    ///
    /// \brief Конструктор класса
    /// \param startCoord начальные координаты
    ///
    explicit Environment(){ atmosphere = new Atmosphere(0);};
    explicit Environment (Atmosphere* atmosphere) : atmosphere(atmosphere){};

    /// @todo !COPY Constructor

    ///
    ///\brief Деструктор класса
    ///
    ~Environment();
    ///
    /// \brief Класс расчёта параметров атмосферы
    ///
    Atmosphere *atmosphere;
    void updateAltitude(double altitude);
    SequenceClock masterClock;

    ///
    /// \brief Функция расчёта силы тяжести
    /// \param altitude Высота полёта
    /// \return
    ///
    double getGravity(const double altitude);

private:

};


#endif // ENVIRONMENT_H
