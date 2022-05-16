#ifndef SEQUENCECLOCK_HPP_
#define SEQUENCECLOCK_HPP_

#include <QDateTime>
#include <cmath>
class SequenceClock {
    public:
        SequenceClock();
        SequenceClock(QDateTime UTC_init);
        SequenceClock(QDateTime UTC_init, double countup_time_init);

        void SyncSolverTime(const double t);

        double julian_data;
        double modified_julian_date;
        double greenwich_sidereal_time;
        QDateTime UTC_date_init;
        double countup_time;

    private:
        double countup_time_ref;
        void UpdateJulianDate();
        double JulianDate2GreenwichSiderealTime(const double julian);
        double JulianDate2ModifiedJulianDate(const double julian);
};


#endif
