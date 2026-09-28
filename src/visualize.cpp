#include <SFML/Graphics.hpp>
#include <iostream>
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

    float click_x = 0;
    float click_y = 0;
    sf::Color point_color = sf::Color::Green;

    sf::CircleShape point(5, 5);
    point.setFillColor(sf::Color::Green);
    point.setPosition({WIDTH/2, HEIGHT/2});

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePressed->button == sf::Mouse::Button::Left) {
                    click_x = mousePressed->position.x;
                    click_y = mousePressed->position.y;

                    if (cg::point_in_simple_polygon(poly, {(click_x - WIDTH/2)/SCALE, (click_y - HEIGHT/2)/SCALE}))
                        point.setFillColor(sf::Color::Green);
                    else
                        point.setFillColor(sf::Color::Red);
                    point.setPosition({click_x-4.5f, click_y-4.5f});
                    
                }
            }
        }

        window.clear(sf::Color::White);

        //window.draw(circ);
        window.draw(shape);
        window.draw(point);


        window.display();
    }
}
