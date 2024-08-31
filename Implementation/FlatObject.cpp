#include "FlatObject.h"

using namespace Settings;

FlatObject::FlatObject(Point2D pos, string f_texture_path, double scale_x, double height) : Object2D(pos, {}, height, 0, f_texture_path, 1), f_scale(scale_x)
{
	this->p_points = { {T_texture.getSize().x / -2. * scale_x, 0}, {T_texture.getSize().x / 2. * scale_x, 0} };
}

FlatObject::FlatObject(FlatObject& other) : Object2D(other)
{
	this->f_scale = other.f_scale;
}

void FlatObject::draw(sf::RenderTarget& window)
{
	//rotation(d_rotation + PI / 2.);
}

void FlatObject::update(double cam_angle)
{
	d_rotation = cam_angle;
	rotation(d_rotation + PI / 2.);
}
