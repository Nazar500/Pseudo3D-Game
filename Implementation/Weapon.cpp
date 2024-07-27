#include "Weapon.h"

Weapon::Weapon(double damage, double reload_time, int ammo, double recoil, Point2D offset, Point2D reload_offset, Point2D flash_offset, double speed, double raise, const std::string& arm_p, const std::string& trunk_p, const std::string& flash_p) : w_damage(damage), w_reload_t(reload_time/0.4), ammo(ammo), offset(offset), w_speed(speed), w_raise(raise), w_recoil(recoil*100), r_offset(reload_offset), f_offset(flash_offset)
{

	checkptr(arm_t, ResourceManager::loadTexture(arm_p));
	checkptr(trunk_t, ResourceManager::loadTexture(trunk_p));
	checkptr(flash_t, ResourceManager::loadTexture(flash_p));
	checkptr(aim_t, ResourceManager::loadTexture(WEAPON_AIM_TEXTURE));

	arm.setTexture(arm_t);
	trunk.setTexture(trunk_t);
	flash.setTexture(flash_t);
	aim.setTexture(aim_t);

	// configure
	aim.setScale(0.25, 0.25);
	aim.setOrigin(aim.getLocalBounds().width / 2, aim.getLocalBounds().height / 2);
	aim.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	flash.setScale(0.75, 0.75);
	flash.setOrigin(flash.getLocalBounds().width, flash.getLocalBounds().height);

	arm.setScale(1.5, 1.5);
	trunk.setScale(1.5, 1.5);
}

double Weapon::getDamage() const
{
	return w_damage;
}

void Weapon::setDamage(double damage)
{
	w_damage = damage;
}

void Weapon::setReloadTime(double time)
{
	w_reload_t = time;
}

double Weapon::getReloadTime() const
{
	return w_reload_t;
}

bool Weapon::shoot()
{
	if (!anim_recoil && !anim_reload) {
		anim_recoil = true;
		d_recoilTime.restart();
		anim_flash = true;

		return true;
	}
	return false;
}

Point2D roll_gen(bool abs_, bool inverse, Point2D end, double time, double speed) {
	Point2D res;
	/*res.x = (abs) ? (1 + sin(time * speed)) * end.x / 2 : sin(time * speed) * end.x;
	res.y = (abs) ? (1 + sin(time * speed)) * end.y / 2 : sin(time * speed) * end.y;*/
	res.x = (abs_) ? abs(sin(time * speed)) * end.x : sin(time * speed) * end.x;
	res.y = (abs_) ? abs(sin(time * speed)) * end.y : sin(time * speed) * end.y;

	if (inverse) {
		res.x = end.x - res.x;
		res.y = end.y - res.y;
	}

	return res;
}

double roll_gen(bool abs, bool inverse, double end, double time, double speed) {
	double res = (abs) ? (1 + sin(time * speed)) * end / 2 : sin(time * speed) * end;
	if (inverse) {
		if (res < 0) {
			return -res;
		}
		else {
			return end - res;
		}
	}
	return res;
}

void Weapon::draw(sf::RenderTarget& window)
{
	// time s
	Point2D roll_arm = roll_gen(0, 0, r_offset, d_armTime.getElapsedTime().asSeconds(), w_speed * 2 / w_reload_t);
	double roll_trunk = roll_gen(1, 0, w_raise, d_trunkTime.getElapsedTime().asSeconds(), w_speed);
	double shoot_recoil = roll_gen(1, 0, w_recoil, d_recoilTime.getElapsedTime().asSeconds(), w_speed * 4);

	// configure
	sf::Vector2f scale = trunk.getScale();
	sf::FloatRect trunk_bounds = trunk.getLocalBounds();
	trunk.setPosition((float)(SCREEN_WIDTH - trunk_bounds.width * scale.x + ((anim_recoil) ? shoot_recoil : roll_trunk)), (float)(SCREEN_HEIGHT - trunk_bounds.height * scale.y + ((anim_recoil) ? shoot_recoil : roll_trunk)));

	Point2D trunk_pos = trunk.getPosition();
	Point2D arm_pos = { (trunk_pos.x + offset.x * scale.x), (trunk_pos.y + offset.y * scale.y) };
	
	// update
	if (anim_flash) {
		flash.setPosition((trunk_pos - f_offset).to_sff());
	}

	if (shoot_recoil <= w_recoil * 0.5) { anim_flash = false; }
	if (shoot_recoil <= 5 && anim_recoil) {
		anim_recoil = false;
		anim_reload = true;
		d_trunkTime.restart();
		d_armTime.restart();

		// debug
		//r_time.restart();

		// to make sure
		roll_arm = roll_gen(0, 0, r_offset, d_armTime.getElapsedTime().asSeconds(), w_speed * 2 / w_reload_t);
	}
	if (anim_reload) {
		arm.setPosition((arm_pos + roll_arm).to_sff());

		double max_value = max(roll_arm.x, roll_arm.y);

		if (max_value < 0) { anim_reload = false; } //cout << r_time.getElapsedTime().asMilliseconds() << endl;
	}
	else {
		arm.setPosition(arm_pos.to_sff());
	}

	// draw
	window.draw(trunk);
	window.draw(arm);
	window.draw(aim);
	if(anim_flash) { window.draw(flash); }
}
