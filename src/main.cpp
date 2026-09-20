#include <SFML/Graphics.hpp>
#include <cassert>
#include "cg.h"
#include "visualize.h"


int main() {

    cg::simple_polygon_2 poly = cg::generate_star_polygon(10);
    draw_poligon(poly);
    

    return 0;
}
