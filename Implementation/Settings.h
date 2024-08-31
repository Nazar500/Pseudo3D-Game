#ifndef PSEUDO3DENGINE_SETTINGS_H
#define PSEUDO3DENGINE_SETTINGS_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <cfenv>
#include <windows.h>

namespace Settings {
	const double PI = 3.1415926535897932384626433832795;
	const double ET = std::chrono::duration<double>(std::chrono::steady_clock::now().time_since_epoch()).count();

	const bool LOG = true;
	const bool DEBUG = true;
	const bool CONSOLE = false;
	const bool MIRROR_DEBUG = false;
	const bool THREADED = true;
	const bool NETWORK = true;

	const int SCALE = 40; //How much pixels = 1 unit of distance measure
	const int MAP_SCALE = ((MIRROR_DEBUG) ? 3 : 10);

	const int SCALE_WINDOW = 50;

	const float SIDE = (float)SCALE_WINDOW * SCALE;

	// SCREEN SIZE
	extern int WIDTH; //sf::VideoMode::getDesktopMode().width; // 1280
	extern int HEIGHT; //sf::VideoMode::getDesktopMode().height; // 720

	extern int SCREEN_WIDTH; //((WIDTH < 800) ? 800 : WIDTH) // 1280
	extern int SCREEN_HEIGHT; //((HEIGHT < 600) ? 600 : HEIGHT) // 720
	extern int SCREEN_SIDE; // 720
	const std::string TITLE = "SFML Pseudo3D";

	// NETWORK
	const unsigned int NETWORK_VERSION = 2U;
	const int TIMEOUT_SECONDS = 5;
	const int WORLD_UPDATE_RATE = 30;
	const double RELIABLE_RETRY_TIME = (1.0 / 20);

	// OBJECTS
	const int OUTLINE_THICKNESS = 10;
	const sf::Color OUTLINE_COLOR = { 255, 255, 255, 255 };
	const sf::Color FILL_COLOR = { 255, 174, 174, 255 };

	const int CIRCLE_CONVEX_NUMBER = 50;

	// CAMERA
	const int DIS_SEG_FOR_BOTS = 8; // How much vertical sectors we use in ray cast
	extern int DISTANCES_SEGMENTS; // How much vertical sectors we use in ray cast
	const int FLOOR_SEGMENT_SIZE = 8;

	const int OUTLINE_CAMERA_THICKNESS = 1;
	const sf::Color OUTLINE_CAMERA_COLOR = { 255, 255, 255, 255 };
	const sf::Color FILL_CAMERA_COLOR = { 255, 67, 67, 255 };
	const double RADIUS_CAMERA = SCALE * 2;
	extern int CONVEX_NUMBER;
	const sf::Color FILED_OF_VEW_COLOR = { 255, 189, 51, 0 };
	const sf::Color FILED_OF_VEW_OUTLINE_COLOR = { 0, 0, 0, 255 };
	const sf::Color RED_COLOR = { 255, 0, 0 };

	const double FOV = PI / 2;
	const bool CORRECTION = false;

	// FOR COLLISION DETECTION
	const int COLLISION_DISTANCE = 80;
	const int COLLISION_AREA = 400;
	const int COLLISION_SEGMENTS = 30; //(int)(std::max((DISTANCES_SEGMENTS / 50.), 1.));
	const int HIDDEN_SEGMENTS = (int)(COLLISION_SEGMENTS * (2 * PI - FOV) / (2 * PI));
	const int SHOWN_SEGMENTS = COLLISION_SEGMENTS - (int)(COLLISION_SEGMENTS * (2 * PI - FOV) / (2 * PI));

	const int OPTICAL_HEIGHT = 100;

	//RENDER
	const sf::Color FILL_RENDER_COLOR = { 255, 255, 255, 255 };
	const int FOG_SEGMENTS = 32;
	const double FOG_INTENSITY = 0.83;

	extern int FPS;
	extern int MONITOR_TILE;
	extern double DIST;

	const bool OUTLINE_SEGMENTS = false;
	const sf::Color OUTLINE_SEGMENTS_COLOR = { 0, 0, 0, 255 };

	//BACKGROUND
	const int OUTLINE_BACK_THICKNESS = 5;
	const sf::Color OUTLINE_BACK_COLOR = { 255, 255, 255, 255 };
	const sf::Color FILL_BACK_COLOR = { 255, 255, 255, 255 };

	//TEXTURES

	void loadSettings(const std::string& filename);
	void clean_file(const std::string& filename);
	void updateSettings();

