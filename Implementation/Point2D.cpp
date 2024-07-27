#include "Point2D.h"


Point2D::Point2D(double x, double y)
{
    this->x = x;
	this->y = y;
}

Point2D::Point2D(double angle)
{
    *this = rad2Vect(angle);
}

Point2D::Point2D(sf::Vector2f p)
{
    this->x = p.x;
    this->y = p.y;
}

Point2D::Point2D(sf::Vector2i p)
{
    this->x = p.x;
    this->y = p.y;
}

Point2D::Point2D(sf::Vector2u p)
{
    this->x = p.x;
    this->y = p.y;
}

Point2D Point2D::rad2Vect(double angle)
{
    return Point2D{ cos(angle), sin(angle) };
}

double Point2D::vect2Rad()
{
    return atan2(this->y, this->x);
}

sf::Vector2f Point2D::to_sff() const {
    return sf::Vector2f((float)this->x, (float)this->y);
}

sf::Vector2i Point2D::to_sfi() const {
    return sf::Vector2i((int)this->x, (int)this->y);
}

Point2D Point2D::operator+(const Point2D& a) const {
	return Point2D{ this->x + a.x, this->y + a.y };
}
Point2D Point2D::operator+(double num) const
{
    return Point2D{ this->x + num, this->y + num };
}

Point2D Point2D::operator-(const Point2D& a) const {
	return Point2D{ this->x - a.x, this->y - a.y };
}
Point2D Point2D::operator-(double num) const {
    return Point2D{ this->x - num, this->y - num };
}

Point2D Point2D::operator*(double a) const {
	return Point2D{ this->x * a, this->y * a };
}
double Point2D::operator*(Point2D p) const
{
    return x * p.x + y * p.y;
}

Point2D Point2D::operator/(double a) const {
	return Point2D{ this->x / a, this->y / a };
}

Point2D& Point2D::operator=(const Point2D& a) {
	this->x = a.x;
	this->y = a.y;
	return *this;
}

Point2D& Point2D::operator+=(const Point2D& point2D)
{
    this->x += point2D.x;
    this->y += point2D.y;
    return *this;
}

Point2D& Point2D::operator-=(const Point2D& point2D)
{
    this->x -= point2D.x;
    this->y -= point2D.y;
    return *this;
}

Point2D& Point2D::operator*=(double a)
{
    this->x *= a;
    this->y *= a;
    return *this;
}

Point2D& Point2D::operator/=(double a)
{
    this->x /= a;
    this->y /= a;
    return *this;
}

double Point2D::length()
{
    return sqrt(this->x * this->x + this->y * this->y);
}

double Point2D::norm_length()
{
    return (x >= 0 && y >= 0) ? sqrt(this->x * this->x + this->y * this->y) : -sqrt(this->x * this->x + this->y * this->y);
}

Point2D Point2D::normalize()
{  
    return *this /= std::max(length(), (double)1);
}

Point2D Point2D::normalized()
{
    return *this / std::max(length(), (double)1);
}


double Point2D::cross(const Point2D& a, const Point2D& b)
{
    return a.x * b.y - a.y * b.x;
}

double Point2D::dot(const Point2D& a, const Point2D& b)
{
    return a.x * b.x + a.y * b.y;
}

double Point2D::dot(Point2D & a)
{
    return this->x * a.x + this->y * a.y;
}

std::string Point2D::to_str()
{
    return std::to_string(this->x) + " " + std::to_string(this->y);
}

double Point2D::cross(Point2D& a)
{
    return this->x * a.y - this->y * a.x;
}


