#include "cg.h"
#include <algorithm>
#include <cstdlib>
#include <math.h>

namespace cg {

double pseudoscalar(point_2 a, point_2 b) {
    return a.x * b.y - a.y * b.x;
}

Orientation orientation(point_2 a, point_2 b, point_2 c) {
    double psc = pseudoscalar(b - a, c - a);

    if (psc > 0)
        return Orientation::left;

    if (psc < 0)
        return Orientation::right;

    return Orientation::collinear;
}

bool is_convex(simple_polygon_2 const& poly) {
    int n = poly.size();

    for (int k = n - 2, j = n - 1, i = 0;
         i < n;
         k = j, j = i, ++i) {

        auto a = poly[k];
        auto b = poly[j];
        auto c = poly[i];

        if (orientation(a, b, c) != Orientation::left)
            return false;
    }

    return true;
}

simple_polygon_2 generate_convex_polygon(int n) {
    srand((unsigned)time(0)); 
    std::vector<int> points;
    while (points.size() < n) {
        int p = rand() % 360;
        if (std::find(points.begin(), points.end(), p) == points.end())
            points.push_back(p);
    }
    std::sort(points.begin(), points.end());

    simple_polygon_2 poly;
    for (int p: points) {
        poly.push_back({cos(p*M_PI/180), sin(p*M_PI/180)});
    }

    return poly;
}

simple_polygon_2 generate_star_polygon(int n) {
    srand((unsigned)time(0)); 
    std::vector<int> points;
    while (points.size() < n) {
        int p = rand() % 360;
        if (std::find(points.begin(), points.end(), p) == points.end())
            points.push_back(p);
    }
    std::sort(points.begin(), points.end());

    simple_polygon_2 poly;
    for (int p: points) {
        float r = (float)((float) rand()/RAND_MAX)/2 + 0.5;
        poly.push_back({cos(p*M_PI/180)*r , sin(p*M_PI/180)*r});
    }

    return poly;
}


bool is_point_in_simple_polygon(simple_polygon_2 poly, point_2 z) {
    int n = poly.size();
    bool l = false;
    for (int j=n-1, i=0; i < n; j=i, ++i) {
        point_2 b = poly[i].y < poly[j].y ? poly[i] : poly[j];
        point_2 t = poly[i].y > poly[j].y ? poly[i] : poly[j];
        if (b.y < z.y && t.y >= z.y && orientation(b, t, z) == Orientation::right) 
            l = !l;
    }
    return l % 2;
}

}
