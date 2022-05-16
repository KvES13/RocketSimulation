#include "position.h"


Position::Position() {
    ECI << 0.0, 0.0, 0.0;
    ECEF << 0.0, 0.0, 0.0;
    LLH << 0.0, 0.0, 0.0;
};

void Position::Initialize(const QDateTime datetime, const Eigen::Vector3d& LLH) {
    SequenceClock clock(datetime);
    Coordinate coordinate;
    this->LLH = LLH;
    ECEF = coordinate.LLH2ECEF(this->LLH);
    coordinate.setECI2ECEF(clock.countup_time);
    ECI = coordinate.dcm.ECEF2ECI * ECEF;
};


void Position::Update(Coordinate& coordinate, const Eigen::Vector3d& ECI) {
    this->ECI = ECI;
    this->ECEF = coordinate.dcm.ECI2ECEF * this->ECI;
    this->LLH = coordinate.ECEF2LLH(this->ECEF);
};
