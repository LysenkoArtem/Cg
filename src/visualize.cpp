#include <SFML/Graphics.hpp>
#include "cg.h"

#define WIDTH 1000
#define HEIGHT 1000
#define SCALE 300

void draw_poligon(cg::simple_polygon_2 poly, int mode) {
    sf::RenderWindow window(
        sf::VideoMode({WIDTH, HEIGHT}),
        "Calc geometry"
    );

    sf::ConvexShape shape;


    shape.setPointCount(poly.size());

    for (std::size_t i = 0; i < poly.size(); ++i)
    {
        shape.setPoint(i, sf::Vector2f(SCALE*poly[i].x + WIDTH/2.0, -SCALE*poly[i].y + HEIGHT/2.0));
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

    cg::polygon_tangents_2 tangents = {{WIDTH/2.0, HEIGHT/2.0}, {WIDTH/2.0, HEIGHT/2.0}};

    sf::CircleShape point(5, 5);
    point.setFillColor(sf::Color::Green);
    point.setPosition({WIDTH/2.0, HEIGHT/2.0});

    sf::Vector2f p0;
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

                    if (mode == 1 && cg::point_in_simple_polygon(poly, {(click_x - WIDTH/2.0)/SCALE, (-click_y + HEIGHT/2.0)/SCALE}))
                        point.setFillColor(sf::Color::Green);
                    else if (mode == 2 && cg::point_in_convex_polygon(poly, {(click_x - WIDTH/2.0)/SCALE, (-click_y + HEIGHT/2.0)/SCALE}))
                        point.setFillColor(sf::Color::Green);
                    else if (mode == 3 && cg::point_in_convex_polygon_2(poly, {(click_x - WIDTH/2.0)/SCALE, (-click_y + HEIGHT/2.0)/SCALE}))
                        point.setFillColor(sf::Color::Green);
                    else if (mode == 4 && cg::point_in_star_polygon(poly, {(click_x - WIDTH/2.0)/SCALE, (-click_y + HEIGHT/2.0)/SCALE}, {0, 0}))
                        point.setFillColor(sf::Color::Green);
                    else if (mode == 5) {
                        tangents = cg::find_tangetnts_optimal(poly, {(click_x - WIDTH/2.0)/SCALE, (-click_y + HEIGHT/2.0)/SCALE});
                        p0 = {click_x, click_y};
                    }

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
        if (mode == 5) {
            sf::Vector2f p1({(float)(SCALE*tangents.first.x + WIDTH/2.0), (float)(-SCALE*tangents.first.y + HEIGHT/2.0)});
            sf::Vector2f p2({(float)(SCALE*tangents.second.x + WIDTH/2.0), (float)(-SCALE*tangents.second.y + HEIGHT/2.0)});
            std::vector<sf::Vertex> line1 = {
                sf::Vertex{p0, sf::Color::Black},
                sf::Vertex{p1, sf::Color::Black}
            };

            std::vector<sf::Vertex> line2 = {
                sf::Vertex{p0, sf::Color::Black},
                sf::Vertex{p2, sf::Color::Black}
            };
            window.draw(line1.data(), line1.size(), sf::PrimitiveType::Lines);
            window.draw(line2.data(), line2.size(), sf::PrimitiveType::Lines);
        }


        window.display();
    }
}
