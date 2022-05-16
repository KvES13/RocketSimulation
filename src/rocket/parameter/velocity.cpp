#include "velocity.h"


Velocity::Velocity() {
    ECI << 0.0, 0.0, 0.0;
    ECEF << 0.0, 0.0, 0.0;
    NED << 0.0, 0.0, 0.0;
    air_body << 0.0, 0.0, 0.0;
    mach_number = 0.0;
};


void Velocity::Initialize(const QDateTime datetime, const Eigen::Vector3d& NED, const Eigen::Vector3d& pos_LLH, const Eigen::Vector3d& pos_ECI) {
    SequenceClock clock(datetime);
    Coordinate coordinate;
    this->NED = NED;
    coordinate.setECEF2NED(pos_LLH);
    this->ECEF = coordinate.dcm.NED2ECEF * this->NED;
    coordinate.setECI2ECEF(clock.countup_time);
    this->ECI = coordinate.dcm.ECEF2ECI * this->ECEF + coordinate.dcm.EarthRotate * pos_ECI;
   };


void Velocity::Update(Coordinate& coordinate, const Eigen::Vector3d& ECI, const Eigen::Vector3d& pos_ECI) {
    this->ECI = ECI;
    this->ECEF = coordinate.dcm.ECI2ECEF * this->ECI - coordinate.dcm.EarthRotate * pos_ECI;
    this->NED = coordinate.dcm.ECEF2NED * this->ECEF;
};

