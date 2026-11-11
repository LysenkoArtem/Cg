#include <SFML/Graphics.hpp>
#include <cassert>
#include "cg.h"
#include <iostream>
#include "visualize.h"



int main() {

    int mode;
    std::cout << "1.Simple polygon\n" << "2.Convex polygon 1\n" << "3.Convex polygon 2\n" << "4.Star polygon\n" << "5.Tanget to convex\n";
    std::cin >> mode;

    int n;
    std::cout << "Amount of points\n";
    std::cin >> n;

    cg::simple_polygon_2 poly;
    cg::points_array arr;
    cg::points_array border;
    switch (mode) {
        case 1:
            poly = cg::generate_convex_polygon(n);
            draw_poligon(poly, mode);
            break;
        case 2:
            poly = cg::generate_convex_polygon(n);
            draw_poligon(poly, mode);
            break;
        case 3:
            poly = cg::generate_convex_polygon(n);
            draw_poligon(poly, mode);
            break;
        case 4:
            poly = cg::generate_star_polygon(n);
            draw_poligon(poly, mode);
            break;

        case 5:
            poly = cg::generate_convex_polygon(n);
            draw_poligon(poly, mode);
            break;

        case 6:
            arr = cg::generate_points(n);
            border = cg::convex_hull_graham(arr);
            draw_block_2(arr, border);
            break;

        case 7:
            arr = cg::generate_points(n);
            border = cg::preparata_hong(arr);
            draw_block_2(arr, border);
            break;
            
        default:
            std::cout << "Wrong arguments\n";
            break;
    }
    return 0;
}
