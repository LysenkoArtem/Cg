#include <SFML/Graphics.hpp>
#include "cg.h"

#define WIDTH 1000
#define HEIGHT 1000
#define SCALE 300

sf::Vector2f transform_to_vis(cg::point_2 p) {
    return sf::Vector2f(SCALE*p.x + WIDTH/2.0, -SCALE*p.y + HEIGHT/2.0);
}

cg::point_2 transform_to_logic(float x, float y) {
    return {(x - WIDTH/2.0)/SCALE, (-y + HEIGHT/2.0)/SCALE};
}


void draw_poligon(cg::simple_polygon_2 poly, int mode) {
    sf::RenderWindow window(
        sf::VideoMode({WIDTH, HEIGHT}),
        "Calc geometry"
    );

    sf::ConvexShape shape;


    shape.setPointCount(poly.size());

    for (std::size_t i = 0; i < poly.size(); ++i)
        shape.setPoint(i, transform_to_vis(poly[i]));

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

                    if (mode == 1 && cg::point_in_simple_polygon(poly, transform_to_logic(click_x, click_y)))
                        point.setFillColor(sf::Color::Green);
                    else if (mode == 2 && cg::point_in_convex_polygon(poly, transform_to_logic(click_x, click_y)))
                        point.setFillColor(sf::Color::Green);
                    else if (mode == 3 && cg::point_in_convex_polygon_2(poly, transform_to_logic(click_x, click_y)))
                        point.setFillColor(sf::Color::Green);
                    else if (mode == 4 && cg::point_in_star_polygon(poly, transform_to_logic(click_x, click_y), {0, 0}))
                        point.setFillColor(sf::Color::Green);
                    else if (mode == 5) {
                        tangents = cg::find_tangetnts_optimal(poly, transform_to_logic(click_x, click_y));
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
            sf::Vector2f p1 = transform_to_vis(tangents.first);
            sf::Vector2f p2 = transform_to_vis(tangents.second);
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


void draw_points(cg::points_array points, sf::RenderWindow& window) {
    sf::CircleShape point(3, 30);
    point.setFillColor(sf::Color::Red);
    for (auto p: points) {
        sf::Vector2f coords = transform_to_vis(p);
        coords.x -= 3.f;
        coords.y -= 3.f;
        point.setPosition(coords);
        window.draw(point);
    }
}


void draw_block_2(cg::points_array points, cg::points_array border) {

    sf::RenderWindow window(
        sf::VideoMode({WIDTH, HEIGHT}),
        "Calc geometry"
    );

    sf::ConvexShape shape;


    shape.setPointCount(border.size());

    for (std::size_t i = 0; i < border.size(); ++i)
        shape.setPoint(i, transform_to_vis(border[i]));

    shape.setFillColor(sf::Color::White);
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(3);

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::White);


        window.draw(shape);
        draw_points(points, window);

        window.display();
    }
}
