#include "wheel.hpp"

#include <cmath>


namespace twr {

Wheel::Wheel(RobotConfig config) : config_(config)
{
}

void Wheel::update(long us)
{
    if (!rotating_) {
        return;
    }

    double s = static_cast<double>(us) / 1000000.0;

    if (std::abs(current_speed_) < config_.max_speed) {
        double delta = config_.acceleration * s * static_cast<int>(direction_);
        current_speed_ += delta;
    }

    if (std::abs(current_speed_) > config_.max_speed) {
        current_speed_ = config_.max_speed * static_cast<int>(direction_);
    }
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

void Wheel::set_max_speed(double max_speed)
{
    config_.max_speed = max_speed;
}

void Wheel::set_acceleration(double acceleration)
{
    config_.acceleration = acceleration;
}

double Wheel::distance(long us)
{
    if (!rotating_) {
        return 0.0;
    }

    double s = static_cast<double>(us) / 1000000.0;

    double v = current_speed_;

    if (current_speed_ < config_.max_speed) {
        current_speed_ += config_.acceleration * s * static_cast<int>(direction_);
    }

    if (current_speed_ > config_.max_speed) {
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

void Wheel::rotate(Direction direction)
{
    direction_ = direction;
    rotating_ = true;
}

void Wheel::stop()
{
    current_speed_ = 0.0;
    rotating_ = false;
}

}
