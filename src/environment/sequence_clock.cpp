#include "sequence_clock.h"

void SequenceClock::UpdateJulianDate() {
    QDateTime UTC_date = UTC_date_init.addSecs(countup_time);
    julian_data = UTC_date.date().toJulianDay();
    modified_julian_date = JulianDate2ModifiedJulianDate(julian_data);
    greenwich_sidereal_time = JulianDate2GreenwichSiderealTime(julian_data);
};


SequenceClock::SequenceClock() :
    UTC_date_init(QDateTime()), countup_time(0.0), countup_time_ref(0.0)
{
    UpdateJulianDate();
};

SequenceClock::SequenceClock(QDateTime UTC_init) :
    UTC_date_init(UTC_init), countup_time(0.0), countup_time_ref(0.0)
{
    UpdateJulianDate();
};

SequenceClock::SequenceClock(QDateTime UTC_init, double countup_time_init) :
     UTC_date_init(UTC_init), countup_time(0.0), countup_time_ref(countup_time_init)
{
    UpdateJulianDate();
};

void SequenceClock::SyncSolverTime(const double t) {
    countup_time = t;// - countup_time_ref;
    UpdateJulianDate();
};

double SequenceClock::JulianDate2GreenwichSiderealTime(const double julian)
{
    double julian_century = (julian - 2451545.0) / 36525.0;
    double greenwich_sidereal = 67310.54841 + (876600.0 * 3600.0 + 8640184.812866) * julian_century + 0.093104 * std::pow(julian_century, 2) - 6.2e-6 * std::pow(julian_century, 3);
    greenwich_sidereal = greenwich_sidereal * std::fmod((2.0 * 3.14159265) / 86400.0, (2.0 * 3.14159265));
    return greenwich_sidereal;
};

double SequenceClock::JulianDate2ModifiedJulianDate(const double julian)
{
    return julian - 2400000.5;
};









