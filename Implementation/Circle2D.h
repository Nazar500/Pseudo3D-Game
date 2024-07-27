#pragma once

#ifndef CIRCLE2D_H
#define CIRCLE2D_H

#include "Settings.h"
#include "Point2D.h"
#include "Object2D.h"

class Circle2D : public Object2D
{
private:
	double d_radius;

protected:
	bool cross(const std::pair<Point2D, Point2D>& ray, std::pair<Point2D, Point2D>& wall, Point2D& point, double& uv) override;
	Point2D loc_middle() const override;

public:
	explicit Circle2D(double radius, Point2D position, double height = 1., const std::string& TEXTURE = COLUMN_TEXTURE, int conevexNum = CIRCLE_CONVEX_NUMBER);
	Circle2D() = default;

	ObjectType type() override;
};

#endif
