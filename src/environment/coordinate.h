#ifndef COORDINATE_HPP_
#define COORDINATE_HPP_

#include <cmath>
#include "utils/utils.h"


///
/// \brief Класс Матрица направляющих косинусов
///
class DirectionCosineMatrix {
public:
    Eigen::Matrix3d ECI2ECEF;
    Eigen::Matrix3d ECEF2NED;
    Eigen::Matrix3d NED2body;
    Eigen::Matrix3d wind2body;

    Eigen::Matrix3d body2NED;
    Eigen::Matrix3d NED2ECEF;
    Eigen::Matrix3d ECEF2ECI;
    Eigen::Matrix3d body2wind;

    Eigen::Matrix3d EarthRotate;

    ///
    /// \brief Конструктор по умолчанию
    ///
    DirectionCosineMatrix(){
        EarthRotate << 0.0, -Constants::w_earth, 0.0,
                        Constants::w_earth, 0.0, 0.0,
                        0.0, 0.0, 0.0;
    };
};


///
/// \brief Класс для преобразования координат
///
class Coordinate {
public:
    ///
    /// \brief Конструктор по умолчанию
    ///
    Coordinate();

    ///
    /// \brief Матрица направляющих косинусов
    ///
    DirectionCosineMatrix dcm;

    ///
    /// \brief Метод для перехода из поточной в связанную СК
    /// \param alpha угол атаки
    /// \param beta угол скольжения
    ///
    void setWind2Body(const double alpha, const double beta);

    ///
    /// \brief Метод для перехода из локальной в связанную
    /// \param euler_angle углы эйлера
    ///
    void setNED2Body(const Eigen::Vector3d& euler_angle);
    ///
    /// \brief Метод для перехода из локальной в связанную
    /// \param quaternion кватернион
    ///
    void setNED2Body(const Eigen::Vector4d& quaternion);

    ///
    /// \brief Метод для перехода из инерциальной в неинерциальную
    /// \param epoch_time текущее время интегрирования
    ///
    void setECI2ECEF(const double epoch_time);

    ///
    /// \brief Метод для перехода из геоцентрической в локальную
    /// \param pos_LLH Географическая СК
    ///
    void setECEF2NED(const Eigen::Vector3d& pos_LLH);

    ///
    /// \brief Метод для перехода из геоцентрической в географическую
    /// \param pos_ECEF Геоцентрическая СК
    /// \return
    ///
    Eigen::Vector3d ECEF2LLH(const Eigen::Vector3d& pos_ECEF);
    ///
    /// \brief Метод для перехода из географической в геоцентрическую
    /// \param pos_LLH Географическая СК
    /// \return
    ///
    Eigen::Vector3d LLH2ECEF(const Eigen::Vector3d& pos_LLH);

    ///
    /// \brief Метод для преобразования углов эйлера в кватернион
    /// \param euler_angle Углы эйлера
    /// \return Кватернион
    ///
    Eigen::Vector4d Quaternion(const Eigen::Vector3d& euler_angle);
    ///
    /// \brief Метол для расчёта углов эёлера
    /// \return
    ///
    Eigen::Vector3d EulerAngle();

    ///
    /// \brief Метод для расчёта расстояния между точками
    /// \param pos0_LLH_ Начальная точка
    /// \param pos1_LLH_ Конечная точка
    /// \return  Расстояние в метрах
    ///
    double distance_surface(const Eigen::Vector3d& pos0_LLH_,
                            const Eigen::Vector3d& pos1_LLH_);
};

#endif
