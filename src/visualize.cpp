#include <SFML/Graphics.hpp>
#include "cg.h"

#define WIDTH 1000
#define HEIGHT 1000

void draw_poligon(cg::simple_polygon_2 rect) {
    sf::RenderWindow window(
        sf::VideoMode({WIDTH, HEIGHT}),
        "Calc geometry"
    );

    sf::ConvexShape shape;


    shape.setPointCount(rect.size());

    for (std::size_t i = 0; i < rect.size(); ++i)
    {
        shape.setPoint(i, sf::Vector2f(rect[i].x, rect[i].y));
    }

    shape.setFillColor(sf::Color::White);
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(5);

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::White);

        window.draw(shape);

        window.display();
    }
}
