#include "robot.hpp"

#include <cmath>

#include <chrono>
#include <iostream>
#include <stdexcept>


namespace twr {

Robot::Robot(RobotConfig config)
        : current_coordinate_(0.0, 0.0, 0.0), left_wheel_(config), right_wheel_(config), robot_config_(config)
{
}

Robot::~Robot()
{
    if (t_.joinable()) {
        t_.join();
    }
}

void Robot::turn_on()
{
    active_ = true;
    t_ = std::thread(&Robot::run, this);
}

void Robot::set_target(Coordinate coordinate, double max_speed, double acceleration)
{
    std::lock_guard lock(m_);
    targets_.emplace_back(std::make_shared<Target>(coordinate, max_speed, acceleration));
}

void Robot::stop()
{
    std::lock_guard lock(m_);

    left_wheel_.stop();
    right_wheel_.stop();

    moving_ = false;
    target_set_ = false;

    /// Remove all targets.
    targets_.clear();

    /// Remove all internal tasks.
    tasks_.clear();
}

void Robot::pause()
{
    std::lock_guard lock(m_);

    left_wheel_.stop();
    right_wheel_.stop();

    moving_ = false;
}

void Robot::resume()
{
    std::lock_guard lock(m_);

    if (target_set_) {
        moving_ = true;
    }
}

void Robot::turn_off()
{
    stop();
    active_ = false;
}

Coordinate Robot::current_coordinate() const
{
    return current_coordinate_;
}

std::pair<double, double> Robot::speed() const
{
    return {left_wheel_.current_speed(), right_wheel_.current_speed()};
}

void Robot::run()
{
    using namespace std::chrono_literals;

    static constexpr auto period = 100us;

    auto start = std::chrono::steady_clock::now();
    auto target_time = start + period;

    while (active_) {
        // TODO Split into separate methods.
        try {
            if (tasks_.empty()) {
                generate_tasks();
            }

            std::lock_guard lock(m_);

            if (moving_ && !tasks_.empty()) {
                auto task = tasks_.front();
                if (auto turn_task = std::dynamic_pointer_cast<TurnTask>(task)) {
                    left_wheel_.set_max_speed(turn_task->max_speed());
                    left_wheel_.set_acceleration(turn_task->acceleration());

                    right_wheel_.set_max_speed(turn_task->max_speed());
                    right_wheel_.set_acceleration(turn_task->acceleration());

                    if (turn(turn_task->theta(), period.count())) {
                        std::cout << "Turn was completed successfully\n";

                        left_wheel_.stop();
                        right_wheel_.stop();

                        tasks_.pop_front();
                    }
                }
                else if (auto move_task = std::dynamic_pointer_cast<MoveTask>(task)) {
                    left_wheel_.set_max_speed(move_task->max_speed());
                    left_wheel_.set_acceleration(move_task->acceleration());

                    right_wheel_.set_max_speed(move_task->max_speed());
                    right_wheel_.set_acceleration(move_task->acceleration());

                    if (move(move_task, period.count())) {
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
                target_set_ = false;
                moving_ = false;
            }
        }
        catch (std::exception &e) {
            std::cerr << "Error occurred: " << e.what() << "\n";
        }

        std::this_thread::sleep_until(target_time);
        target_time += period;
    }
}

void Robot::generate_tasks()
{
    std::lock_guard lock(m_);

    if (targets_.empty()) {
        return;
    }

    auto target = targets_.front();
    targets_.pop_front();

    double theta = calc_theta(target->target());

    tasks_.emplace_back(std::make_shared<TurnTask>(TurnTask(theta)));
    tasks_.emplace_back(std::make_shared<MoveTask>(MoveTask(current_coordinate_, target->target(), target->max_speed(), target->acceleration())));
    tasks_.emplace_back(std::make_shared<TurnTask>(TurnTask(target->target().theta())));

    moving_ = true;
    target_set_ = true;
}

bool Robot::turn(double target_theta, long us)
{
    left_wheel_.rotate(Wheel::Direction::FORWARD);
    right_wheel_.rotate(Wheel::Direction::BACKWARD);

    left_wheel_.update(us);
    right_wheel_.update(us);

    double s = static_cast<double>(us) / 1000000.0;

    double left_v = left_wheel_.current_speed();
    double right_v = right_wheel_.current_speed();

    if (std::abs(left_v) - std::abs(right_v) > 0.001) {
        throw std::runtime_error("wheels are out of sync with each other: "
                + std::to_string(left_v) + " and " + std::to_string(right_v));
    }

    double new_theta = current_coordinate_.theta() + 2 * left_v * s / robot_config_.wheels_distance;
    if (new_theta > M_PI) {
        new_theta -= 2 * M_PI;
    }

    current_coordinate_.set_theta(new_theta);

    return std::abs(new_theta - target_theta) < 0.00001;
}

bool Robot::move(std::shared_ptr<MoveTask> task, long us)
{
    left_wheel_.rotate(Wheel::Direction::FORWARD);
    right_wheel_.rotate(Wheel::Direction::FORWARD);

    double left_wheel_dist = left_wheel_.distance(us);
    double right_wheel_dist = right_wheel_.distance(us);

    if (std::abs(left_wheel_dist - right_wheel_dist) > 0.001) {
        throw std::runtime_error("robot got off the course");
    }

    double new_x = current_coordinate_.x() + cos(current_coordinate_.theta()) * left_wheel_dist;
    double new_y = current_coordinate_.y() + sin(current_coordinate_.theta()) * right_wheel_dist;

    double dist = sqrt(pow(task->coordinate().x() - new_x, 2) + pow(task->coordinate().y() - new_y, 2));
    if (dist > task->distance()) {
        std::cout << "Robot is moving away from the destination\n";
        return true;
    }

    task->set_distance(dist);

    current_coordinate_.set_x(new_x);
    current_coordinate_.set_y(new_y);

    // FIXME Pass accuracy.
    return dist < 0.001;
}

double Robot::calc_theta(const Coordinate &coordinate) const
{
    double y_diff = coordinate.y() - current_coordinate_.y();
    double x_diff = coordinate.x() - current_coordinate_.x();
    double theta = atan2(y_diff, x_diff);
    return theta;
}

}
