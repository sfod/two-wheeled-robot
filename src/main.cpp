#include <iostream>

#include "robot.hpp"


int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
    try {
        twr::Robot robot(10, 5.5, 2);
        twr::Coordinate target_coordinate(4.0, 4.0, 0.0);
        robot.set_target(target_coordinate);
    }
    catch (std::exception &e) {
        std::cerr << e.what() << "\n";
    }

    return EXIT_SUCCESS;
}
