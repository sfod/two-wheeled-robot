#include <iostream>

#include "contrib/httplib.h"
#include "contrib/json.hpp"

#include "robot_config.hpp"
#include "robot.hpp"


struct ServerConfig {
    int port;
};

bool init(int argc, char **argv, ServerConfig &server_config, twr::RobotConfig &robot_config)
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <config file>\n";
        return false;
    }

    std::ifstream config_ifs(argv[1]);
    nlohmann::json json;

    try {
        config_ifs >> json;

        json.at("server").at("port").get_to(server_config.port);

        json.at("robot").at("wheel-radius").get_to(robot_config.wheel_radius);
        json.at("robot").at("wheels-distance").get_to(robot_config.wheels_distance);
        json.at("robot").at("max-speed").get_to(robot_config.max_speed);
        json.at("robot").at("acceleration").get_to(robot_config.acceleration);
    }
    catch (std::exception &e) {
        std::cerr << "Failed to parse config file: " << e.what() << "\n";
        return false;
    }

    return true;
}

int main(int argc, char **argv)
{
    httplib::Server server;

    ServerConfig server_config{};
    twr::RobotConfig robot_config{};

    if (!init(argc, argv, server_config, robot_config)) {
        return EXIT_FAILURE;
    }

    try {
        twr::Robot robot(robot_config);

        server.Post("/goto", [&robot](const httplib::Request &req, httplib::Response &resp) {
            if (robot.is_target_set()) {
                resp.status = 409;
                resp.set_content("Conflict", "text/plain");
                return;
            }

            try {
                auto json = nlohmann::json::parse(req.body);
                twr::Coordinate target{json.at("x"), json.at("y"), json.at("theta")};
                robot.set_target(target);
            }
            catch (std::exception &e) {
                std::cerr << "Invalid request: " << req.body << ", error: " << e.what() << "\n";
                resp.status = 400;
                resp.set_content("Bad Request", "text/plain");
                return;
            }
        });

        server.Get("/stop", [&robot]([[maybe_unused]] const httplib::Request &req, [[maybe_unused]] httplib::Response &resp) {
            robot.stop();
        });

        server.Get("/pause", [&robot]([[maybe_unused]] const httplib::Request &req, [[maybe_unused]] httplib::Response &resp) {
            robot.pause();
        });

        server.Get("/resume", [&robot]([[maybe_unused]] const httplib::Request &req, [[maybe_unused]] httplib::Response &resp) {
            robot.resume();
        });

        // TODO Return JSON.
        server.Get("/status", [&robot]([[maybe_unused]] const httplib::Request &req, httplib::Response &resp) {
            resp.set_content(robot.status(), "text/plain");
        });

        server.Get("/turnoff", [&server, &robot]([[maybe_unused]] const httplib::Request &req, [[maybe_unused]] httplib::Response &resp) {
            robot.turnoff();
            server.stop();
        });

        std::cout << "Listen at 5001\n";
        server.listen("0.0.0.0", server_config.port);
    }
    catch (std::exception &e) {
        std::cerr << e.what() << "\n";
    }

    return EXIT_SUCCESS;
}
