#ifndef MENU_H
#define MENU_H

#include "Settings.h"
#include "ResourceManager.h"
#include"Point2D.h"

#include <sstream>

using namespace sf;

enum Tabs {
	Quit,
	Play,
	Settings,
	About,
	Main
};

class Menu {
private:
	unsigned char size;
	Text::Style style;

	int left;
	int text_width;
	int interval;
	Tabs tab = Tabs::Main;
	std::vector<pair<string, bool>> settingTabs;

	double sensivity = .002, maxSensivity = .02, minSensivity = .0;

	Font& f;
	Vector2i mouse;
public:
	explicit Menu(Font& font, Text::Style style, unsigned char size);

	void update(RenderTarget& sc, Vector2i mouse_position);
	void to_main();
	void draw(RenderTarget& sc);

	static Color oppositeColor(Color col);
	static std::vector<std::string> split(const std::string& str, const char& delimeter = ' ');
	static string rot(const std::string& str, unsigned char& count);
	static string strip(const string& a);

	void draw_settings(RenderTarget& sc);
	void draw_about(RenderTarget& sc) const;
	void draw_slider(RenderTarget& sc, const Vector2f& pos, int width, int height);
	void draw_switcher(RenderTarget& sc, const Vector2f& pos, int width, int height, int i);

	Tabs getState() const;
	double getSensivity() const;
	vector<pair<string, bool>> getSettings() const;
};

#endif