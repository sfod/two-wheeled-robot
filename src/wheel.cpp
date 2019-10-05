#include "wheel.hpp"

#include <cmath>


namespace twr {

Wheel::Wheel(double radius, double max_speed, double acceleration)
        : radius_(radius), max_speed_(max_speed), acceleration_(acceleration)
{
}

void Wheel::set_target_speed(double speed)
{
    target_speed_ = speed;
}

double Wheel::max_speed() const
{
    return max_speed_;
}

double Wheel::acceleration() const
{
    return acceleration_;
}

double Wheel::current_speed() const
{
    return current_speed_;
}

double Wheel::target_speed() const
{
    return target_speed_;
}

void Wheel::stop()
{
    current_speed_ = 0.0;
    target_speed_ = 0.0;
}

double Wheel::distance(long us)
{
    double s = static_cast<double>(us) / 1000000.0;

    double v = current_speed_;

    current_speed_ += acceleration_ * s;
    if (current_speed_ == max_speed_) {
        current_speed_ = max_speed_;
    }

    double dist = 0.0;
    /// Use linear motion formula when robot reaches max speed.
    if (v == max_speed_) {
        dist = v * s;
    }
    else {
        dist = acceleration_ * pow(s, 2) / 2.0 + v * s;
    }

    return dist;
}

}
