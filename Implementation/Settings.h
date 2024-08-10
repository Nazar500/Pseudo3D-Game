#pragma once

#ifndef PSEUDO3DENGINE_SETTINGS_H
#define PSEUDO3DENGINE_SETTINGS_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <string>
#include <cmath>
#include <filesystem>
#include <chrono>
#include <cfenv>

#define PI 3.1415926535897932384626433832795
#define ET std::chrono::duration<double>(std::chrono::steady_clock::now().time_since_epoch()).count()

#define SCALE 40 //How much pixels = 1 unit of distance measure
#define MAP_SCALE ((MIRROR_DEBUG) ? 3 : 10)

#define SCALE_WINDOW 50

#define SIDE SCALE_WINDOW * SCALE

// SCREEN SIZE
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define TITLE "SFML Pseudo3D"

// NETWORK
#define NETWORK_VERSION 2U
#define TIMEOUT_SECONDS 5
#define WORLD_UPDATE_RATE 30
#define RELIABLE_RETRY_TIME (1.0/20)

// OBJECTS
#define OUTLINE_THICKNESS 10
#define OUTLINE_COLOR {255, 255, 255, 255}
#define FILL_COLOR {255, 174, 174, 255}

#define CIRCLE_CONVEX_NUMBER 50

// CAMERA
#define OUTLINE_CAMERA_THICKNESS 1
#define OUTLINE_CAMERA_COLOR {255, 255, 255, 255}
#define FILL_CAMERA_COLOR {255, 67, 67, 255}
#define RADIUS_CAMERA SCALE * 2
#define CONVEX_NUMBER 100
#define FILED_OF_VEW_COLOR {255, 189, 51, 0}
#define FILED_OF_VEW_OUTLINE_COLOR {0, 0, 0, 255}
#define RED_COLOR {255, 0, 0}

#define FOV PI / 2
#define CORRECTION 0

// FOR COLLISION DETECTION
#define COLLISION_DISTANCE 80
#define COLLISION_AREA 400
#define COLLISION_SEGMENTS (int)(max((DISTANCES_SEGMENTS / 50.), 1.))
#define HIDDEN_SEGMENTS (int)(COLLISION_SEGMENTS * (2 * PI - d_fieldOfView) / (2 * PI))
#define SHOWN_SEGMENTS COLLISION_SEGMENTS - (int)(COLLISION_SEGMENTS * (2 * PI - d_fieldOfView) / (2 * PI))

#define OPTICAL_HEIGHT 100

//RENDER
#define FILL_RENDER_COLOR {255, 255, 255, 255}
#define FOG_SEGMENTS 32
#define FOG_INTENSITY 0.83

#define MIRROR_DEBUG false
#define THREADED true
#define PLAYERS_FLAT true

#define DIS_SEG_FOR_BOTS 8 // How much vertical sectors we use in ray cast
#define DISTANCES_SEGMENTS 1280 // How much vertical sectors we use in ray cast
#define FLOOR_SEGMENT_SIZE 8

#define FPS 60
#define MONITOR_TILE (SCREEN_WIDTH / DISTANCES_SEGMENTS)
#define DIST /*(FOV < PI-0.1) ? */DISTANCES_SEGMENTS / (tan(FOV / 2) * 2) * 13 * FOV / (PI / 2) * 1280 / DISTANCES_SEGMENTS/* : DISTANCES_SEGMENTS * 3*/

#define OUTLINE_SEGMENTS false
#define OUTLINE_SEGMENTS_COLOR { 0, 0, 0, 255 }

//BACKGROUND
#define OUTLINE_BACK_THICKNESS 5
#define OUTLINE_BACK_COLOR {255, 255, 255, 255}
#define FILL_BACK_COLOR {255, 255, 255, 255}

//TEXTURES

inline std::string getBaseName(const std::string& path) {
	size_t slash1 = path.find_last_of("/\\");
	size_t slash2 = path.find_last_of("/\\", slash1 - 1);

	if (slash2 == std::string::npos) {
		slash2 = 0;
	}
	else {
		slash2 += 1;
	}
	return path.substr(slash2, slash1 - slash2);
}

#define BUILDED getBaseName(std::filesystem::current_path().string()) == "build"

#define MAIN_TEXTURE BUILDED ? "../../../Textures/TroubleShooting/without_texture.png" : "Textures/TroubleShooting/without_texture.png"
#define BALDI BUILDED ? "../../../Textures/Heroes/Baldi.png" : "Textures/Heroes/Baldi.png"
#define SKIN BUILDED ? "../../../Textures/Heroes/DoomGuy.png" : "Textures/Heroes/DoomGuy.png"
#define SKIN1 BUILDED ? "../../../Textures/Heroes/DoomGuy(1).png" : "Textures/Heroes/DoomGuy(1).png"

