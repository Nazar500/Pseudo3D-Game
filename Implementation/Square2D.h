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
	explicit Square2D(Point2D& pos, double size = SCALE, double h_height = 1., const std::string& texture=SKIN) : s_size(size), Object2D(pos, { 0, 0, 0, 0 }, h_height, 0, texture, 1) {
		p_points[0] = Point2D(-s_size / 2, -s_size / 2);
		p_points[1] = Point2D(s_size / 2, -s_size / 2);
		p_points[2] = Point2D(s_size / 2, s_size / 2);
		p_points[3] = Point2D(-s_size / 2, s_size / 2);
	}
};

#endif