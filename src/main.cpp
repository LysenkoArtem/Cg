#include <SFML/Graphics.hpp>
#include <cassert>
#include "cg.h"
#include <iostream>
#include "visualize.h"


int main() {

    int mode;
    std::cout << "1.Simple polygon\n" << "2.Convex polygon 1\n" << "3.Convex polygon 2\n" << "4.Star polygon\n";
    std::cin >> mode;

    int n;
    std::cout << "Amount of points\n";
    std::cin >> n;

    cg::simple_polygon_2 poly;
    switch (mode) {
        case 1:
            poly = cg::generate_convex_polygon(n);
            draw_poligon(poly);
            break;
        case 2:
            poly = cg::generate_convex_polygon(n);
            draw_poligon(poly);
            break;
        case 3:
            poly = cg::generate_convex_polygon(n);
            draw_poligon(poly);
            break;
        case 4:
            poly = cg::generate_star_polygon(n);
            draw_poligon(poly);
            break;

        default:
            std::cout << "Wrong arguments\n";
            break;
    }
    return 0;
}
