#pragma once

#ifndef SQUARE2D_H
#define SQUARE2D_H

#include "Settings.h"
#include "Object2D.h"
#include "Point2D.h"

class Square2D : public Object2D {
private:
	double s_size;

public:
	Square2D(const Point2D& pos, double size = SCALE, double h_height = 1., const std::string& texture = SKIN) : s_size(size), Object2D(pos, vector<Point2D>(4, 0), h_height, false, texture, true) {
		p_s_points[0] = Point2D(-s_size / 2., -s_size / 2.);
		p_s_points[1] = Point2D(s_size / 2., -s_size / 2.);
		p_s_points[2] = Point2D(s_size / 2, s_size / 2.);
		p_s_points[3] = Point2D(-s_size / 2., s_size / 2.);

		p_points = p_s_points;
	}
};

#endif