#include "Settings.h"

namespace Settings {

	int WIDTH = sf::VideoMode::getDesktopMode().width; // 1280
	int HEIGHT = sf::VideoMode::getDesktopMode().height; // 720
	int FPS = 60;

	int SCREEN_WIDTH = std::max(800, static_cast<int>(WIDTH)); //((WIDTH < 800) ? 800 : WIDTH) // 1280
	int SCREEN_HEIGHT = std::max(600, static_cast<int>(HEIGHT)); //((HEIGHT < 600) ? 600 : HEIGHT) // 720
	int SCREEN_SIDE = int((SCREEN_WIDTH + SCREEN_HEIGHT) / 2); // 720

	int DISTANCES_SEGMENTS = SCREEN_WIDTH;
	double DIST = DISTANCES_SEGMENTS / (tan(FOV / 2) * 2) * 8 * SCREEN_WIDTH / SCREEN_HEIGHT;
	int MONITOR_TILE = int(static_cast<float>(SCREEN_WIDTH) / DISTANCES_SEGMENTS);
	int CONVEX_NUMBER = int(DISTANCES_SEGMENTS / 10.f);

	void updateSettings() {
		SCREEN_WIDTH = std::max(800, static_cast<int>(WIDTH)); //((WIDTH < 800) ? 800 : WIDTH) // 1280
		SCREEN_HEIGHT = std::max(600, static_cast<int>(HEIGHT)); //((HEIGHT < 600) ? 600 : HEIGHT) // 720
		SCREEN_SIDE = int((SCREEN_WIDTH + SCREEN_HEIGHT) / 2); // 720

		DIST = DISTANCES_SEGMENTS / (tan(FOV / 2) * 2) * 7 * SCREEN_WIDTH / SCREEN_HEIGHT * SCREEN_WIDTH / DISTANCES_SEGMENTS;
		MONITOR_TILE = int(static_cast<float>(SCREEN_WIDTH) / DISTANCES_SEGMENTS);
		CONVEX_NUMBER = int(DISTANCES_SEGMENTS / 10.f);
	}

	void clean_file(const std::string& filename) {
		std::ofstream out(filename);

		out << "SCREEN_WIDTH = " << WIDTH << '\n';
		out << "SCREEN_HEIGHT = " << HEIGHT << '\n';
		out << "QUALITY = " << (WIDTH / DISTANCES_SEGMENTS) << '\n';
		out << "FPS = " << FPS << '\n';

		out.close();
	}

	void loadSettings(const std::string& filename)
	{
		std::fstream file(filename);
		std::string temp;

		int counter = 0;

		while (std::getline(file, temp)) {
			std::istringstream iss(temp);
			std::string key;
			int temp1;

			if (iss >> key >> temp) {
				if (temp == "=") {
					if (key == "SCREEN_WIDTH") iss >> WIDTH;
					else if (key == "SCREEN_HEIGHT") iss >> HEIGHT;
					else if (key == "QUALITY") { iss >> temp1; DISTANCES_SEGMENTS = WIDTH / std::max(1, int(temp1 / 2.f) * 2); }
					else if (key == "FPS") iss >> FPS; 
				}
				updateSettings();
			}
			++counter;
		}

		if (counter == 0 || counter < 4) {
			clean_file(filename);
		}

		file.close();
	}

	std::string getBaseName(const std::string& path) {
		size_t slash1 = path.find_last_of("/\\");
		size_t slash2 = path.find_last_of("/\\", slash1 - 1);

		if (slash2 == std::string::npos) {
			slash2 = 0;
		}
		else {
			slash2 += 1;
		}
		return path.substr(slash2, slash1 - slash2);
	}


	bool checkptr(sf::Texture& obj, sf::Texture* ptr) {
		if (ptr != nullptr) {
			obj = *ptr;
			return true;
		}

		const std::string path = BUILDED ? "../../../Textures/TroubleShooting/without_texture.png" : "Textures/TroubleShooting/without_texture.png";
		if (!std::filesystem::exists(path)) {
			sf::RenderTexture surface;
			unsigned int width = 800, height = 600;
			surface.create(width, height);

			sf::Font font;
			if (!font.loadFromFile("C:/Windows/Fonts/arialbd.ttf")) {
				std::cerr << "Error with loading a font!!!" << std::endl;
			}

			sf::Text text("Without Texture", font, 100);
			text.setFillColor(sf::Color(255, 0, 0, 255));
			sf::FloatRect text_bounds = text.getLocalBounds();
			text.setPosition(sf::Vector2f((width - text_bounds.width) / 2.f, height / 2.f - text.getCharacterSize() / 2.f));

			surface.clear(sf::Color(200, 200, 200));
			surface.draw(text);
			surface.display();

			sf::Texture rendered_texture = surface.getTexture();
			sf::Image rendered_image = rendered_texture.copyToImage();
			rendered_image.saveToFile(path);

			obj = rendered_texture;
		}
		else {
			sf::Texture texture;
			texture.loadFromFile(path);
			obj = texture;
		}

		return false;
	}

	bool checkptr(sf::Sound& obj, sf::SoundBuffer* ptr) {
		if (ptr) {
			obj.setBuffer(*ptr);

			return true;
		}

		obj.setBuffer(sf::SoundBuffer());
		return false;
	}
}