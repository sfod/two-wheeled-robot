#pragma once

#include "robot_config.hpp"


namespace twr {

class Wheel {
public:
    enum class Direction {
        BACKWARD = -1,
        FORWARD = 1
    };

public:
    Wheel(RobotConfig config);

    void update(long us);

    double max_speed() const;
    double acceleration() const;
    double current_speed() const;

    void set_max_speed(double max_speed);
    void set_acceleration(double acceleration);

    double distance(long us);

    void rotate(Direction direction);
    void stop();

private:
    RobotConfig config_;
    double current_speed_ = 0.0;
    bool rotating_ = false;
    Direction direction_ = Direction::FORWARD;
};

}
