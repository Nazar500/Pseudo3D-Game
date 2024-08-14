#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "Settings.h"
#include "Point2D.h"
#include "ObjectType.h"
#include "Object2D.h"
#include "ClientUdp.h"
#include "ServerUdp.h"
#include "FlatObject.h"
#include "World.h"
#include "Player.h"
#include "Weapon.h"
#include "ResourceManager.h"

#include <vector>
#include <condition_variable>
#include <atomic>
#include <mutex>
#include <thread>
#include <Windows.h>

using namespace std;

struct Point4D {
	sf::Uint8 x, y, z, w;

	Point4D(sf::Uint8 xx = 0, sf::Uint8 yy = 0, sf::Uint8 zz = 0, sf::Uint8 ww = 255) : x(xx), y(yy), z(zz), w(ww) {}
	Point4D(sf::Color c) : x(c.r), y(c.g), z(c.b), w(c.a) {}

	Point4D operator +(Point4D p) {
		return Point4D{ (sf::Uint8)(x + p.x), (sf::Uint8)(y + p.y), (sf::Uint8)(z + p.z), (sf::Uint8)(w + p.w) };
	}
	Point4D operator -(Point4D p) {
		return Point4D{ (sf::Uint8)(x - p.x), (sf::Uint8)(y - p.y), (sf::Uint8)(z - p.z), (sf::Uint8)(w - p.w) };
	}

	Point4D operator *(int num) {
		return Point4D{ (sf::Uint8)(x * num), (sf::Uint8)(y * num), (sf::Uint8)(z * num), (sf::Uint8)(w * num) };
	}

	Point4D operator /(int num) {
		return Point4D{ (sf::Uint8)(x / num), (sf::Uint8)(y / num), (sf::Uint8)(z / num), (sf::Uint8)(w / num) };
	}

	Point4D average(Point4D p) {
		return (*this + p) / 2;
	}

	Point4D avarage(sf::Color p) {
		return (*this + Point4D(p)) / 2;
	}

	sf::Color to_col() {
		return sf::Color(x, y, z, w);
	}
};

struct RayCastStructure
{
	RayCastStructure(double d_dist, double d_prog, bool back_texture, Point2D v_angle, Object2D* obj, double height, std::vector<RayCastStructure> v_mirrorRayCast = {}) {
		distance = d_dist;
		progress = d_prog;
		backTexture = back_texture;
		object = obj;
		rayDirection = v_angle;

		this->height = height;
		this->v_mirrorRayCast = v_mirrorRayCast;
	}

	double distance;
	double progress;
	bool backTexture;
	Point2D rayDirection;
	Object2D* object;
	double height;


	std::vector<RayCastStructure> v_mirrorRayCast;
};

struct CollisionInfo
{
	CollisionInfo(Point2D d_point = {}, pair<Point2D, Point2D> d_wall = { {}, {} }, double d_dist = COLLISION_AREA, double d_height = 0, Object2D* d_obj = nullptr) {
		point = d_point;
		edge = d_wall;
		distance = d_dist;
		height = d_height;
		object = d_obj;
	}

	Point2D point;
	std::pair<Point2D, Point2D> edge;
	double distance;
	double height;
	Object2D* object;
};

struct FrameImportantInfo
{
	std::vector<std::vector<RayCastStructure>> distances;
	std::vector<CollisionInfo> collisions;
	Point2D position;
	double direction;
	double height;
};


class Camera : public Player
{
private:

	double directionSin = 0;
	double directionCos = 0;
	/*double horizontalCos[DISTANCES_SEGMENTS];
	double horizontalSin[DISTANCES_SEGMENTS];
	double verticalTan[SCREEN_HEIGHT];*/

	FrameImportantInfo oldFrame, curFrame;

	//std::map<std::string, std::shared_ptr<Player>> m_playersOnTheScreen;

	// accesories
	Weapon weapon;
	unsigned char wind_index = 0;
	short wind_speed = 50; // milliseconds
	int wind_count = 0;

