
#ifndef INTERPOLATEPARAMETER_H_
#define INTERPOLATEPARAMETER_H_

#include <vector>

#include "interpolate.h"


class InterpolateParameter {
    public:
        InterpolateParameter();
        InterpolateParameter(const double value);
        InterpolateParameter(const std::vector<double> x_axis_src, const std::vector<double> value_src, std::string fill_value);
        InterpolateParameter(const std::vector<double> x_axis_src, const std::vector<double> y_axis_src, const std::vector<double> value_src, std::string fill_value);
        InterpolateParameter(const InterpolateParameter& from);
        InterpolateParameter& operator=(const InterpolateParameter& from);

        double operator()(const double x_axis_value);
        double operator()(const double x_axis_value, const double y_axis_value);

    private:
        bool enable_1dlog;
        bool enable_2dlog;

        double value_const;
        interpolate::Interp1d polator_1d;
        interpolate::Interp2d polator_2d;

};

#endif
