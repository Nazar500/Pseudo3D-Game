#include "ResourceManager.h"

namespace ResourceManager {
	bool checkPath(const string& path)
	{
		for (auto& el : _textures) {
			if (el.first != path && el.first != (SKY_TEXTURE)) {
				return false;
			}
		}
		return true;
	}

	void unloadAllResources() {
		unloadTextures();
		unloadSoundBuffers();
		unloadFonts();
	}

	void unloadTextures()
	{
		for (auto it = _textures.begin(); it != _textures.end(); it++) {
			it->second.reset();
		}
		_textures.clear();
	}

	void unloadSoundBuffers()
	{
		for (auto it = _sounds.begin(); it != _sounds.end(); it++) {
			it->second.reset();
		}
		_sounds.clear();
	}

	void unloadFonts() {
		for (auto it = _fonts.begin(); it != _fonts.end(); it++) {
			it->second.reset();
		}
		_fonts.clear();
	}

	sf::Texture* loadTexture(const string& filename)
	{
		auto it = _textures.find(filename);
		if (it != _textures.end()) {
			return it->second.get();
		}

		shared_ptr<sf::Texture> texture(new sf::Texture);
		if (!texture->loadFromFile(filename)) {
			return nullptr;
		}

		texture->setRepeated(true);
		_textures.emplace(filename, texture);

		return texture.get();
	}

	sf::SoundBuffer* loadSound(const string& filename) {
		auto it = _sounds.find(filename);
		if (it != _sounds.end()) {
			
			return it->second.get();
		}

		shared_ptr<sf::SoundBuffer> sound(new sf::SoundBuffer);
		if (!sound->loadFromFile(filename)) {
			return nullptr;
		}

		_sounds.emplace(filename, sound);

		return sound.get();
	}
	
	sf::Font* loadFont(const string& filename) {
		auto it = _fonts.find(filename);
		if (it != _fonts.end()) {
			return it->second.get();
		}

		shared_ptr<sf::Font> font(new sf::Font);
		if (!font->loadFromFile(filename)) {
			return nullptr;
		}

		_fonts.emplace(filename, font);

		return font.get();
	}
}