#pragma once

#include <deque>
#include <mutex>
#include <string>
#include <thread>

#include "coordinate.hpp"
#include "robot_config.hpp"
#include "task.hpp"
#include "wheel.hpp"


namespace twr {

class Robot {
public:
    explicit Robot(RobotConfig config);
    ~Robot();

    void set_target(Coordinate coordinate);
    void stop();
    void pause();
    void resume();
    std::string status();

    bool is_target_set() const;

    void turnoff();

private:
    void run();

    void set_speed_left(double w);
    void set_speed_right(double w);

    bool turn(double theta, long us);
    bool move(Coordinate coordinate, long us);

    double calc_theta(Coordinate coordinate) const;

private:
    bool active_ = false;
    bool moving_ = false;
    bool target_set_ = false;

    Coordinate current_coordinate_;

    Wheel left_wheel_;
    Wheel right_wheel_;

    std::mutex m_;
    std::thread t_;

    std::deque<std::shared_ptr<Task>> tasks_;
};

}