	// sounds
	sf::Sound walkSound;
	sf::Music backGround, menuMusic;

	//// for static collision when the object move and you aren't moving
	//Point2D collision_vector;

	double d_direction;
	double d_fieldOfView;
	double d_eyesHeight;
	double d_depth;
	double d_vSpeed = 0;
	double d_health = 100;

	double d_jumpSpeed;

	double d_walkSpeed;
	unsigned char d_walk = 0, d_old_walk = d_walk;

	double d_viewSpeed;

	// setting
	//double d_elapsedTime;
	double d_verticalShift = 0;
	int d_reflection_limit;

	bool b_collision = false;
	bool b_textured = false;
	bool b_hadFocus = false;
	bool b_2d_map = false;
	bool b_music = false;
	bool b_sounds = false;

	World& W_world;

	// threads
	short threadCount;
	std::atomic<int> finished;
	std::atomic<signed char> work;

	std::vector<std::shared_ptr<std::thread>> threads;
	std::mutex renderM;
	std::mutex collisionM;
	std::mutex startM;
	std::mutex endM;

	std::condition_variable startCV;
	std::condition_variable endCV;

	sf::Vector2i mouse;

	// for 2d map
	sf::ConvexShape fov;
	sf::CircleShape player;

	std::vector<sf::Vector2f> map_points;

	void rayDraw(sf::RenderTarget& window, std::vector<RayCastStructure>& v_raycast, int& shift);
	void drawVerticalStrip(sf::RenderTarget& window, RayCastStructure k, int shift);
	void drawRunningWind(RenderTarget& window, int dt);

	void updateThread(int i, int n);
	void setThreadAffinity(std::thread& thread, int core_id);

	void updateHiddenDistances(int from, int to);
	void updateDistances(int from, int to);
	void objectsRayCrossed(std::pair<Point2D, Point2D>& ray, std::vector<RayCastStructure>& v_raycast, const Object2D* caster, CollisionInfo* collision = nullptr, int reflections = 0);
	void hiddenObjectsRayCrossed(pair<Point2D, Point2D>& ray, CollisionInfo& collision, const Object2D* caster);
	void IncreaseDistance(vector<RayCastStructure>& v_RayCastStructure, double dist);

	void setFieldOfView(double angle);

protected:
	void shift_col(Point2D vector);
	void shift(Point2D vector);
	void recoil_shift(Point2D vector);

public:
	explicit Camera(World& world, const Point2D& position, double vPos = 0, double height = 0.6, double health = 100., const std::string& texture = SKIN, const std::string& texture1 = SKIN1, double fieldOfView = FOV, double angle = FOV, double eyesHeight = 1.f, double depth = 9000, double walkSpeed = 400, double jumpSpeed = 2.75, double viewSpeed = .002, int reflection_limit = 20);
	//Camera(const Camera& other);
	~Camera(); // finishing threads

	void SoundsPause();
	void SoundsResume();
	void SoundsCorrection();

	void setTextured(bool active);
	bool getTextured() const;

	void setCollision(bool active);
	bool getCollision() const;

	void set2D_map(bool active);
	bool get2D_map() const;

	void setSensivity(double value);
	double getSensivity() const;

	void setSounds(bool active);
	bool getSounds() const;

	void setMusic(bool active);
	bool getMusic() const;

	static sf::Vector2f scaling(const sf::Vector2u& size_before, const sf::Vector2u& size_after);
	sf::Vector2f scaling(const sf::IntRect& size_before, const sf::Vector2u& size_after);

	void keyboardControl(double dt, sf::Vector2i position, RenderTarget& window);
	void lookAt(const std::string& name);
	Point2D normal() const;
	void fire(vector<RayCastStructure>& v_rayCast, Point2D vect);
	void cameraRayCheck();

	void startFrameProcessing();
	void endFrameProcessing();
	
	void draw_map(sf::RenderTarget& window);
	void drawCameraView(sf::RenderTarget& window, int dt);
};


#endif