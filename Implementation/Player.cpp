#include "PLayer.h"

using namespace Settings;

void Player::oneMoreKill()
{
	++i_kills;
}

void Player::oneMoreDeath()
{
	++i_deaths;
}

void Player::setDeaths(int num)
{
	i_deaths = num;
}

void Player::setKills(int num)
{
	i_kills = num;
}

Player::Player(const Point2D& position, double height, double health, const std::string& TEXTURE, const std::string& TEXTURE1) : Square2D(position, RADIUS_CAMERA, height, TEXTURE), d_health(health), d_s_health(health), p_s_pos(position)
{
	setTexture1(TEXTURE1);
}

Player::Player(const Player& player) : Square2D(player.position(), RADIUS_CAMERA, player.height())
{
	this->T_texture = player.T_texture;
	this->d_health = player.d_health;
	this->d_height = player.d_height;
	this->p_s_pos = player.p_s_pos;
	this->d_s_health = player.d_s_health;
}

void Player::setPosition(const Point2D& pos)
{
	this->p_pos = pos;
}

void Player::setStartPos(const Point2D& pos)
{
	this->p_s_pos = pos;
}

Point2D Player::getStartPos() const
{
	return p_s_pos;
}

double Player::getStartHealth() const
{
	return d_s_health;
}

double Player::health() const
{
	return d_health;
}

ObjectType Player::type()
{
	return ObjectType::player;
}

void Player::setHealth(double health)
{
	d_health = health;
}

bool Player::reduceHealth(const double& damage)
{	
	if (d_health - damage <= 0) {
		d_health = d_s_health;
		this->oneMoreDeath();

		p_pos = p_s_pos;

		return true;
	}
	else
	{
		d_health -= damage;
	}
	return false;
}

int Player::getKills() const
{
	return i_kills;
}

int Player::getDeaths() const
{
	return i_deaths;
}

