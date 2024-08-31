#include "Object2D.h"

using namespace Settings;

Object2D::Object2D(const Object2D& object2D)
{
	this->p_pos.x = object2D.p_pos.x;
	this->p_pos.y = object2D.p_pos.y;

	this->p_points = object2D.p_points;
	this->p_s_points = object2D.p_points;
	this->d_height = object2D.d_height;
	this->d_rotation = object2D.d_rotation;

	this->T_texture = object2D.T_texture; 
	this->T_texture1 = object2D.T_texture1; 
	this->T_texture_path = object2D.T_texture_path;
	this->T_texture1_path = object2D.T_texture1_path;
	this->whole_texture_overlay = object2D.whole_texture_overlay;
	this->b_mirror = object2D.b_mirror;

	this->l_width = (*max_element(object2D.p_points.begin(), object2D.p_points.end(), [](const Point2D& p1, const Point2D& p2) { return p1.x < p2.x; })).x;
	this->l_length = (*max_element(object2D.p_points.begin(), object2D.p_points.end(), [](const Point2D& p1, const Point2D& p2) { return p1.y < p2.y; })).y;
}

Object2D::Object2D(Point2D pos, std::vector<Point2D> points, double height, bool mirror, const std::string& TEXTURE, bool all_texture) : p_points(points), p_s_points(points), p_pos(pos), d_height(height), T_texture_path(TEXTURE), whole_texture_overlay(all_texture), b_mirror(mirror), d_rotation(0) {
	checkptr(T_texture, ResourceManager::loadTexture(TEXTURE));
	checkptr(T_texture1, ResourceManager::loadTexture(TEXTURE));
}


void Object2D::draw(sf::RenderTarget& window)
{
	sf::ConvexShape polygon;
	polygon.setPointCount(nodes().size());
	for (int i = 0; i < nodes().size(); i++) {
		polygon.setPoint(i, (p_points[i] / MAP_SCALE).to_sff());
	}
	polygon.setFillColor(FILL_COLOR);
	polygon.setOutlineThickness(OUTLINE_THICKNESS / MAP_SCALE);
	polygon.setOutlineColor(OUTLINE_COLOR);
	polygon.setPosition((p_pos / MAP_SCALE).to_sff());

	window.draw(polygon);
}

const sf::Texture& Object2D::loadTexture() const
{
	return T_texture;
}

void Object2D::setTexture1(const std::string& TEXTURE1)
{
	this->T_texture1_path = TEXTURE1;
	checkptr(T_texture1, ResourceManager::loadTexture(TEXTURE1));
}

const sf::Texture& Object2D::loadTexture1() const
{
	return T_texture1;
}

const string& Object2D::loadTexturePath() const
{
	return T_texture_path;
}

const string& Object2D::loadTexturePath1() const
{
	return T_texture1_path;
}


ObjectType Object2D::type()
{
	return ObjectType::object;
}

Point2D Object2D::position() const
{
	return p_pos;
}

void Object2D::translate(Point2D vec)
{
	p_pos += vec;
}

void Object2D::translate(double x, double y)
{
	p_pos += Point2D(x, y);
}

double Object2D::width() const
{
	return l_width;
}

double Object2D::length() const
{
	return l_length;
}

Point2D Object2D::loc_middle() const
{
	return Point2D(l_width / 2, l_length / 2);
}

bool Object2D::isMirror() const
{
	return b_mirror;
}

void Object2D::setMirror(bool value)
{
	b_mirror = value;
}

std::vector<Point2D>& Object2D::nodes() { return p_points; }

std::vector<Point2D>& Object2D::nodess()
{
	return p_s_points;
}


double Object2D::x() const { return p_pos.x; }
double Object2D::y() const { return p_pos.y; }

bool sign(double number) {
	return number >= 0 ? 1 : 0;
}

