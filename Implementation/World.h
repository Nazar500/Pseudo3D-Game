#pragma once

#ifndef WORLD_H
#define WORLD_H

#include "Settings.h"
#include "Object2D.h"

#include <map>
#include <memory>

class World
{
private:

	std::map<std::string, std::shared_ptr<Object2D>> map_objects;

	unsigned char players = 0;

	sf::Texture T_sky_texture;
	sf::Texture T_floor_texture;

public: 
	World(const std::string& sky_texture = Settings::SKY_TEXTURE);

	bool addObject2D(const std::shared_ptr<Object2D>& object, const std::string& name);
	bool removeObject2D(const std::string& name);
	std::map<std::string, std::shared_ptr<Object2D>> objects();

	std::shared_ptr<Object2D> findObject2D(const std::string& name);
	std::string getObject2DName(const Object2D* ptr);
	
	void draw(sf::RenderTarget& window);
	const sf::Texture& skyTexture() const;
	const sf::Texture& floorTexture() const;

	std::shared_ptr<Object2D> operator [](std::string& name);
};


#endif
