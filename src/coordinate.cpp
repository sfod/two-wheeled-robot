#include "coordinate.hpp"

namespace twr {

Coordinate::Coordinate(double x, double y, double theta) : x_(x), y_(y), theta_(theta)
{
}

double Coordinate::x() const
{
    return x_;
}

double Coordinate::y() const
{
    return y_;
}

double Coordinate::theta() const
{
    return theta_;
}

void Coordinate::set_x(double x)
{
    x_ = x;
}

void Coordinate::set_y(double y)
{
    y_ = y;
}

void Coordinate::set_theta(double theta)
{
    theta_ = theta;
}

}
