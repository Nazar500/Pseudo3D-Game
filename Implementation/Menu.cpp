#include "Menu.h"

Menu::Menu(Font& font, Text::Style style, unsigned char size) : f(font), size(size), style(style)
{
	text_width = (int)(Text("Settings", font, size).getLocalBounds().width);
	left = SCREEN_WIDTH / 2 - text_width / 2 - text_width / 7;

	interval = (SCREEN_HEIGHT - size * 2 * Tabs::Main) / Tabs::Main;
}

void Menu::update(RenderTarget& sc, Vector2i mouse_position)
{
	mouse = mouse_position;

	if (getState() > 1) {
		draw(sc);
	}
}

void Menu::to_main()
{
	tab = (tab == Tabs::Main) ? Tabs::Quit : Tabs::Main;
}

void Menu::draw(RenderTarget& sc)
{
	switch (tab) {
	case Tabs::Play:
		break;
	case Tabs::Settings:
		draw_settings(sc);
		break;
	case Tabs::About:
		draw_about(sc);
		break;
	case Tabs::Main:

		for (int i = Tabs::Play; i <= Tabs::Main; i++) {
			int j = i - 1;

			int box_width = text_width + text_width / 7 * 2;
			int box_top = j * size * 2 + (int)((j + 0.5f) * interval);
			IntRect button(Rect<int>(left, box_top, box_width, size * 2));

			Text text("", f, size);
			text.setStyle(style);
			

			switch (i) {
			case 4:
				text.setString("Quit");
				break;
			case 1:
				text.setString("Play");
				break;
			case 2:
				text.setString("Settings");
				break;
			case 3:
				text.setString("About");
				break;
			}

			int width = (int)text.getGlobalBounds().width;
			text.setPosition(left + (box_width - width) / 2.f, box_top + size / 2.5f);

			if (button.contains(mouse)) {
				text.setFillColor({ 255, 255, 255 });
				if (Mouse::isButtonPressed(Mouse::Left)) {
					tab = ((Tabs)i == Tabs::Main) ? Tabs::Quit : (Tabs)i;
				}
			}
			else {
				text.setFillColor({ 0, 0, 0 });
			}

			RectangleShape obj(Vector2f(button.getSize()));
			obj.setPosition(Vector2f(button.getPosition()));

			obj.setOutlineThickness(size / -10.f);
			obj.setOutlineColor(Color::Black);
			obj.setFillColor(oppositeColor(text.getFillColor()));

			sc.draw(obj);
			sc.draw(text);
		}

		break;
	}

	if (tab < 4 && tab > 1) {
		Text text("<<< Back (Escape)", f, size / 3);

		Vector2f text_pos = { 5, 0 };

		text.setPosition(text_pos);
		text.setOutlineThickness(-1);

		IntRect rect(Rect<int>((int)text_pos.x, (int)text_pos.y, (int)text.getGlobalBounds().width, (int)text.getCharacterSize()));

		if (rect.contains(mouse)) {
			text.setFillColor({0, 255, 255});
			text.setOutlineColor({ 0, 0, 0 });
			if (Mouse::isButtonPressed(Mouse::Left)) {
				to_main();
			}
		}
		else {
			text.setFillColor({ 0, 0, 0 });
			text.setOutlineColor({ 0, 255, 255 });
		}

		sc.draw(text);
	}
}

Color Menu::oppositeColor(Color col)
{
	return Color(255 - col.a, 255 - col.b, 255 - col.g);
}

void Menu::draw_settings(RenderTarget& sc) const
{
}

void Menu::draw_about(RenderTarget& sc) const
{
	int text_size = size / 2;
	/*std::string text("What is Lorem Ipsum? Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book.");
	
	int x = 20, width = (int)Text(text, f, text_size).getGlobalBounds().width;
	int line_len = SCREEN_WIDTH - x * 2;
	int temp_lines = width / line_len;
	int temp_len = line_len / (width / (int)text.length()) - 2;*/

	//std::cout << temp_len << " " << temp_lines << " " << line_len << std::endl;

	
	Text temp("Pseudo-3D Game", f, text_size * 2), temp1("A Simple Ray-Cast Algorithm can build Pseudo-3D Image!", f, text_size), temp2("This is an implementation of Pseudo-3D Game Engine", f, text_size), temp3("Enjoy!!!", f, text_size); // Text temp(text.substr(temp_len * i, temp_len), f, text_size);
	temp.setStyle(style), temp1.setStyle(style), temp2.setStyle(style), temp3.setStyle(style);
	temp.setFillColor({ 0, 0, 0 }), temp1.setFillColor({ 0, 0, 0 }), temp2.setFillColor({ 0, 0, 0 }), temp3.setFillColor({ 0, 0, 0 });

	temp.setPosition((float)(int)((SCREEN_WIDTH - temp.getGlobalBounds().width) / 2.f), 0 * (float)text_size + 20);
	temp1.setPosition((float)(int)((SCREEN_WIDTH - temp1.getGlobalBounds().width) / 2.f), 1 * (float)text_size + 20);
	temp2.setPosition((float)(int)((SCREEN_WIDTH - temp2.getGlobalBounds().width) / 2.f), 2 * (float)text_size + 20);
	temp3.setPosition((float)(int)((SCREEN_WIDTH - temp3.getGlobalBounds().width) / 2.f), 3 * (float)text_size + 20);

	sc.draw(temp);
	sc.draw(temp1);
	sc.draw(temp2);
	sc.draw(temp3);
}

Tabs Menu::getState() const 
{
	return tab;
}


