#ifndef CG_H
#define CG_H

#include <cstdlib>
#include <vector>

namespace cg {

struct point_2 {
    double x;
    double y;

    point_2(double X, double Y) {
        x = X;
        y = Y;
    }

    point_2() {
        x = 0;
        y = 0;
    }

    point_2 operator+(const point_2& other) const {
        return {x + other.x, y + other.y};
    }

    point_2 operator-(const point_2& other) const {
        return {x - other.x, y - other.y};
    }

    double operator*(const point_2& other) const {
        return x * other.x + y * other.y;
    }

    point_2 operator/(double d) const {
        return {x / d, y / d};
    }
};

using simple_polygon_2 = std::vector<point_2>;

double pseudoscalar(point_2 a, point_2 b);

enum class Orientation {
    left,
    right,
    collinear
};

Orientation orientation(point_2 a, point_2 b, point_2 c);

bool is_convex(simple_polygon_2 const& poly);

simple_polygon_2 generate_convex_polygon(int n);
simple_polygon_2 generate_star_polygon(int n);

}

#endif
