#pragma once

#ifndef OBJECT2D_H
#define OBJECT2D_H

#include "Settings.h"
#include "Point2D.h"
#include "ObjectType.h"
#include "Idrawable.h"
#include "ResourceManager.h"

class Object2D : public Idrawable
{
protected:
	std::vector<Point2D> p_s_points;
	std::vector<Point2D> p_points;
	Point2D p_pos;

	sf::Texture T_texture;
	string T_texture_path;
	bool whole_texture_overlay, b_mirror;

	double d_height, d_aspectRatio = 0.2, l_width = 0, l_length = 0;

public:
	Object2D(const Object2D& object2D);
	explicit Object2D(Point2D pos, std::vector<Point2D> points, double height = 1., bool mirror = false, const std::string& TEXTURE = WALL_TEXTURE, bool all_texture=false);

	void draw(sf::RenderTarget& window) override;
	const sf::Texture& loadTexture() const;
	const string loadTexturePath() const;
	virtual ObjectType type();

	std::vector<Point2D> world_nodes(double number);
	void rotate(double angle);
	void rotation(double angle);

	[[nodiscard]] double x() const;
	[[nodiscard]] double y() const;
	[[nodiscard]] Point2D position() const;
	void translate(Point2D vec);
	void translate(double x, double y);

	[[nodiscard]] double width() const;
	[[nodiscard]] double length() const;
	[[nodiscard]] double height() const;
	void setHeight(double height);

	// middle point in local coordinates system
	[[nodiscard]] virtual Point2D loc_middle() const;

	[[nodiscard]] double aspect() const {
		return d_aspectRatio;
	}

	bool isMirror() const;
	void setMirror(bool value);

	std::vector<Point2D>& nodes();
	std::vector<Point2D>& nodess();

	static bool segment_crossing(const std::pair<Point2D, Point2D> seg1, const std::pair<Point2D, Point2D> seg2, Point2D& point);
	virtual bool cross(const std::pair<Point2D, Point2D>& ray, std::pair<Point2D, Point2D>& wall, Point2D& point, double& uv);
};

#endif