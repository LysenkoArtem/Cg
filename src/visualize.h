#ifndef VISUALIZE_H
#define VISUALIZE_H

#include "cg.h"
#include <SFML/Graphics.hpp>

void draw_poligon(cg::simple_polygon_2 poly, int mode);
void draw_points(cg::points_array points, sf::RenderWindow& window);
void draw_block_2(cg::points_array points, cg::points_array border);

#endif
