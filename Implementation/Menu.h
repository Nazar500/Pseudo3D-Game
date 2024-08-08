#ifndef MENU_H
#define MENU_H

#include "Settings.h"

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

	Font& f;
	Vector2i mouse;
public:
	explicit Menu(Font& font, Text::Style style, unsigned char size);

	void update(RenderTarget& sc, Vector2i mouse_position);
	void to_main();
	void draw(RenderTarget& sc);

	Color oppositeColor(Color col);

	void draw_settings(RenderTarget& sc) const;
	void draw_about(RenderTarget& sc) const;

	Tabs getState() const;
};

#endif