#include "task.hpp"

#include <cmath>


namespace twr {

Target::Target(Coordinate target, double max_speed, double acceleration)
        : target_(target), max_speed_(max_speed), acceleration_(acceleration)
{
}

const Coordinate &Target::target() const
{
    return target_;
}

double Target::max_speed() const
{
    return max_speed_;
}

double Target::acceleration() const
{
    return acceleration_;
}

void Target::set_target(const Coordinate &target)
{
    target_ = target;
}

void Target::set_max_speed(double max_speed)
{
    max_speed_ = max_speed;
}

void Target::set_acceleration(double acceleration)
{
    acceleration_ = acceleration;
}

TurnTask::TurnTask(double theta) : theta_(theta)
{
}

double TurnTask::theta() const noexcept
{
    return theta_;
}

double TurnTask::max_speed() const
{
    return 0.01;
}

double TurnTask::acceleration() const
{
    return 0.01;
}

MoveTask::MoveTask(const Coordinate &current, const Coordinate &target, double max_speed, double acceleration)
        : target_(target), max_speed_(max_speed), acceleration_(acceleration)
{
    distance_ = std::sqrt(std::pow(target_.x() - current.x(), 2) + std::pow(target_.y() - current.y(), 2));
}

const Coordinate &MoveTask::coordinate() const noexcept
{
    return target_;
}

double MoveTask::max_speed() const
{
    return max_speed_;
}

double MoveTask::acceleration() const
{
    return acceleration_;
}

double MoveTask::distance() const noexcept
{
    return distance_;
}

void MoveTask::set_distance(double distance)
{
    distance_ = distance;
}

}
