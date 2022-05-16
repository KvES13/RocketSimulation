#ifndef POSITION_H_
#define POSITION_H_

#define EIGEN_MPL2_ONLY
#include "Eigen/Core"

#include <QDateTime>
#include "environment/coordinate.h"
#include "environment/sequence_clock.h"

class Position {
    public:
        Eigen::Vector3d ECI{0.0, 0.0, 0.0};
        Eigen::Vector3d ECEF{0.0, 0.0, 0.0};
        Eigen::Vector3d LLH{0.0, 0.0, 0.0};

        Position();

        void Initialize(const QDateTime datetime, const Eigen::Vector3d& LLH);
        void Update(Coordinate& coordinate, const Eigen::Vector3d& ECI);
};

#endif
