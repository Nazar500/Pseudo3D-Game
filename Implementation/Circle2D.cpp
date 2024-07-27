#include "Circle2D.h"

Circle2D::Circle2D(double radius, Point2D position, double height, const std::string& TEXTURE, int convexNum) : d_radius(radius), Object2D(position, {}, height, 0, TEXTURE)
{
    for (int i = 0; i < convexNum; i++) {
        double x = static_cast<double>(d_radius * cos(2 * PI * i / convexNum));
        double y = static_cast<double>(d_radius * sin(2 * PI * i / convexNum));

		nodes().push_back(Point2D(x, y));
	}
}

ObjectType Circle2D::type()
{
    return ObjectType::circle;
}

bool Circle2D::cross(const std::pair<Point2D, Point2D>& ray, std::pair<Point2D, Point2D>& wall, Point2D& point, double& uv)
{
    bool success = false;
    Point2D d = ray.second - ray.first;
    Point2D f = ray.first - p_pos;
    double a = static_cast<double>(d.x * d.x + d.y * d.y);
    double b = static_cast<double>(2 * (f.x * d.x + f.y * d.y));
    double c = static_cast<double>((f.x * f.x + f.y * f.y) - d_radius * d_radius);

    double discriminant = b * b - 4 * a * c;
    if (discriminant >= 0)
    {
        discriminant = sqrt(discriminant);
        double t1 = (-b - discriminant) / (2 * a);
        double t2 = (-b + discriminant) / (2 * a);

        if (t1 >= 0 && t1 <= 1)
        {
            point.x = ray.first.x + t1 * d.x;
            point.y = ray.first.y + t1 * d.y;
            success = true;
        }
        else if (t2 >= 0 && t2 <= 1)
        {
            point.x = ray.first.x + t2 * d.x;
            point.y = ray.first.y + t2 * d.y;
            success = true;
        }
    }
    if (success)
    {
        double cx = cos(PI / 4), cy = sin(PI / 4);
        Point2D diff = point - p_pos;
        double angle = atan2((cx * diff.y - cy * diff.x) / d_radius, (cx * diff.x + cy * diff.y) / d_radius);
        wall.first = { p_pos.x + diff.y, p_pos.y - diff.x };
        wall.second = { p_pos.x - diff.y, p_pos.y + diff.x };

        uv = fmod(1 - angle / PI, 1.f);
    }
    return success;
}

Point2D Circle2D::loc_middle() const
{
    return { 0, 0 };
}


