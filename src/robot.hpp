#pragma once

#include <deque>
#include <mutex>
#include <string>
#include <thread>

#include "coordinate.hpp"
#include "task.hpp"
#include "wheel.hpp"


namespace twr {

class Robot {
public:
    Robot(double radius, double max_speed, double acceleration);
    ~Robot();

    void set_target(Coordinate coordinate);
    void stop();
    void pause();
    void resume();
    std::string status();

private:
    void run();

    void set_speed_left(double w);
    void set_speed_right(double w);

    bool turn(double theta, long ms);
    bool move(Coordinate coordinate, long ms);

    double calc_theta(Coordinate coordinate) const;
    double calc_delta_theta(Coordinate coordinate) const;

private:
    bool active_ = false;
    bool moving_ = false;
    bool target_set_ = false;

    Coordinate current_coordinate_;

    Wheel left_wheel_;
    Wheel right_wheel_;

    double max_speed_;
    double acceleration_;

    std::mutex m_;
    std::thread t_;

    std::deque<std::shared_ptr<Task>> tasks_;
};

}
