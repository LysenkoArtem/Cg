#include <SFML/Graphics.hpp>
#include <cassert>
#include <iostream>
#include <vector>

#define SCALE 100
#define SIZE 10

namespace cg {
// Begin of namespace

struct point_2 {
    double x;
    double y;

    point_2(double X, double Y) {
        x = X;
        y = Y;
    }

    point_2() {
        x = 0;
        y = 0;
    }

    point_2 operator+(const point_2& other) const {
        return {x + other.x, y + other.y};
    };
    point_2 operator-(const point_2& other) const {
        return {x - other.x, y - other.y};
    };
    double operator*(const point_2& other) const {
        return (x * other.x + y * other.y);
    };
    point_2 operator/(double d) const {
        return {x/d, y/d};
    };
};
using simple_polygon_2 = std::vector<point_2>;

double pseudoscalar(point_2 a, point_2 b) {
    return a.x*b.y - a.y*b.x;
}

enum class Orientation {
    left,
    right,
    collinear
};

Orientation orientation(point_2 a, point_2 b, point_2 c) {
    double psc = pseudoscalar(b-a, c-a);
    if (psc > 0) 
        return Orientation::left;
    if (psc < 0)
        return Orientation::right;
    else
        return Orientation::collinear;
}

bool is_convex(simple_polygon_2 const & poly) {
    int n = poly.size();
    for (int k=n-2, j = n-1, i=0; i < n; k=j, j=i, ++i) {
        auto a = poly[k];
        auto b = poly[j];
        auto c = poly[i];
        if (orientation(a, b, c) != Orientation::left) 
            return false;
    }
    return true;
}

};
// End of namespace


void visualize(cg::simple_polygon_2 rect) {
    sf::RenderWindow window(
        sf::VideoMode({SIZE*SCALE, SIZE*SCALE}),
        "Calc geometry"
    );

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
}

int main() {

    cg::point_2 a(0, 0);
    cg::point_2 b(100, 0);
    cg::point_2 c(100, 100);
    assert(cg::orientation(a, b, c) == cg::Orientation::left);

    cg::simple_polygon_2 poly;
    poly.push_back({0, 0});
    poly.push_back({100, 0});
    poly.push_back({100, 100});
    poly.push_back({0, 100});

    std::cout << cg::is_convex(poly);


    return 0;
}
