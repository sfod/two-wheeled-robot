#include "wheel.hpp"

#include <cmath>


namespace twr {

Wheel::Wheel(RobotConfig config) : config_(config)
{
}

void Wheel::set_target_speed(double speed)
{
    target_speed_ = speed;
}

double Wheel::max_speed() const
{
    return config_.max_speed;
}

double Wheel::acceleration() const
{
    return config_.acceleration;
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

    current_speed_ += config_.acceleration * s;
    if (current_speed_ == config_.max_speed) {
        current_speed_ = config_.max_speed;
    }

    double dist = 0.0;
    /// Use linear motion formula when robot reaches max speed.
    if (v == config_.max_speed) {
        dist = v * s;
    }
    else {
        dist = config_.acceleration * pow(s, 2) / 2.0 + v * s;
    }

    return dist;
}

}
