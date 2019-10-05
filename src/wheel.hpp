#pragma once

namespace twr {

class Wheel {
public:
    explicit Wheel(double radius);
    Wheel(double radius, double target_speed);

    void set_target_speed(double speed);
    double current_speed() const;

private:
    double radius_;
    double current_speed_ = 0.0;
    double target_speed_ = 0.0;
};

}
