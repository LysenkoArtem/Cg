#include "cg.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <algorithm>

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


bool point_in_simple_polygon(simple_polygon_2& poly, point_2 z) {
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

bool point_in_star_polygon(simple_polygon_2& poly, point_2 z, point_2 kernel_point)  {
    int p_s = 0;
    int p_end = poly.size();
    int p_m = p_end / 2;
    while (p_end - p_s > 1) {
        p_m = (p_end + p_s) / 2;
        if (orientation(kernel_point, poly[p_s], poly[p_m]) == Orientation::left)
            if (orientation(kernel_point, poly[p_s], z) != Orientation::right && orientation(kernel_point, poly[p_m], z) == Orientation::right) {
                p_end = p_m;
            }
            else {
                p_s = p_m;
            }
            else {
                if (orientation(kernel_point, poly[p_s], z) == Orientation::right && orientation(kernel_point, poly[p_m], z) != Orientation::right) {
                    p_s = p_m;
                }
                else {
                    p_end = p_m;
                }
            }
    }
    return (orientation(poly[p_s], poly[(p_s+1)%poly.size()], z) == Orientation::left);
}

bool point_in_convex_polygon(simple_polygon_2& poly, point_2 z) {
    return point_in_star_polygon(poly, z, (poly[0]+poly[1]+poly[2])/3.0);
}

//struct convex_pred_t {
//    bool operator()(point_2 p) const {
//        return orientation(p0, p, z) == Orientation::left;
//
//    }
//    point_2 p0;
//    point_2 z;
//};

bool point_in_convex_polygon_2(simple_polygon_2& poly, point_2 z) {
    auto partition = std::partition_point(poly.begin()+1, poly.end(),
                    [p0 = poly[0], z](point_2 p) {return orientation(p0, p, z) == Orientation::left;});

    if (partition == poly.begin()+1 || partition == poly.end())
        return false;
    return (orientation(*(partition-1), *(partition), z) == Orientation::left);
}

polygon_tangents_2 find_tangetnts_brute_force(simple_polygon_2& poly, point_2 z) {
    if (point_in_convex_polygon(poly, z)) {
        return {z, z};
    }
    polygon_tangents_2 res = {poly[0], poly[0]};
    for (int i=1; i < poly.size(); i++) {
        if (orientation(z, res.first, poly[i]) == Orientation::left)
            res.first = poly[i];
        if (orientation(z, res.second, poly[i]) == Orientation::right)
            res.second = poly[i];
    }

    return res;
}

polygon_tangents_2 find_tangetnts_optimal(simple_polygon_2& poly, point_2 z) {
    if (point_in_convex_polygon(poly, z)) {
        return {z, z};
    }
    polygon_tangents_2 res{z, z};
    auto partition = std::partition_point(poly.begin(), poly.end(),
                    [p0 = poly[0], orient = orientation(z, poly[0], poly[1]), z](point_2& p) {return orientation(z, p0, p) == orient;});


    res.first = *(std::partition_point(poly.begin()+1, partition,
                    [orient = orientation(z, poly[0], poly[1]), z](point_2& p) {return orientation(z, *(&p - 1), p) == orient;}) - 1);

    if (partition == poly.end())
        res.second = poly[0];
    else
        res.second = *(std::partition_point(partition+1, poly.end(),
                        [orient = orientation(z, poly[0], poly[1]), z](point_2& p) {return orientation(z, *(&p - 1), p) != orient;}) - 1);

    return res;
}

}
