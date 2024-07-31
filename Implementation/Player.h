#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include "Settings.h"
#include "Square2D.h"
#include "Circle2D.h"

class Player : public Square2D
{
protected:
	double d_health, d_start_health;
	Point2D start_pos;

	int i_kills = 0, i_deaths = 0;

	void oneMoreKill();
	void oneMoreDeath();
public:
	explicit Player(const Point2D& position, double height = 1., double health = 100, const std::string& TEXTURE = SKIN);
	Player(const Player& player);

	double health() const;
	ObjectType type() override;

	bool reduceHealth(const double& damage, const double& angle);
	int getKills() const;
	int getDeaths() const;
};

#endif