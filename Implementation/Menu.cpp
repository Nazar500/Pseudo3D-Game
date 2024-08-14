#include "Menu.h"

Menu::Menu(Font& font, Text::Style style, unsigned char size) : f(font), size(size), style(style)
{
	text_width = (int)(Text("Settings", font, size).getLocalBounds().width);
	left = SCREEN_WIDTH / 2 - text_width / 2 - text_width / 7;

	interval = (SCREEN_HEIGHT - size * 2 * Tabs::Main) / Tabs::Main;

	vector<string> temp = split("Textures,Collisions,Map,Log,Sounds,Music,Sensivity", ',');
	settingTabs.resize(temp.size());

	for (int i = 0; i < temp.size(); i++) {
		settingTabs[i] = pair(temp[i], true);
	}
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

std::vector<std::string> Menu::split(const std::string& str, const char& delimeter) {
	std::vector<std::string> res;

	istringstream iss(str);
	std::string temp;

	while (getline(iss, temp, delimeter)) {
		res.push_back(temp);
	}

	return res;
}

string Menu::rot(const std::string& str, unsigned char& count) {
	string abc = "";
	string ABC = "";
	for (char a = 'a'; a <= 'z'; ++a) { abc += a; ABC += toupper(a); }

	string res = "";
	for (const auto& el : str) {
		if (abc.find(tolower(el)) != string::npos) {
			if (el == tolower(el)) {
				res += abc[(abc.find(el) + 13) % abc.length()];
			}
			else {
				res += ABC[(ABC.find(el) + 13) % ABC.length()];
			}
		}
		else {
			res += el;
		}
	}

	return res;
}

string Menu::strip(const string& a) {
	int index = 0, index0 = (int)a.length();
	for (int i = 0; i < a.length(); i++) {
		if (a[i] == ' ') {
			index++;
			cout << index << endl;
		}
		if (a[a.length() - 1 - i] == ' ') {
			index0--;
		}
	}

	return a.substr(index, a.length());
}

void Menu::draw_settings(RenderTarget& sc)
{
	unsigned char t_size = (int)(size * 1.5f);

	for (int i = 0; i < settingTabs.size(); i++) {
		int x = (int)((i < 4) ? SCREEN_WIDTH * 0.05f : SCREEN_WIDTH * 6.f / 10);

		Vector2f pos = { (float)x, (x > SCREEN_WIDTH / 2) ? (SCREEN_HEIGHT / 4) * (i - 3.7f) : SCREEN_HEIGHT / 5 * (i + 0.5f) };

		Text t(settingTabs[i].first, f, t_size);
		Vector2f bounds = t.getGlobalBounds().getSize();

		t.setStyle(style);
		t.setFillColor({ 0, 0, 0 });
		t.setPosition(pos);

		RectangleShape frame(Vector2f(bounds.x + 15.f, bounds.y + t_size * .25f));
		frame.setPosition(pos.x, pos.y + t_size * .125f);
		frame.setFillColor(Color::Transparent); 
		frame.setOutlineThickness(1.f);
		frame.setOutlineColor({ 0, 0, 0 });

		Vector2f frame_pos = frame.getPosition(), frame_size = frame.getSize();

		if (i == settingTabs.size() - 1) {
			Vector2f value_pos = { frame_pos.x, frame_pos.y + frame_size.y + SCREEN_HEIGHT / 20.f };
			Text value(to_string(sensivity), f, t_size / 2);
			value.setFillColor({ 255, 255, 255 });
			value.setPosition(value_pos.x + SCREEN_WIDTH / 160.f, value_pos.y - SCREEN_HEIGHT / 160.f);

			Vector2f slider_pos = { value_pos.x, value_pos.y + t_size / 2.f + SCREEN_HEIGHT / 40.f };
			draw_slider(sc, slider_pos, (int)frame_size.x, (int)(frame_size.y / 2.f));

			RectangleShape value_rect(Vector2f(value.getGlobalBounds().getSize().x + SCREEN_WIDTH / 80.f, value.getGlobalBounds().getSize().y + SCREEN_HEIGHT / 60.f));
			value_rect.setFillColor({ 0, 0, 0 });
			value_rect.setPosition(value_pos.x, value_pos.y);

			sc.draw(value_rect);
			sc.draw(value);
		}
		else {
			draw_switcher(sc, { frame_pos.x + frame_size.x + SCREEN_WIDTH / 30.f, frame_pos.y }, SCREEN_WIDTH / 10, (int)frame_size.y, i);
		}

		sc.draw(t);
		sc.draw(frame);
	}
}

void Menu::draw_about(RenderTarget& sc) const
{
	int text_size = (int)(size / 2);

	std::string text = "A Simple Ray-Cast Algorithm can build Pseudo-3D Image!\nThis is an Implementaion of Pseudo 3D Game Engine\nEnjoy!!!\n\nConnect.txt\nLine 1: Ip Address of the server; Line 2: Port\nServer creates if ip(line 1) is a localhost(127.0.0.1)!!!";

	std::vector<std::string> lines = split(text, '\n');

	Text temp("Pseudo-3D Game", f, text_size * 2);

	temp.setPosition((float)(int)((SCREEN_WIDTH - temp.getGlobalBounds().width) / 2.f), 0 * (float)text_size + 20);
	temp.setStyle(style);
	temp.setFillColor({ 0, 0, 0 });

	int width = SCREEN_WIDTH / 3;
	int height;

	Texture map_;
	if (checkptr(map_, ResourceManager::loadTexture(IMAG_MAP)) && false) {
		height = (int)map_.getSize().y;
	}
	else {
		height = SCREEN_HEIGHT / 3;
	}

	for (int i = 0; i < 3; i++) {
		std::string image = "";
		switch (i) {
		case 0:
			image = IMAG_TEXTURES_OFF;
			break;
		case 1:
			image = IMAG_MIRRORS;
			break;
		case 2:
			image = IMAG_MAP;
			break;
		}

		Texture texture;
		checkptr(texture, ResourceManager::loadTexture(image));
		Vector2u texture_size = texture.getSize();

		Sprite obj(texture);

		float ratio = (float)height / texture_size.y;
		obj.setScale({ ratio, ratio });

		RectangleShape frame(Vector2f(obj.getGlobalBounds().getSize().x, obj.getGlobalBounds().getSize().y - 1.f));
		Vector2f obj_pos = { (width + 50.f) * i + 40.f, text_size * 4.f };
		obj.setPosition(obj_pos);

		frame.setPosition(obj_pos.x, obj_pos.y + 1.f);
		frame.setOutlineThickness(2);
		frame.setOutlineColor({ 0, 0, 0 });
		frame.setFillColor(Color::Transparent);

		sc.draw(obj);
		sc.draw(frame);
	}

	text_size = (int)((SCREEN_HEIGHT - height * 1.5f) / lines.size() / 2.f);

	for (int i = 0; i < lines.size(); i++) {
		Text t(lines[i], f, text_size);
		t.setStyle(style);
		t.setFillColor({ 0, 0, 0 });

		t.setPosition((float)(int)((SCREEN_WIDTH - t.getGlobalBounds().width) / 2.f), i * text_size * 2.f + height * 1.5f);

		sc.draw(t);
	}

	sc.draw(temp);
}

void Menu::draw_slider(RenderTarget& sc, const Vector2f& pos, int width, int height)
{
	RectangleShape rect(Vector2f((float)width, (float)height));
	rect.setFillColor(Color::Transparent);
	rect.setOutlineThickness(1);
	rect.setOutlineColor({ 0, 0, 0 });

	rect.setPosition(pos);

	IntRect hitbox(Rect<int>((int)pos.x, (int)pos.y, width, height));
	if (hitbox.contains(mouse) && Mouse::isButtonPressed(Mouse::Left)) {
		sensivity = (mouse.x - pos.x) / (double)width * maxSensivity;
	}

	Vector2f circle_pos = { (float)(pos.x + width * sensivity / maxSensivity), pos.y + height / 2.f };
	RectangleShape line(Vector2f((float)width, 2.f));
	line.setPosition(pos.x, pos.y + height / 2.f);
	line.setFillColor({ 0, 100, 180 });

	CircleShape point(height / 2.f);
	point.setOrigin(height / 2.f, height / 2.f);
	point.setPosition(circle_pos);
	point.setFillColor({ 0, 167, 251 });

	sc.draw(line);
	sc.draw(point);
	sc.draw(rect);
}

void Menu::draw_switcher(RenderTarget& sc, const Vector2f& pos, int width, int height, int i)
{
	bool& val = settingTabs[i].second;

	RectangleShape rect(Vector2f((float)width, (float)height));
	rect.setFillColor(Color::Transparent);
	rect.setOutlineThickness(1);
	rect.setOutlineColor({ 0, 0, 0 });

	rect.setPosition(pos);

	RectangleShape switcher(Vector2f(width / 2.f, (float)height));
	Text t((val) ? "ON" : "OFF", f, size / 3);
	t.rotate((float)PI / 2.f);
	t.setFillColor({ 0, 0, 0 });

	if (val) {
		switcher.setPosition(pos);
		switcher.setFillColor({ 20, 230, 20 });

		t.setPosition(pos.x + size * .125f, pos.y);
	}
	else {
		switcher.setPosition(pos.x + width / 2.f, pos.y);
		switcher.setFillColor({ 230, 20, 20 });

		t.setPosition(pos.x + width / 2.f + size * .125f, pos.y);
	}

	if (IntRect(Rect<int>((int)switcher.getGlobalBounds().left, (int)switcher.getGlobalBounds().top, (int)switcher.getGlobalBounds().width, (int)switcher.getGlobalBounds().height)).contains(mouse) && Mouse::isButtonPressed(Mouse::Left)) {
		val = !val;
	}

	sc.draw(switcher);
	sc.draw(t);
	sc.draw(rect);
}

Tabs Menu::getState() const 
{
	return tab;
}

double Menu::getSensivity() const
{
	return sensivity;
}

vector<pair<string, bool>> Menu::getSettings() const
{
	return settingTabs;
}


