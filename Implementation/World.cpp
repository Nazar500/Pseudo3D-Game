#include "World.h"
#include "ResourceManager.h"

World::World(int length, int width, const std::string& sky_texture) : d_length(length), d_width(width)
{
	checkptr(T_sky_texture, ResourceManager::loadTexture(sky_texture));
}

void World::draw(sf::RenderTarget& window)
{
	sf::ConvexShape background;
	background.setPointCount(4);
	background.setPoint(0, sf::Vector2f{ 0, 0 });
	background.setPoint(1, sf::Vector2f{ SCALE_WINDOW * SCALE / MAP_SCALE, 0 });
	background.setPoint(2, sf::Vector2f{ SCALE_WINDOW * SCALE / MAP_SCALE,  SCALE_WINDOW * SCALE / MAP_SCALE });
	background.setPoint(3, sf::Vector2f{ 0,  SCALE_WINDOW * SCALE / MAP_SCALE });
	background.setFillColor(FILL_BACK_COLOR);
	background.setOutlineColor(OUTLINE_BACK_COLOR);
	background.setOutlineThickness(OUTLINE_BACK_THICKNESS);

	window.draw(background);

	for (auto p : map_objects)
		p.second->draw(window);
}

std::shared_ptr<Object2D> World::operator[](std::string& name)
{
	return findObject2D(name);
}

bool World::addObject2D(const std::shared_ptr<Object2D> object, const std::string& name) {
	if (name != "" && name != "None") {
		return map_objects.insert({ name, object }).second;
	}
	else {
		return map_objects.insert({ to_string(players++), object }).second;
	}
}

std::shared_ptr<Object2D> World::findObject2D(const std::string& name)
{
	for (auto& el : map_objects) {
		if(el.first == name)
			return el.second;
	}
	return nullptr;
}

std::string World::getObject2DName(const Object2D* ptr)
{
	for (auto& el : map_objects) {
		if (el.second.get() == ptr)
			return el.first;
	}
	return "None";
}

bool World::removeObject2D(const std::string& name) {
	if (map_objects[name] == nullptr) { return false; }
	return map_objects.erase(name) > 0;
}

std::map<std::string, std::shared_ptr<Object2D>> World::objects() {
	return map_objects;
}

const sf::Texture& World::skyTexture() const
{
	return T_sky_texture;
}

const sf::Texture& World::floorTexture() const
{
	return T_floor_texture;
}
