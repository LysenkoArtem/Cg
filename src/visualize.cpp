#include <SFML/Graphics.hpp>
#include "cg.h"

#define WIDTH 1000
#define HEIGHT 1000
#define SCALE 300

void draw_poligon(cg::simple_polygon_2 poly) {
    sf::RenderWindow window(
        sf::VideoMode({WIDTH, HEIGHT}),
        "Calc geometry"
    );

    sf::ConvexShape shape;


    shape.setPointCount(poly.size());

    for (std::size_t i = 0; i < poly.size(); ++i)
    {
        shape.setPoint(i, sf::Vector2f(SCALE*poly[i].x + WIDTH/2, SCALE*poly[i].y + HEIGHT/2));
    }

    shape.setFillColor(sf::Color::White);
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(3);

   //sf::CircleShape circ(SCALE, 500);
   //circ.setPosition({WIDTH/2 - SCALE, HEIGHT/2 - SCALE});
   //circ.setOutlineColor(sf::Color::Red);
   //circ.setOutlineThickness(3);

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::White);

        //window.draw(circ);
        window.draw(shape);


        window.display();
    }
}
