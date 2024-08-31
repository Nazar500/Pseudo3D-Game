#ifndef MENU_H
#define MENU_H

#include "Settings.h"
#include "ResourceManager.h"
#include "Point2D.h"
#include "SFML/Network/IpAddress.hpp"

#include <sstream>

enum Tabs {
	Quit,
	Play,
	Setting,
	About,
	Main
};

class Menu {
private:
	unsigned char size;
	sf::Text::Style style;

	int left;
	int text_width;
	int interval;
	Tabs tab = Tabs::Main;
	std::vector<pair<string, bool>> settingTabs;

	double sensivity = .002, maxSensivity = .02, minSensivity = .0;
	sf::IpAddress localIp;
	sf::IpAddress globalIp;

	sf::Font& f;
	sf::Vector2i mouse;
	bool was_released;
public:
	explicit Menu(sf::Font& font, sf::Text::Style style, unsigned char size);

	void update(sf::RenderTarget& sc, sf::Vector2i mouse_position, sf::IpAddress global_ip);
	void to_main();
	void draw(sf::RenderTarget& sc);

	static sf::Color oppositeColor(sf::Color col);
	static std::vector<std::string> split(const std::string& str, const char& delimeter = ' ');
	static string rot(const std::string& str, unsigned char& count);
	static string strip(const string& a);

	void draw_settings(sf::RenderTarget& sc);
	void draw_about(sf::RenderTarget& sc) const;
	void draw_slider(sf::RenderTarget& sc, const sf::Vector2f& pos, int width, int height);
	void draw_switcher(sf::RenderTarget& sc, const sf::Vector2f& pos, int width, int height, int i);

	Tabs getState() const;
	double getSensivity() const;
	vector<pair<string, bool>> getSettings() const;
	pair<sf::Font, sf::Text::Style> getFont() const;

	void SetPerception(bool active);
};

#endif