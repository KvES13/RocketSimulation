#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QStringList>

#include "Constants.h"
#define EIGEN_MPL2_ONLY
#include "Eigen/Core"


///
/// \brief Функция для удобного вывода вектора
/// \param vec Вектор
/// \param args Аргументы для каждого элемента вектор
/// \param separator Разделить
/// \return Строка в виде Аргумент + Элемент вектора + Разделитель
///
QString printEigenVector(const Eigen::Vector3d &vec, const QStringList&& args, char separator = ',');

///
/// \brief Функция для перевода угла из градусов в радианы
/// \param deg угол в градусах
/// \return угол в радианах
///
double deg2rad(double deg);

///
/// \brief Функция для перевода угла из радиан в градусы
/// \param rad угол в радианах
/// \return угол в градусах
///
double rad2deg(double rad);
#endif // UTILS_H
