#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include "Settings.h"
#include "Square2D.h"
#include "Circle2D.h"

class Player : public Square2D
{
protected:
	double d_health, d_s_health;
	Point2D p_s_pos;

	int i_kills = 0, i_deaths = 0;
public:
	explicit Player(const Point2D& position, double height = 0.6, double health = 100, const std::string& TEXTURE = SKIN, const std::string& TEXTURE1 = SKIN1);
	Player(const Player& player);

	void setPosition(const Point2D& pos);
	void setStartPos(const Point2D& pos);
	Point2D getStartPos() const;
	double getStartHealth() const;
	double health() const;
	ObjectType type() override;

	void setHealth(double health);
	bool reduceHealth(const double& damage);
	int getKills() const;
	int getDeaths() const;

	void oneMoreKill();
	void oneMoreDeath();

	void setDeaths(int num);
	void setKills(int num);
};

#endif