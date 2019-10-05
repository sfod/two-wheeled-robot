#pragma once

namespace twr {

class Wheel {
public:
    Wheel(double radius, double max_speed, double acceleration);

    void set_target_speed(double speed);

    double max_speed() const;
    double acceleration() const;
    double current_speed() const;
    double target_speed() const;

    double distance(long us);

    void stop();

private:
    double radius_;
    double max_speed_;
    double acceleration_;
    double current_speed_ = 0.0;
    double target_speed_ = 0.0;
};

}