#define SKY_TEXTURE BUILDED ? "../../../Textures/Backgrounds/back.jpg" : "Textures/Backgrounds/back.jpg"
#define WALL_TEXTURE BUILDED ? "../../../Textures/Walls/brickWall.png" : "Textures/Walls/brickWall.png"
#define WALL_TEXTURE1 BUILDED ? "../../../Textures/Walls/wall_12.jpg" : "Textures/Walls/wall_12.jpg"
#define WALL_TEXTURE2 BUILDED ? "../../../Textures/Walls/minecraft.jpg" : "Textures/Walls/minecraft.jpg"
#define COLUMN_TEXTURE BUILDED ? "../../../Textures/Walls/column1.jpg" : "Textures/Walls/column1.jpg"

#define WIND_TEXTURE BUILDED ? "../../../Textures/BackGrounds/wind.png" : "Textures/BackGrounds/wind.png"

#define WEAPON_ARM_TEXTURE BUILDED ? "../../../Textures/Weapons/Shotgun/Arm.png" : "Textures/Weapons/Shotgun/Arm.png"
#define WEAPON_TRUNK_TEXTURE BUILDED ? "../../../Textures/Weapons/Shotgun/Trunk.png" : "Textures/Weapons/Shotgun/Trunk.png"
#define WEAPON_FLASH_TEXTURE BUILDED ? "../../../Textures/Weapons/Shotgun/Flash.png" : "Textures/Weapons/Shotgun/Flash.png"
#define WEAPON_AIM_TEXTURE BUILDED ? "../../../Textures/Weapons/Shotgun/aim.png" : "Textures/Weapons/Shotgun/aim.png"

#define MENU_MUSIC BUILDED ? "../../../Sounds/UnrealSuperHero3.ogg" : "Sounds/UnrealSuperHero3.ogg"
#define WALK_SOUND BUILDED ? "../../../Sounds/Walking_sounds.wav" : "Sounds/Walking_sounds.wav"
#define BACK_GROUND_SOUND BUILDED ? "../../../Sounds/Nature_sounds.wav" : "Sounds/Nature_sounds.wav"
#define SHOTGUN_SHOT BUILDED ? "../../../Sounds/Shotgun_shot.wav" : "Sounds/Shotgun_shot.wav"
#define SHOTGUN_RELOAD BUILDED ? "../../../Sounds/Shotgun_reload.wav" : "Sounds/Shotgun_reload.wav"

#define CONNECT_FILE  BUILDED ? "../../../Connect.txt" : "Connect.txt"

inline bool checkptr(sf::Texture& obj, sf::Texture* ptr) {
	if (ptr != nullptr) {
		obj = *ptr;
		return true;
	}
	
	const std::string path = BUILDED ? "../../../Textures/TroubleShooting/without_texture.png" : "Textures/TroubleShooting/without_texture.png";
	if (!std::filesystem::exists(path)) {
		sf::RenderTexture surface;
		unsigned int width = 800, height = 600;
		surface.create(width, height);

		sf::Font font;
		if (!font.loadFromFile("C:/Windows/Fonts/arialbd.ttf")) {
			std::cerr << "Error with loading a font!!!" << std::endl;
		}

		sf::Text text("Without Texture", font, 100);
		text.setFillColor(sf::Color(255, 0, 0, 255));
		sf::FloatRect text_bounds = text.getLocalBounds();
		text.setPosition(sf::Vector2f((width - text_bounds.width) / 2.f, height / 2.f - text.getCharacterSize() / 2.f));

		surface.clear(sf::Color(200, 200, 200));
		surface.draw(text);
		surface.display();

		sf::Texture rendered_texture = surface.getTexture();
		sf::Image rendered_image = rendered_texture.copyToImage();
		rendered_image.saveToFile(path);

		obj = rendered_texture;
	}
	else {
		sf::Texture texture;
		texture.loadFromFile(path);
		obj = texture;
	}
	
	return false;
}

inline bool checkptr(sf::Sound& obj, sf::SoundBuffer* ptr) {
	if (ptr) {
		obj.setBuffer(*ptr);

		return true;
	}
	
	obj.setBuffer(sf::SoundBuffer());
	return false;
}

#endif //PSEUDO3DENGINE_SETTINGS_H