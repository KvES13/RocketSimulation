
#ifndef FLIGHTDATARECORDER_H_
#define FLIGHTDATARECORDER_H_


#include <string>
#include <QVector>
#include <QMap>

#define EIGEN_MPL2_ONLY
#include "Eigen/Core"

#include "environment.h"
#include "rocket/rocket.h"
#include "dynamics/dynamics_base.h"

///
/// \brief Класс для записи статистики
///
class FlightObserver {
public:

    FlightObserver() = default;
    explicit FlightObserver(const Rocket* rocket);

    void ReserveCapacity(int capacity);
    void operator()(const DynamicsBase::state& x, const double t);
    void DumpCsv(const std::string &file_name, bool full_dump = true);

    int counter = 0;

    const Rocket* p_rocket;
    Coordinate crd;

    double max_alt = 0;
    double max_downrage= 0;
    double max_accelerarion= 0;
    double max_speed = 0;

    double separation_altitude = 0;
    double separatiom_speed= 0;


    QVector<double> countup_time;
    QVector<double> countup_burn_time;

    QVector<double> thrust;
    QVector<double> mdot_prop;
    QVector<bool> burning;
    QVector<double> gimbal_angle_y_axis;
    QVector<double> gimbal_angle_z_axis;

    QVector<double> mass_prop;
    QVector<double> mass;
    QVector<double> length_CG;
    QVector<double> length_CP;
    std::vector<Eigen::Matrix3d> inertia_tensor;
    QVector<double> CA;
    QVector<double> CNa;
    QVector<double> Cld;
    QVector<double> Clp;
    QVector<double> Cmq;
    QVector<double> mach_number;

    QVector<Position> position;
    QVector<Velocity> velocity;
    QVector<double> dynamic_pressure;
    QVector<Acceleration> acceleration;
    QVector<Force> force;
    QVector<Attitude> attitude;
    QVector<Eigen::Vector4d> quaternion_dot;
    QVector<Eigen::Vector3d> angular_velocity;
    QVector<Eigen::Vector3d> angular_acceleration;
    QVector<double> angle_of_attack;
    QVector<double> sideslip_angle;
    QVector<Moment> moment;
    QVector<double> downrage;
    QVector<double> sum_velocity;

    QVector<QVector<double>> vposition = QVector(9,QVector<double>());
    QVector<QVector<double>> vvelocity= QVector(9,QVector<double>());
    QVector<QVector<double>> vacceleration= QVector(6,QVector<double>());
    QVector<QVector<double>> vforce= QVector(9,QVector<double>());
    QVector<QVector<double>> vattitude;
    QVector<QVector<double>> vangular_velocity;

    std::string filePath = "";
};


#endif
