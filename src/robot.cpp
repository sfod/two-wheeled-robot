#include "robot.hpp"

#include <cassert>
#include <cmath>

#include <chrono>
#include <iostream>
#include <stdexcept>


namespace twr {

Robot::Robot(double radius, double max_speed, double acceleration)
        : current_coordinate_(0.0, 0.0, 0.0), target_coordinate_(0.0, 0.0, 0.0),
        left_wheel_(radius), right_wheel_(radius),
        max_speed_(max_speed), acceleration_(acceleration)
{
    t_ = std::thread(&Robot::run, this);
}

Robot::~Robot()
{
    t_.join();
}

void Robot::set_target(Coordinate coordinate)
{
    std::lock_guard lock(m_);

    if (target_set_) {
        throw std::runtime_error("target is already set");
    }

    tasks_.clear();

    double theta = 8;
    tasks_.emplace_back(std::make_shared<TurnTask>(TurnTask(theta)));
    tasks_.emplace_back(std::make_shared<MoveTask>(MoveTask(coordinate)));
    tasks_.emplace_back(std::make_shared<TurnTask>(TurnTask(coordinate.theta())));

    moving_ = true;
    target_set_ = true;
}

void Robot::stop()
{
    std::lock_guard lock(m_);

    moving_ = false;
    target_set_ = false;

    tasks_.clear();
}

void Robot::pause()
{
    std::lock_guard lock(m_);

    moving_ = false;
}

void Robot::resume()
{
    std::lock_guard lock(m_);

    if (!target_set_) {
        throw std::runtime_error("target is not set");
    }
    moving_ = true;
}

std::string Robot::status()
{
    std::lock_guard lock(m_);

    return std::to_string(current_coordinate_.x()) + ":" + std::to_string(current_coordinate_.y())
            + ", " + std::to_string(current_coordinate_.theta()) + ", " + std::to_string(0);
}

void Robot::run()
{
    using namespace std::chrono_literals;

    std::cout << "Starting...\n";
    active_ = true;

    auto start = std::chrono::steady_clock::now();
    auto target_time = start + 20ms;

    while (active_) {
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(target_time - start).count() << "\n";

        {
            std::lock_guard lock(m_);
            if (moving_) {

            }
        }

        std::this_thread::sleep_until(target_time);
        target_time += 20ms;
    }
}

void Robot::set_speed_left(double w)
{
    left_wheel_.set_target_speed(w);
}

void Robot::set_speed_right(double w)
{
    right_wheel_.set_target_speed(w);
}

void Robot::turn(double theta)
{
    assert(theta <= 2 * M_PI);
}

}
