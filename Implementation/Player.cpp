#include "PLayer.h"

void Player::oneMoreKill()
{
	++i_kills;
}

void Player::oneMoreDeath()
{
	++i_deaths;
}

Player::Player(const Point2D& position, double height, double health, const std::string& TEXTURE) : Square2D(position, RADIUS_CAMERA, height, TEXTURE), d_health(health), d_start_health(health), start_pos(position)
{}

Player::Player(const Player& player) : Square2D(player.position(), RADIUS_CAMERA, player.height())
{
	this->T_texture = player.T_texture;
	this->d_health = player.d_health;
	this->d_height = player.d_height;
	this->start_pos = player.start_pos;
	this->d_start_health = player.d_start_health;
}

double Player::health() const
{
	return d_health;
}

ObjectType Player::type()
{
	return ObjectType::player;
}

bool Player::reduceHealth(const double& damage)
{	
	if (d_health - damage <= 0) {
		d_health = d_start_health;
		oneMoreDeath();

		p_pos = start_pos;

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

