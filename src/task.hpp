#pragma once

#include "coordinate.hpp"


namespace twr {

/**
 * \brief Class representing targets specified by users.
 */
class Target {
public:
    Target(Coordinate target, double max_speed, double acceleration);

    const Coordinate &target() const;
    double max_speed() const;
    double acceleration() const;

    void set_target(const Coordinate &target);
    void set_max_speed(double max_speed);
    void set_acceleration(double acceleration);

private:
    Coordinate target_;
    double max_speed_;
    double acceleration_;
};


/**
 * \brief Robot's abstract internal task.
 */
class Task {
public:
    virtual ~Task() noexcept = default;
};


/**
 * \brief Robot's internal task to perform turn.
 */
class TurnTask : public Task {
public:
    explicit TurnTask(double theta);
    double theta() const noexcept;

    double acceleration() const;
    double max_speed() const;

private:
    double theta_;
};


/**
 * \brief Robot's internal task to move to the target.
 */
class MoveTask : public Task {
public:
    MoveTask(const Coordinate &current, const Coordinate &target, double max_speed, double acceleration);
    const Coordinate &coordinate() const noexcept;

    double max_speed() const;
    double acceleration() const;

    double distance() const noexcept;
    void set_distance(double distance);

private:
    Coordinate target_;
    double max_speed_;
    double acceleration_;
    double distance_;
};

}
