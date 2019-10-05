#include "wheel.hpp"


namespace twr {

Wheel::Wheel(double radius) : radius_(radius)
{
}

Wheel::Wheel(double radius, double target_speed) : radius_(radius), target_speed_(target_speed)
{
}

void Wheel::set_target_speed(double speed)
{
    target_speed_ = speed;
}

double Wheel::current_speed() const
{
    return current_speed_;
}

}
