#include "FlatObject.h"

FlatObject::FlatObject(Point2D pos, string f_texture_path, double scale_x, double height) : Object2D(pos, {}, height, 0, f_texture_path, 1), f_scale(scale_x), f_camera_angle(0)
{
	this->p_points = { {T_texture.getSize().x / -2. * scale_x, 0}, {T_texture.getSize().x / 2. * scale_x, 0} };
}

FlatObject::FlatObject(FlatObject& other) : Object2D(other)
{
	this->f_scale = other.f_scale;
	this->f_camera_angle = other.f_camera_angle;
}

void FlatObject::draw(sf::RenderTarget& window)
{
	rotation(f_camera_angle+PI/2);
}

void FlatObject::update(double cam_angle)
{
	f_camera_angle = cam_angle;
}
