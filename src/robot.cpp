#include "robot.hpp"

#include <cassert>
#include <cmath>

#include <chrono>
#include <iostream>
#include <stdexcept>


namespace twr {

Robot::Robot(double radius, double max_speed, double acceleration)
        : current_coordinate_(0.0, 0.0, 0.0),
        left_wheel_(radius, max_speed, acceleration), right_wheel_(radius, max_speed, acceleration)
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

    double theta = calc_theta(coordinate);
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

    static constexpr auto period = 100us;

    std::cout << "Starting...\n";
    active_ = true;

    auto start = std::chrono::steady_clock::now();
    auto target_time = start + period;

    while (active_) {
        // TODO Split into separate methods.
        {
            std::lock_guard lock(m_);
            if (moving_ && !tasks_.empty()) {
                auto task = tasks_.front();
                if (auto turn_task = std::dynamic_pointer_cast<TurnTask>(task)) {
                    if (turn(turn_task->theta(), period.count())) {
                        std::cout << "Turn was completed successfully\n";
                        tasks_.pop_front();
                    }
                }
                else if (auto move_task = std::dynamic_pointer_cast<MoveTask>(task)) {
                    if (move(move_task->coordinate(), period.count())) {
                        std::cout << "Reached destination\n";
                        /// A very non-realistic assumption that robot can come to a complete stop immediately.
                        left_wheel_.stop();
                        right_wheel_.stop();

                        tasks_.pop_front();
                    }
                }
            }

            if (moving_ && tasks_.empty()) {
                std::cout << "Target reached\n";
                active_ = false; // FIXME
            }
        }

        std::this_thread::sleep_until(target_time);
        target_time += period;
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

bool Robot::turn(double target_theta, long us)
{
    assert(target_theta >= -M_PI && target_theta <= M_PI);

    double s = static_cast<double>(us) / 1000000.0;

    // FIXME Calculate speed.
    // FIXME Turn to the nearest direction.
    double v = 1.0;
    if (target_theta < current_coordinate_.theta()) {
        v = -v;
    }

    // FIXME Pass wheel base.
    double l = 20.0;
    double new_theta = current_coordinate_.theta() + 2 * v * s / l;

    std::cout << "Current theta is " << new_theta << ", target theta is " << target_theta << "\n";

    current_coordinate_.set_theta(new_theta);

    return std::abs(new_theta - target_theta) < 0.00001;
}

bool Robot::move(Coordinate coordinate, long us)
{
    double wheel_dist = left_wheel_.distance(us);

    double new_x = current_coordinate_.x() + cos(current_coordinate_.theta()) * wheel_dist;
    double new_y = current_coordinate_.y() + sin(current_coordinate_.theta()) * wheel_dist;

    std::cout << "Current coord is " << new_x << ":" << new_y
            << ", target coord is " << coordinate.x() << ":" << coordinate.y()
            << ", speed is " << left_wheel_.current_speed() << "\n";

    double dist = sqrt(pow(coordinate.x() - new_x, 2) + pow(coordinate.y() - new_y, 2));

    current_coordinate_.set_x(new_x);
    current_coordinate_.set_y(new_y);

    return dist < 0.001;
}

double Robot::calc_theta(Coordinate coordinate) const
{
    double y_diff = coordinate.y() - current_coordinate_.y();
    double x_diff = coordinate.x() - current_coordinate_.x();
    double theta = atan2(y_diff, x_diff);
    return theta;
}

}
