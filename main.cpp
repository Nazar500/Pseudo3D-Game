#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "Implementation/Settings.h"
#include "Implementation/World.h"
#include "Implementation/Camera.h"
#include "Implementation/Object2D.h"
#include "Implementation/FlatObject.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

using namespace std;

string format_value(const double& value, const char& fill, const size_t& precision, const size_t& len) {
    ostringstream oss;
    oss << fixed << setprecision(precision) << setw(len + precision+1) << setfill(fill) << value;
    return oss.str();
}

int main()
{
    srand((unsigned int)time(0));

    // Window should be created first because of drawing context.

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Pseudo3DEngine");
    sf::Vector2i window_position;
    sf::Vector2i window_size;
    sf::Rect<int> window_rect;

    //window.setFramerateLimit(100);

    // World Init
    World world;
    std::shared_ptr<Camera> camera(new Camera(world, { SIDE / 2, -SIDE / 2 }, window));

    sf::Mouse::setPosition((Point2D(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2) + Point2D(window.getPosition())).to_sfi());
    window.setMouseCursorVisible(false);

    // world
    world.addObject2D(camera, "cam1");

    // walls
    world.addObject2D(std::make_shared<Object2D>(Object2D({ 0, 0 }, { {0, 0}, {SCALE, 0}, {SCALE, SCALE_WINDOW * SCALE}, {0, SCALE_WINDOW * SCALE} }, 1.)), "wall1");
    world.addObject2D(std::make_shared<Object2D>(Object2D({ SCALE_WINDOW * SCALE, 0 }, { {0, 0}, {SCALE, 0}, {SCALE, SCALE_WINDOW * SCALE}, {0, SCALE_WINDOW * SCALE} }, 1.)), "wall2");
    world.addObject2D(std::make_shared<Object2D>(Object2D({ 0, SCALE_WINDOW * SCALE }, { {0, 0}, {SCALE_WINDOW * SCALE, 0}, {SCALE_WINDOW * SCALE, SCALE}, {0, SCALE} }, 1., 0, WALL_TEXTURE1)), "wall3");

    // columns
    world.addObject2D(std::make_shared<Circle2D>(Circle2D(SCALE * 4, { 0, 0 }, 1.)), "column1");
    world.addObject2D(std::make_shared<Circle2D>(Circle2D(SCALE * 4, { 0, SCALE_WINDOW * SCALE }, 1.)), "column2");
    world.addObject2D(std::make_shared<Circle2D>(Circle2D(SCALE * 4, { SCALE_WINDOW * SCALE, 0 }, 1.)), "column3");
    world.addObject2D(std::make_shared<Circle2D>(Circle2D(SCALE * 4, { SCALE_WINDOW * SCALE, SCALE_WINDOW * SCALE }, 1.)), "column4");

    // objects
    world.addObject2D(std::make_shared<Object2D>(Object2D({ SIDE * 0.4, SIDE / 6 }, { {0, 0}, {SIDE / 5, 0}, {SIDE / 5, SIDE / 5} })), "triangle1");
    world.addObject2D(std::make_shared<Object2D>(Object2D({ SIDE * 0.4, SIDE / 3 }, { {0, 0}, {SIDE / 5, SIDE / 5}, {0, SIDE / 2.5} }, 1., 1)), "triangle2"); // {0, SIDE / 2.5}, {SIDE / 5, SIDE / 5}

    world.addObject2D(std::make_shared<Object2D>(Object2D({ SIDE * 0.8, SIDE / 6 }, { {0, 0}, {SCALE, 0}, {SCALE, SCALE}, {0, SCALE} }, 0.5)), "cube1");
    world.addObject2D(std::make_shared<Object2D>(Object2D({ SIDE * 0.8, SIDE * 0.5 }, { {0, 0}, {SCALE * 3, 0}, {SCALE * 3, SCALE * 3}, {0, SCALE * 3} }, 1)), "cube2");
    world.addObject2D(std::make_shared<Object2D>(Object2D({ SIDE * 0.60, SIDE * 0.73 }, { {0, 0}, {SCALE * 3, 0}, {SCALE * 3, SCALE * 3}, {0, SCALE * 3} })), "cube3");
    world.addObject2D(std::make_shared<Object2D>(Object2D({ SCALE * 3, SIDE * 0.60 }, { {0, 0}, {SIDE * 0.22, 0}, {SIDE * 0.22, SIDE * 0.22}, {0, SIDE * 0.22} }, 1, 0, WALL_TEXTURE2, 1)), "cube4");

    // 2d objs
    world.addObject2D(std::make_shared<FlatObject>(FlatObject({ SIDE * 0.25, SIDE * 0.25 })), "BALDI");

    bool learn = false;
    bool botView = false;

    camera->setTextured(true);
    camera->setCollision(true);
    camera->set2D_map(true);

    double dt = 0.02;
    int iterations = 0;

    sf::Clock dt_clock;

    // Game loop
    while (window.isOpen())
    {
        double d_elapsedTime = dt_clock.restart().asSeconds();

        window_position = { window.getPosition() };
        window_size = { static_cast<int>(window.getSize().x), static_cast<int>(window.getSize().y) };
        window_rect = { window_position.x, window_position.y, window_size.x, window_size.y };

        std::string title = "Pseudo3DEngine ";

        // Title update
        if (d_elapsedTime > 0) {
            title += format_value((double)1 / d_elapsedTime, '0', 3, 3) + "fps.";
        }
        if (camera != nullptr)
            title += " x:" + std::to_string(camera->x()) + ", y:" + std::to_string(camera->y()) + ", health: " + std::to_string(camera->health()) + ", kills: " + std::to_string(camera->getKills()) + ", deaths: " + std::to_string(camera->getDeaths());
        window.setTitle(title);

        // Close event search
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyReleased) {
                switch (event.key.code)
                {
                case sf::Keyboard::Key::Escape:
                    if (window_rect.contains(sf::Mouse::getPosition())) {
                        sf::Mouse::setPosition(sf::Vector2i(0, 0));
                    }
                    else {
                        window.close();
                    }
                    break;
                }
            }
        }

        // Actually game
        window.clear(sf::Color::White);
        if (MIRROR_DEBUG) {
            world.draw(window);
            camera->draw_map(window);
            camera->startFrameProcessing();
            //camera->drawCameraView(window);
            camera->endFrameProcessing();
        }
        else {
            camera->startFrameProcessing();
            camera->drawCameraView(window);
            camera->endFrameProcessing();
            
            world.draw(window);
            camera->draw_map(window);
        }
        window.display();

        // if escape was pressed
        if (window.hasFocus() && window_rect.contains(sf::Mouse::getPosition()))
            camera->keyboardControl(d_elapsedTime, window.getPosition());
    }

    ResourceManager::unloadAllResources();

    return 0;
}