bool Object2D::segment_crossing(const std::pair<Point2D, Point2D> seg1, const std::pair<Point2D, Point2D> seg2, Point2D& point) {
	Point2D cut1 = seg1.second - seg1.first;
	Point2D cut2 = seg2.second - seg2.first;

	//std::cout << "Cut1: " << cut1.x << " " << cut1.y << '\n' << "Cut2: " << cut2.x << " " << cut2.y << '\n';
	//std::cout << "FF: " << (seg2.first - seg1.first).to_str() << '\n' << "SF: " << (seg2.second - seg1.first).to_str() << '\n';

	double prod1, prod2;

	prod1 = Point2D::cross(cut1, (seg2.first - seg1.first));
	prod2 = Point2D::cross(cut1, (seg2.second - seg1.first));

	//std::cout << "Prod1: " << prod1 << '\n' << "Prod2: " << prod2 << '\n';

	if (sign(prod1) == sign(prod2) || (prod1 == 0) || (prod2 == 0)) { return false; }

	prod1 = Point2D::cross(cut2, (seg1.first - seg2.first));
	prod2 = Point2D::cross(cut2, (seg1.second - seg2.first));

	//std::cout << "Prod1: " << prod1 << '\n' << "Prod2: " << prod2 << '\n';

	if (sign(prod1) == sign(prod2) || (prod1 == 0) || (prod2 == 0)) { return false; }

	point.x = seg1.first.x + cut1.x * std::abs(prod1) / std::abs(prod2 - prod1);
	point.y = seg1.first.y + cut1.y * std::abs(prod1) / std::abs(prod2 - prod1);

	return true;
}

bool Object2D::cross(const std::pair<Point2D, Point2D>& ray, std::pair<Point2D, Point2D>& wall, Point2D& point, double& uv)
{
	Point2D CrossPoint = { 0, 0 };
	std::pair<Point2D, Point2D> seg = { p_pos + p_points.back(), p_pos + p_points.front() };
	std::vector<std::pair<Point2D, std::pair<Point2D, Point2D>>> items;
	bool s = false;
	for (size_t i = 0; i < p_points.size(); i++) {
		if (segment_crossing(ray, seg, CrossPoint)) {
			s = true;
			items.emplace_back(CrossPoint, seg);
		}
		if (i != p_points.size() - 1) {
			seg = { p_pos + p_points[i], p_pos + p_points[i + 1] };
		}

		if (p_points.size() == 2) { break; }
	}

	if(!items.empty())
	{
		std::sort(items.begin(), items.end(), [ray](std::pair<Point2D, std::pair<Point2D, Point2D>> p, std::pair<Point2D, std::pair<Point2D, Point2D>> p1) { return (p.first - ray.first).length() < (p1.first - ray.first).length(); });
		point = items[0].first;
		wall = items[0].second;
	}
	if (s) {
		Point2D texture_size = this->loadTexture().getSize();

		double dist_of_point = (wall.second - point).length();
		double dist_of_wall = (ResourceManager::checkPath(MAIN_TEXTURE) || whole_texture_overlay) ? (wall.second - wall.first).length() : texture_size.x;

		uv = fmod(dist_of_point / dist_of_wall, 1.f);
	}
	return s;
}

std::vector<Point2D> Object2D::world_nodes(double number) {
	std::vector<Point2D> points;
	for (auto el : nodes()) {
		points.push_back(el /= number);
	}
	return points;
}

void Object2D::rotate(double angle)
{
	d_rotation += angle;
	double cosA = cos(angle);
	double sinA = sin(angle);

	for (auto& p : nodes()) {
		double oldX = p.x;
		double oldY = p.y;

		p.x = cosA * oldX - sinA * oldY;
		p.y = sinA * oldX + cosA * oldY;
	}
}

void Object2D::rotation(double angle)
{
	d_rotation = angle;
	Point2D vec(d_rotation);

	for (int i = 0; i < nodes().size(); i++) {
		p_points[i].x = p_s_points[i].x * cos(angle) - p_s_points[i].y * sin(angle);
		p_points[i].y = p_s_points[i].x * sin(angle) + p_s_points[i].y * cos(angle);
	}
}

double Object2D::getRotation() const
{
	return d_rotation;
}

double Object2D::height() const
{
	return d_height;
}

void Object2D::setHeight(double height)
{
	d_height = height;
}
