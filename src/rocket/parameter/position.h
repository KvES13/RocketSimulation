#ifndef POSITION_H_
#define POSITION_H_

#define EIGEN_MPL2_ONLY
#include "Eigen/Core"

#include <QDateTime>
#include "environment/coordinate.h"
#include "environment/sequence_clock.h"

///
/// \brief Класс для определения позиции ракеты
///
class Position {
public:
    ///
    /// \brief Конструктор класса
    ///
    Position();

    ///
    /// \brief Метод для инициализации координат
    /// \param datetime Время
    /// \param LLH Долгота,широта,высота
    ///
    void Initialize(const QDateTime datetime, const Eigen::Vector3d& LLH);
    ///
    /// \brief Метод для обновления координат
    /// \param coordinate Класс для работы с матрицами перехода
    /// \param ECI Вектор скорости
    ///
    void Update(Coordinate& coordinate, const Eigen::Vector3d& ECI);

    /// \brief Вектора координат
    Eigen::Vector3d ECI{0.0, 0.0, 0.0};
    Eigen::Vector3d ECEF{0.0, 0.0, 0.0};
    Eigen::Vector3d LLH{0.0, 0.0, 0.0};
};

#endif
