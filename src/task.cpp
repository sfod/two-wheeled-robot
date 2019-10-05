#include "task.hpp"

namespace twr {

TurnTask::TurnTask(double theta) : theta_(theta)
{
}

double TurnTask::theta() const noexcept
{
    return theta_;
}

MoveTask::MoveTask(Coordinate coordinate) : coordinate_(coordinate)
{
}

Coordinate MoveTask::coordinate() const noexcept
{
    return coordinate_;
}

}
