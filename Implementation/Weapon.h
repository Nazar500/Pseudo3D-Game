#pragma once

#ifndef WEAPON_H
#define WEAPON_H

#include "Settings.h"
#include "ResourceManager.h"
#include "Point2D.h"
#include "Idrawable.h"

#include <vector>
#include <cmath>

class Weapon : virtual public Idrawable {
private:
	sf::Clock d_trunkTime, d_armTime, d_recoilTime;

	// debug . if you want you can make sure that reload_time corresponds the reload_time by removing the slashes before "r_time" actions and of course 'write correct code'(correct the errors if any)
	//sf::Clock r_time;

	double w_damage;
	double w_reload_t;
	double w_speed;
	double w_raise;
	double w_recoil;
	int ammo;

	bool anim_recoil = false, anim_reload = false, anim_flash = false;

	// offset refering the trunk (of arm), reloading_offset ( to what position have the arm with the holder move (local coordinates system)), flash_offset (the offset points where the falsh have to be placed)
	Point2D offset, r_offset, f_offset;

	sf::Texture arm_t, trunk_t, flash_t, aim_t;
	sf::Sprite arm, trunk, flash, aim;

public:
	explicit Weapon(double damage = 150., double reload_time = 0.4f, int ammo = 10, double recoil = 1., Point2D offset = { -48, 40 }, Point2D reload_offset = { 18, 15 }, Point2D flash_offset = { 0, 0 }, double speed = 4, double raise = 20, const std::string& arm_t = WEAPON_ARM_TEXTURE, const std::string& trunk_t = WEAPON_TRUNK_TEXTURE, const std::string& flash_t = WEAPON_FLASH_TEXTURE);

	double getDamage() const;
	void setDamage(double damage);

	// in seconds
	void setReloadTime(double time);
	double getReloadTime() const;

	bool shoot();
	void draw(sf::RenderTarget& window) override;
};

#endif
