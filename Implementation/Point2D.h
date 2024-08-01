#pragma once

#ifndef POINT2D_H
#define POINT2D_H

#include "Settings.h"

#include <cmath>

class Point2D
{
public:
	double x;
	double y;

	Point2D();
	Point2D(double x, double y);
	Point2D(double angle);
	Point2D(sf::Vector2f p);
	Point2D(sf::Vector2i p);
	Point2D(sf::Vector2u p);
	
	static Point2D rad2Vect(double angle);
	double vect2Rad();

	sf::Vector2f to_sff() const;
	sf::Vector2i to_sfi() const;

	bool operator==(const Point2D& a) const;
	bool operator!=(const Point2D& a) const;

	Point2D operator+(const Point2D& a) const;
	Point2D operator+(double num) const;

	Point2D operator-(const Point2D& a) const;
	Point2D operator-(double num) const;

	Point2D operator*(double a) const;
	double operator*(Point2D p) const;

	Point2D operator/(double a) const;

	Point2D& operator=(const Point2D& a);
	Point2D& operator+=(const Point2D& a);
	Point2D& operator-=(const Point2D& a);
	Point2D& operator*=(double a);
	Point2D& operator/=(double a);

	double length();
	double norm_length();
	Point2D normalize();
	Point2D normalized();

	static double cross(const Point2D& a, const Point2D& b);
	static double dot(const Point2D& a, const Point2D& b);

	double cross(Point2D& p);
	double dot(Point2D& p);

	std::string to_str();
};

#endif