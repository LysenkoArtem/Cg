#include "cg.h"

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

}
