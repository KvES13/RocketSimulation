#include "utils.h"




double deg2rad(double deg)
{
   return deg * Constants::pi / 180;
}

double rad2deg(double rad)
{
   return rad * 180 / Constants::pi;
}

QString printEigenVector(const Eigen::Vector3d &vec, const QStringList &&args, char separator)
{

    if(args.size() != vec.size())
        return QString();

    return QString(args[0]+" "+QString::number(vec[0])+ separator + " " +
            args[1]+" "+QString::number(vec[1])+ separator + " " +
            args[2]+" "+QString::number(vec[2]));
}
