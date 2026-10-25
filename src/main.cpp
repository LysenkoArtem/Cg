#include <SFML/Graphics.hpp>
#include <cassert>
#include "cg.h"
#include <iostream>
#include "visualize.h"


void test() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML 3 Line");

    sf::Vector2f p1({100.f, 100.f});
    sf::Vector2f p2({700.f, 500.f});

    std::vector<sf::Vertex> line = {
        sf::Vertex{p1, sf::Color::Black},
        sf::Vertex{p2, sf::Color::Black}
    };

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear(sf::Color::White);
        
        window.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
        
        window.display();
    }
}

int main() {

    int mode;
    std::cout << "1.Simple polygon\n" << "2.Convex polygon 1\n" << "3.Convex polygon 2\n" << "4.Star polygon\n" << "5.Tanget to convex\n";
    std::cin >> mode;

    int n;
    std::cout << "Amount of points\n";
    std::cin >> n;

    cg::simple_polygon_2 poly;
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

        default:
            std::cout << "Wrong arguments\n";
            break;
    }
    return 0;
}
