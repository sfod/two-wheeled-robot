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

    /**
     * \brief Start robot's internal thread to execute internal tasks generated for the specified targets.
     */
    void turn_on();

    /**
     * \brief Set new target coordinates and orientation
     * along with robot's permissible maximum speed and acceleration.
     */
    void set_target(Coordinate coordinate, double max_speed, double acceleration);

    /**
     * \brief Stop movement, discard all set targets.
     */
    void stop();

    /**
     * \brief Stop movement, do NOT discard any target.
     */
    void pause();

    /**
     * \brief Resume movement if target is set.
     */
    void resume();

    /**
     * \brief Turn the robot off.
     */
    void turn_off();

    /**
     * \brief Return robot's current coordinates and orientation.
     */
    Coordinate current_coordinate() const;

    /**
     * \brief Return robot's current speed.
     */
    std::pair<double, double> speed() const;

private:
    void run();

    /**
     * \brief Generate from the specified target set of internal tasks, such as Turn or Move.
     */
    void generate_tasks();

    /**
     * \brief Perform turn to the calculated theta.
     */
    bool turn(double theta, long us);

    /**
     * \brief Move to the target coordinates.
     */
    bool move(std::shared_ptr<MoveTask> task, long us);

    double calc_theta(const Coordinate &coordinate) const;

private:
    bool active_ = false;
    bool moving_ = false;
    bool target_set_ = false;

    Coordinate current_coordinate_;

    Wheel left_wheel_;
    Wheel right_wheel_;

    RobotConfig robot_config_;

    std::mutex m_;
    std::thread t_;

    std::deque<std::shared_ptr<Target>> targets_;
    std::deque<std::shared_ptr<Task>> tasks_;
};

}
