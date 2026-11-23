#include "cg.h"
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <list>
#include <math.h>
#include <algorithm>
#include <optional>

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


// Block 2


points_array generate_points(int n) {
    srand((unsigned)time(0)); 
    points_array res;
    for (int i=0; i < n; i++) {
        float x = (float)((float) rand()/RAND_MAX)*2 - 1;
        float y = (float)((float) rand()/RAND_MAX)*2 - 1;
        res.push_back({x, y});
    }
    return res;
    
}


simple_polygon_2 convex_hull_graham(points_array const &points) {
point_2 bottom_left = points[0];
    for (auto p: points) {
        if (p.x < bottom_left.x)
            bottom_left = p;
        else if (p.x == bottom_left.x && p.y < bottom_left.y)
            bottom_left = p;
    }

    points_array sorted_points(points);

    std::sort(sorted_points.begin(), sorted_points.end(), 
        [bottom_left](point_2 const &a, point_2 const &b) {

            Orientation o = orientation(bottom_left, a, b);
            if (o == Orientation::left)
                return 1;
            if (o == Orientation::collinear)
                if (a.x < b.x || (a.x == b.x && a.y > b.y))
                    return 1;
            return 0;
        }
        );
    points_array hull;
    for (auto point: sorted_points) {
        while (hull.size() >= 2 && orientation(hull[hull.size()-2], hull[hull.size()-1], point) != Orientation::left) {
            hull.pop_back();
        }
        hull.push_back(point);
    }

    return hull;
}

simple_polygon_2 preparata_hong(points_array& points) {
    points_array sorted_points(points);
    std::sort(sorted_points.begin(), sorted_points.end(), 
        [](point_2 const &a, point_2 const &b) {
                return a.x < b.x;
        }
        );
    return prh_rec(sorted_points, 0, points.size());

}

simple_polygon_2 prh_rec(points_array& points, int start, int end) {
    points_array left_part;
    points_array right_part;

    if (end - start > 6) {
        left_part = prh_rec(points, start, (start+end)/2);
        right_part = prh_rec(points, (start+end)/2, end);

        return merge_points(left_part, right_part);
    }
    else {
        points_array np(points.begin()+start, points.begin()+end);
        return convex_hull_graham(np);
    }

}

simple_polygon_2 merge_points(simple_polygon_2 left, simple_polygon_2 right) {
    int lp_r = 0;
    double tmp_x = left[0].x;
    for (int i = 0; i < left.size(); ++i) {
        if (left[i].x > tmp_x) {
            lp_r = i;
            tmp_x = left[i].x;
        }
    }

    int rp_l = 0;
    tmp_x = right[0].x;
    for (int i = 0; i < right.size(); ++i) {
        if (right[i].x < tmp_x) {
            rp_l = i;
            tmp_x = right[i].x;
        }
    }

    //Верхняя касательная
    int upper_l = lp_r;
    int upper_r = rp_l;
    bool flag = 0;
    while (!flag) {
        flag = 1;
        while (orientation(right[upper_r], left[upper_l], left[(upper_l+1)%left.size()]) == Orientation::right) {
            upper_l = (upper_l+1) % left.size();
            flag = 0;
        }
        while (orientation(left[upper_l], right[upper_r], right[(right.size()+upper_r-1)%right.size()]) == Orientation::left) {
            upper_r = (right.size()+upper_r-1)%right.size();
            flag = 0;
        }
    }

    // Нaжняя кaсательная
    int lower_l = lp_r;
    int lower_r = rp_l;
    flag = 0;
    while (!flag) {
        flag = 1;
        while (orientation(right[lower_r], left[lower_l], left[(left.size() + lower_l-1)%left.size()]) == Orientation::left) {
            lower_l = (left.size()+lower_l - 1) % left.size();
            flag = 0;
        }
        while (orientation(left[lower_l], right[lower_r], right[(lower_r+1)%right.size()]) == Orientation::right) {
            lower_r = (lower_r+1) % right.size();
            flag = 0;
        }
    }

    points_array res;
    int idx = upper_l;
    res.push_back(left[upper_l]);
    while (idx != lower_l) {
        idx = (idx + 1) % left.size();
        res.push_back(left[idx]);
    }

    idx = lower_r;
    res.push_back(right[lower_r]);
    while (idx != upper_r) {
        idx = (idx + 1) % right.size();
        res.push_back(right[idx]);
    }

    return res;
}

triangulation_2 ear_clipping(const simple_polygon_2 &poly) {

    struct vertex_t;
    using vertex_list_t = std::list<vertex_t>;
    using vertex_handle_t = vertex_list_t::iterator;
    using concave_list_t = std::list<vertex_handle_t>;
    using ear_list_t = std::list<vertex_handle_t>;

    struct vertex_t {
        size_t index;
        point_2 coords;
        std::optional<concave_list_t::iterator> concave;
        std::optional<ear_list_t::iterator> ear;

    };

    triangulation_2 triangulation;
    auto output_triangle = [&triangulation](vertex_handle_t p0, vertex_handle_t p1, vertex_handle_t p2){
        triangulation.push_back({p0->index, p1->index, p2->index});
    };


    vertex_list_t vertex_list;
    concave_list_t concave_list;
    ear_list_t ears_list;

    auto cyclic_prev = [&vertex_list](vertex_handle_t p) {
        if (p == vertex_list.begin())
            return std::prev(vertex_list.end());
        return std::prev(p);
    };
    auto cyclic_next = [&vertex_list](vertex_handle_t p) {
        if (p == std::prev(vertex_list.end()))
            return  vertex_list.begin();
        return std::next(p);

    };

    auto is_concave = [&cyclic_prev, &cyclic_next](vertex_handle_t p) {
        return (orientation(cyclic_prev(p)->coords, p->coords, cyclic_next(p)->coords) == Orientation::right);
    };

    auto is_ear = [&concave_list, &cyclic_next, &cyclic_prev](vertex_handle_t p) {
        for (auto const &cp: concave_list)
            doSex();
    };

    for (size_t i=0; i < poly.size(); i++) {
        vertex_list.push_back({i, poly[i]});
    }

    for (auto p = vertex_list.begin(); p != vertex_list.end(); ++p) {
        if (is_concave(p)) {
            p->concave = concave_list.emplace(concave_list.end(), p);

        }
    }

    return {};

}


}
