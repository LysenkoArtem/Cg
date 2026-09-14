#include <SFML/Graphics.hpp>
#include <cassert>
#include <iostream>
#include "cg.h"
#include "visualize.h"


int main() {

    cg::point_2 a(0, 0);
    cg::point_2 b(100, 0);
    cg::point_2 c(100, 100);
    assert(cg::orientation(a, b, c) == cg::Orientation::left);

    cg::simple_polygon_2 poly;
    poly.push_back({100, 100});
    poly.push_back({200, 100});
    poly.push_back({200, 200});
    poly.push_back({100, 200});

    std::cout << cg::is_convex(poly);
    draw_poligon(poly);


    return 0;
}
