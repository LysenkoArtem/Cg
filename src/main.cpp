#include <SFML/Graphics.hpp>
#include <cassert>
#include <iostream>
#include "cg.h"
#include "visualize.h"


int main() {

    cg::simple_polygon_2 poly = cg::generate_star_polygon(50);
    cg::point_2 z{0, 0.12};
    std::cout << cg::is_point_in_simple_polygon(poly, z);
    draw_poligon(poly);

    return 0;
}
