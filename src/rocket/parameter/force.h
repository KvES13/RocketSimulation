#ifndef FORCE_HPP_
#define FORCE_HPP_

#define EIGEN_MPL2_ONLY
#include "Eigen/Core"

///
/// \brief Класс Сил, дейсвтвующих на ракету
///
class Force {

public:
    ///
    /// \brief Конструктор класса
    ///
    Force();

    /// \brief Вектор тяги
    Eigen::Vector3d thrust;
    /// \brief Вектор аэродинамических сил
    Eigen::Vector3d aero;
    /// \brief Вектор гравитации
    Eigen::Vector3d gravity;

    ///
    /// \brief Метод для нахождения суммы сил
    /// \return Вектор суммы сил
    ///
    Eigen::Vector3d Sum();

};


#endif
