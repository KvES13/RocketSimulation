#ifndef VELOCITY_H_
#define VELOCITY_H_

#define EIGEN_MPL2_ONLY
#include "Eigen/Core"

#include <QDateTime>
#include "environment/coordinate.h"
#include "environment/sequence_clock.h"

///
/// \brief Класс для определения скорости ракеты
///
class Velocity {
public:
    ///
    /// \brief Конструктор класса
    ///
    Velocity();

    ///
    /// \brief Метод для инициализации векторов скорости
    /// \param datetime Текущее время
    /// \param NED Начальная скорость
    /// \param pos_LLH Позиция в географической СК
    /// \param pos_ECI Позиция в инерциальной геоцентрической СК
    ///
    void Initialize(const QDateTime datetime, const Eigen::Vector3d& NED,
                    const Eigen::Vector3d& pos_LLH,
                    const Eigen::Vector3d& pos_ECI);
    ///
    /// \brief Метод для обновления векторов скорости
    /// \param coordinate Класс для работы с матрицами перехода СК
    /// \param ECI Скорость в инерциальной геоцентрической СК
    /// \param pos_ECI Позиция в инерциальной геоцентрической СК
    ///
    void Update(Coordinate& coordinate, const Eigen::Vector3d& ECI, const Eigen::Vector3d& pos_ECI);

    /// \brief Вектора скорости
    Eigen::Vector3d ECI{0.0, 0.0, 0.0};
    Eigen::Vector3d ECEF{0.0, 0.0, 0.0};
    Eigen::Vector3d NED{0.0, 0.0, 0.0};

    Eigen::Vector3d air_body{0.0, 0.0, 0.0};
    double mach_number = 0.0;
};




#endif
