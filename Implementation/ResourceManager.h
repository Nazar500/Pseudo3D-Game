#pragma once

#ifndef RESOURCEMANEGER_H
#define RESOURCEMANAGER_H

#include "Settings.h"
#include <SFML/Audio.hpp>
#include <map>
#include <memory>

using namespace std;

namespace ResourceManager
{
	namespace
	{
		map<string, shared_ptr<sf::Texture>> _textures;
		map<string, shared_ptr<sf::SoundBuffer>> _sounds;
		map<string, shared_ptr<sf::Font>> _fonts;
	}

	bool checkPath(const string& path);

	void unloadAllResources();
	void unloadTextures();
	void unloadSoundBuffers();
	void unloadFonts();

	sf::Texture* loadTexture(const string& filename);
	sf::SoundBuffer* loadSound(const string& filename);
	sf::Font* loadFont(const string& filename);
}

#endif 
