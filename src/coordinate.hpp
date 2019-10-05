#pragma once

namespace twr {

class Coordinate {
public:
    Coordinate(double x, double y, double theta);
    double x() const;
    double y() const;
    double theta() const;

    void set_x(double x);
    void set_y(double y);
    void set_theta(double theta);

private:
    double x_;
    double y_;
    double theta_;
};

}
