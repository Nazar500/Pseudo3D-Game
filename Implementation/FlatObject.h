#pragma once

#ifndef FLATOBJECT_H
#define FLATOBJECT_H

#include "Settings.h"
#include "Object2D.h"
#include "ResourceManager.h"

// 2d sprite
class FlatObject : public Object2D
{
private:
	double f_scale;

public:
	explicit FlatObject(Point2D pos, string f_texture_path = BALDI, double scale_x = 1., double height = 1.);
	FlatObject(FlatObject& other);

	void draw(sf::RenderTarget& window) override;
	void update(double cam_agnle);
};

#endif