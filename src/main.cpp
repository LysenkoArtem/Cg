#include <SFML/Graphics.hpp>
#include <cassert>
#include <iostream>
#include "cg.h"
#include "visualize.h"


int main() {

    cg::simple_polygon_2 poly = cg::generate_star_polygon(50);
    draw_poligon(poly);

    return 0;
}
