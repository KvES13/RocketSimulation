#ifndef MASS_H_
#define MASS_H_

///
/// \brief Класс Масса ракеты
///
class Mass {
public:

    /// \brief Масса конструкции
    double inert;

    /// \brief Масса топлива
    double propellant;

    double Sum() {return inert /*+ propellant*/;}
};

#endif
