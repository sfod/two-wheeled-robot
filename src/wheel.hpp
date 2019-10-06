#pragma once

#include "robot_config.hpp"


namespace twr {

class Wheel {
public:
    Wheel(RobotConfig config);

    void set_target_speed(double speed);

    double max_speed() const;
    double acceleration() const;
    double current_speed() const;
    double target_speed() const;

    double distance(long us);

    void stop();

private:
    RobotConfig config_;
    double current_speed_ = 0.0;
    double target_speed_ = 0.0;
};

}
