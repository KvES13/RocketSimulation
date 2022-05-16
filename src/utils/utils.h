#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QStringList>

#include "Constants.h"
#define EIGEN_MPL2_ONLY
#include "Eigen/Core"

//template <typename T>
QString printEigenVector(const Eigen::Vector3d &vec, const QStringList&& args, char separator = ',');

// QString printEigenVector<Eigen::Vector3d> (const Eigen::Vector3d &vec, const QStringList&& args, char separator = ',');

/*!
* @brief       Converts an angle in degrees to radians.
* @ingroup     Geometry
* @param       degrees An angle in degrees.
* @return      The angle in radians.
*/
double deg2rad(double deg);

/*!
* @brief       Converts an angle in radians to degrees.
* @ingroup     Geometry
* @param       radians An angle in radians.
* @return      The angle in degrees.
*/
double rad2deg(double rad);
#endif // UTILS_H
