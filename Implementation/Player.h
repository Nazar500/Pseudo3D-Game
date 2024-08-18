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
	explicit Player(const Point2D& position, double height = 0.6, double health = 100, const std::string& TEXTURE = SKIN, const std::string& TEXTURE1 = SKIN1);
	Player(const Player& player);

	void setPosition(const Point2D& pos);
	Point2D getStartPos() const;
	double health() const;
	ObjectType type() override;

	void setHealth(double health);
	bool reduceHealth(const double& damage);
	int getKills() const;
	int getDeaths() const;
};

#endif