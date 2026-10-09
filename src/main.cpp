#include <SFML/Graphics.hpp>
#include <cassert>
#include <iostream>
#include "cg.h"
#include "visualize.h"


int main() {

    cg::simple_polygon_2 poly = cg::generate_star_polygon(16);
    for (int i=0; i < poly.size(); ++i) {
        //std::cerr << poly[i].x << ' ' << poly[i].y << '\n';
    }
    draw_poligon(poly);

    return 0;
}
