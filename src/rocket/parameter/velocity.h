#ifndef VELOCITY_H_
#define VELOCITY_H_

#define EIGEN_MPL2_ONLY
#include "Eigen/Core"

#include <QDateTime>
#include "environment/coordinate.h"
#include "environment/sequence_clock.h"

class Velocity {
    public:
        Eigen::Vector3d ECI{0.0, 0.0, 0.0};
        Eigen::Vector3d ECEF{0.0, 0.0, 0.0};
        Eigen::Vector3d NED{0.0, 0.0, 0.0};

        Eigen::Vector3d air_body{0.0, 0.0, 0.0};
        double mach_number = 0.0;

        Velocity();

        void Initialize(const QDateTime datetime, const Eigen::Vector3d& NED, const Eigen::Vector3d& pos_LLH, const Eigen::Vector3d& pos_ECI);
        void Update(Coordinate& coordinate, const Eigen::Vector3d& ECI, const Eigen::Vector3d& pos_ECI);
};




#endif
