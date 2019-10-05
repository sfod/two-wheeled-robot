#pragma once

#include "coordinate.hpp"


namespace twr {

class Task {
public:
    virtual ~Task() noexcept = default;
};


class TurnTask : public Task {
public:
    explicit TurnTask(double theta);
    double theta() const noexcept;

private:
    double theta_;
};


class MoveTask : public Task {
public:
    explicit MoveTask(Coordinate coordinate);
    Coordinate coordinate() const noexcept;

private:
    Coordinate coordinate_;
};

}