	std::string getBaseName(const std::string& path);

	const bool BUILDED = getBaseName(std::filesystem::current_path().string()) == "build";

	const std::string FONT = BUILDED ? "../../../Fonts/CascadiaMono.ttf" : "Fonts/CascadiaMono.ttf";

	const std::string MAIN_TEXTURE = BUILDED ? "../../../Textures/TroubleShooting/without_texture.png" : "Textures/TroubleShooting/without_texture.png";
	const std::string BALDI = BUILDED ? "../../../Textures/Heroes/Baldi.png" : "Textures/Heroes/Baldi.png";
	const std::string SKIN = BUILDED ? "../../../Textures/Heroes/DoomGuy.png" : "Textures/Heroes/DoomGuy.png";
	const std::string SKIN1 = BUILDED ? "../../../Textures/Heroes/DoomGuy(1).png" : "Textures/Heroes/DoomGuy(1).png";

	const std::string SKY_TEXTURE = BUILDED ? "../../../Textures/Backgrounds/back.jpg" : "Textures/Backgrounds/back.jpg";
	const std::string WALL_TEXTURE = BUILDED ? "../../../Textures/Walls/brickWall.png" : "Textures/Walls/brickWall.png";
	const std::string WALL_TEXTURE1 = BUILDED ? "../../../Textures/Walls/wall_12.jpg" : "Textures/Walls/wall_12.jpg";
	const std::string WALL_TEXTURE2 = BUILDED ? "../../../Textures/Walls/minecraft.jpg" : "Textures/Walls/minecraft.jpg";
	const std::string COLUMN_TEXTURE = BUILDED ? "../../../Textures/Walls/column1.jpg" : "Textures/Walls/column1.jpg";

	const std::string WIND_TEXTURE = BUILDED ? "../../../Textures/BackGrounds/wind.png" : "Textures/BackGrounds/wind.png";
	const std::string IMAG_TEXTURES_OFF = BUILDED ? "../../../Textures/Backgrounds/Textures_off.png" : "Textures/Backgrounds/Textures_off.png";
	const std::string IMAG_MIRRORS = BUILDED ? "../../../Textures/Backgrounds/Mirrors.png" : "Textures/Backgrounds/Mirrors.png";
	const std::string IMAG_MAP = BUILDED ? "../../../Textures/Backgrounds/Map.png" : "Textures/Backgrounds/Map.png";
	const std::string IMAG_INSRUCTION = BUILDED ? "../../../Textures/Walls/Instruction.png" : "Textures/Walls/Instruction.png";

	const std::string WEAPON_ARM_TEXTURE = BUILDED ? "../../../Textures/Weapons/Shotgun/Arm.png" : "Textures/Weapons/Shotgun/Arm.png";
	const std::string WEAPON_TRUNK_TEXTURE = BUILDED ? "../../../Textures/Weapons/Shotgun/Trunk.png" : "Textures/Weapons/Shotgun/Trunk.png";
	const std::string WEAPON_FLASH_TEXTURE = BUILDED ? "../../../Textures/Weapons/Shotgun/Flash.png" : "Textures/Weapons/Shotgun/Flash.png";
	const std::string WEAPON_AIM_TEXTURE = BUILDED ? "../../../Textures/Weapons/Shotgun/aim.png" : "Textures/Weapons/Shotgun/aim.png";

	const std::string MENU_MUSIC = BUILDED ? "../../../Sounds/UnrealSuperHero3.ogg" : "Sounds/UnrealSuperHero3.ogg";
	const std::string WALK_SOUND = BUILDED ? "../../../Sounds/Walking_sounds.wav" : "Sounds/Walking_sounds.wav";
	const std::string BACK_GROUND_SOUND = BUILDED ? "../../../Sounds/Nature_sounds.wav" : "Sounds/Nature_sounds.wav";
	const std::string SHOTGUN_SHOT = BUILDED ? "../../../Sounds/Shotgun_shot.wav" : "Sounds/Shotgun_shot.wav";
	const std::string SHOTGUN_RELOAD = BUILDED ? "../../../Sounds/Shotgun_reload.wav" : "Sounds/Shotgun_reload.wav";

	const std::string CONNECT_FILE = BUILDED ? "../../../Connect.txt" : "Connect.txt";

	bool checkptr(sf::Texture& obj, sf::Texture* ptr);
	bool checkptr(sf::Sound& obj, sf::SoundBuffer* ptr);
}

#endif //PSEUDO3DENGINE_SETTINGS_H