#ifndef INTERPOLATE_HPP_
#define INTERPOLATE_HPP_

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>


namespace interpolate {
class Polator1D {
    public:
        virtual double polate(const double& x, const std::vector<double>& x_src,
                              const std::vector<double>& y_src,
                              const int& fill_value) = 0;

};


class Linear1D : public Polator1D {
    public:
        double polate(const double& x, const std::vector<double>& x_src,
                      const std::vector<double>& y_src,
                      const int& fill_value) override;
};


class CubicSpline1D : public Polator1D {
    public:
        CubicSpline1D(const std::vector<double>& x, const std::vector<double>& y);
        ~CubicSpline1D();
        CubicSpline1D(const CubicSpline1D& from);
        CubicSpline1D& operator=(const CubicSpline1D& from);

        double polate(const double& x, const std::vector<double>& x_src,
                      const std::vector<double>& y_src, const int& fill_value) override;
    
    private:
        void SolveSplineCoefficient(const std::vector<double>& x, const std::vector<double>& y);
        int array_size;
        double* a_spline;
        double* b_spline;
        double* c_spline;
        double* d_spline;
};


class Interp1d {
    public:
        Interp1d() {};
        Interp1d(const std::vector<double>& x, const std::vector<double>& y,
                 const std::string kind, const std::string fill_value);
        Interp1d(const Interp1d& from);
        Interp1d& operator=(const Interp1d& from);

        double operator()(const double x);
        std::vector<double> operator()(const std::vector<double>& x);
    
        std::vector<double> x_src;
        std::vector<double> y_src;
        int fill_value;

    private:
        Polator1D* polator;

        bool need_sort(const std::vector<double>& x);
        void ascending_order_sort(std::vector<double>& x, std::vector<double>& y);

};


class Interp2d {
    public:
        Interp2d() {};
        Interp2d(const std::vector<double>& x, const std::vector<double>& y,
                 const std::vector<double>& z, const std::string fill_value);

        double operator()(const double x, const double y);


    private:
        std::vector<double> x_src;
        std::vector<double> y_src;
        int fill_value;
};
}  // namespace interpolate


#endif
