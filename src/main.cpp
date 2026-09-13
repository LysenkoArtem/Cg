#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#define SCALE 100
#define SIZE 10

namespace cg {
    struct point2d {
        double x;
        double y;

        point2d operator+(const point2d& other) const {
            return {x + other.x, y + other.y};
        };
        point2d operator-(const point2d& other) const {
            return {x - other.x, y - other.y};
        };
        double operator*(const point2d& other) const {
            return (x * other.x + y * other.y);
        };
        point2d operator/(double d) const {
            return {x/d, y/d};
        };
    };
    
    using polygon2d = std::vector<point2d>;
};

int main() {

    sf::RenderWindow window(
        sf::VideoMode({SIZE*SCALE, SIZE*SCALE}),
        "Calc geometry"
    );

    cg::polygon2d rect;
    rect.push_back({2, 2});
    rect.push_back({2, 4});
    rect.push_back({3, 5});
    rect.push_back({4, 4});
    rect.push_back({4, 2});

    sf::ConvexShape shape;

    shape.setPointCount(rect.size());

    for (std::size_t i = 0; i < rect.size(); ++i)
    {
        shape.setPoint(i, sf::Vector2f(rect[i].x*SCALE, rect[i].y*SCALE));
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

    return 0;
}
