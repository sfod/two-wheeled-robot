#include <iostream>

#include "robot.hpp"


int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
    try {
        twr::Robot robot(10, 5.5, 2);
//        std::cout << "Robot status: " << robot.status() << std::endl;
    }
    catch (std::exception &e) {
        std::cerr << e.what() << "\n";
    }
//    twr::Coordinate target_coordinate(0, 11.6, 0.0);
//    robot.set_target(target_coordinate);

    return EXIT_SUCCESS;
}
