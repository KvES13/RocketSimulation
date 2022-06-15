#ifndef ACCELERATION_H_
#define ACCELERATION_H_

#define EIGEN_MPL2_ONLY
#include "Eigen/Core"

///
/// \brief Класс Ускорение
///
class Acceleration {

public:
    ///
    /// \brief Конструктор класса
    ///
    Acceleration();

    /// \brief Вектор ускорения в инерциальной СК
    Eigen::Vector3d ECI;
    /// \brief Вектор ускорения в связанной СК
    Eigen::Vector3d body;


};

#endif
