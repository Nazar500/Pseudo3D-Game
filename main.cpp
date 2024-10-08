#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "Implementation/ClientUdp.h"
#include "Implementation/ServerUdp.h"
#include "Implementation/Menu.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

using namespace std;
using namespace Settings;

string format_value(const double& value, const char& fill, const size_t& precision, const size_t& len) {
    ostringstream oss;
    oss << fixed << setprecision(precision) << setw(len + precision+1) << setfill(fill) << value;
    return oss.str();
}

void draw_load(RenderWindow& sc, const Font& f, const Text::Style& style, const string& text) {
    sc.clear({ 255, 255, 255 });

    Text t(text, f, static_cast<int>(SCREEN_SIDE / 8.f));
    t.setFillColor({ 0, 0, 0 });
    t.setStyle(style);
    t.setPosition(static_cast<float>(int((SCREEN_WIDTH - t.getLocalBounds().width) / 2.f)), static_cast<float>(int(SCREEN_HEIGHT / 2.f - t.getCharacterSize() / 2.f)));

    sc.draw(t);
    sc.display();
}

void updateCam(Camera* cam, vector<pair<string, bool>>& settings, double sens, bool& online) {
    if(cam->getTextured() != settings[0].second)
        cam->setTextured(settings[0].second);

    if (cam->getCollision() != settings[1].second)
        cam->setCollision(settings[1].second);

    if (cam->get2D_map() != settings[2].second)
        cam->set2D_map(settings[2].second);

    if (online != settings[3].second) {
        online = settings[3].second;
        cam->setOnline(online);
    }

    if (cam->getSounds() != settings[4].second)
        cam->setSounds(settings[4].second);

    if (cam->getMusic() != settings[5].second)
        cam->setMusic(settings[5].second);

    if (cam->getSensivity() != sens) 
        cam->setSensivity(sens);
}

void clean_m_file(const std::string& filename) {
    ofstream out(filename);

    out << IpAddress::LocalHost << "\n";
    out << 60000 << "\n";
}

void InitNet(unique_ptr<ServerUdp>& server, unique_ptr<ClientUdp>& client, World& world, shared_ptr<Camera>& camera, bool& isServer, int safe_counter = 0) {
    ifstream file(CONNECT_FILE, ifstream::in);

    IpAddress ip;
    unsigned short port;

    if (DEBUG)
        cout << "Started Reading from File  ";

    string temp, temp1;
    if (file.is_open()) {
        getline(file, temp);
        getline(file, temp1);
        if ((temp == "" || temp == "\n") || (temp1 == "" || temp1 == "\n") && safe_counter < 10) {
            file.close();
            clean_m_file(CONNECT_FILE);
            InitNet(server, client, world, camera, isServer, safe_counter+1);
            return;
        }
        ip = temp;
        port = (unsigned short)stoi(temp1);
    }
    else {
        file.close();
        clean_m_file(CONNECT_FILE);
        InitNet(server, client, world, camera, isServer, safe_counter + 1);
        return;
    }

    file.close();
    
    if (DEBUG)
        cout << "Ended Reading from File\n";

    if (ip == IpAddress::LocalHost) {
        server = make_unique<ServerUdp>(world, camera, ip, port);
        isServer = true;
    }
    else {
        client = make_unique<ClientUdp>(world, camera, ip, port);
        isServer = false;
    }
}

int main()
{
#ifdef _WIN32
    HWND hWnd = GetConsoleWindow();
    #ifdef _DEBUG
        ShowWindow(hWnd, 1);
    #else
        if (!CONSOLE) {
            ShowWindow(hWnd, 0);
        }
        else {
            ShowWindow(hWnd, 1);
        }
    #endif
#endif

    srand((unsigned int)time(0));

    loadSettings("Config.txt");

    ofstream log("log.txt");
    streambuf* old = nullptr;

    if (LOG && DEBUG) {
        old = cout.rdbuf();
        cout.rdbuf(log.rdbuf());
    }

    // Window should be created first because of drawing context.
    if (DEBUG)
        cout << "Started Loading Font ";
    Font font;
    if (!font.loadFromFile(FONT)) {
        #ifdef _WIN32
            font.loadFromFile("C:Windows/Fonts/calibri.ttf");
        #else
            font.loadFromFile("usr/share/fonts/calibri.ttf");
        #endif
    }

    if (DEBUG)
        cout << "Ended Loading Font\n";

    Text::Style text_style = Text::Style::Bold;

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Pseudo3DEngine", (VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT) == VideoMode::getDesktopMode()) ? Style::Fullscreen : Style::Default);
    sf::Vector2i window_position;
    sf::Vector2i window_size;
    sf::Rect<int> window_rect;

    window.setFramerateLimit(FPS);

    // Menu Init
    draw_load(window, font, text_style, "Intializing...");
    Menu menu(font, text_style, static_cast<unsigned char>(SCREEN_SIDE / 20.f));

    // World Init
    World world;

    // Camera Init
    std::shared_ptr<Camera> camera(new Camera(world, { SIDE / 2, -SIDE / 2 }));

    if(DEBUG)
        cout << "Ended Configuring Camera\n";

    //std::shared_ptr<Camera> test(new Camera(world, { SIDE / 3, SIDE / 4 }, 0., 0.6, 5000.));

    // Network Init
    bool b_serv = false, online = NETWORK;

    unique_ptr<ClientUdp> client;
    unique_ptr<ServerUdp> server;

    if (DEBUG)
        cout << "Started Configuring Network    ";

   // for(int i = 0; i < 1; i++)
    if(NETWORK)
        InitNet(server, client, world, camera, b_serv);

    if (DEBUG)
        cout << "Ended Configuring Network\n";

    sf::Mouse::setPosition((Point2D(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2) + Point2D(window.getPosition())).to_sfi());
    window.setMouseCursorVisible(true);

    // world
    world.addObject2D(camera, "Main Player");
    //world.addObject2D(test, "");

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
    world.addObject2D(std::make_shared<Object2D>(Object2D({ SIDE * 0.4, SIDE / 6 }, { {0, 0}, {SIDE / 5, 0}, {SIDE / 5, SIDE / 5} }, 1., 0, IMAG_INSRUCTION, 1)), "triangle1");
    world.addObject2D(std::make_shared<Object2D>(Object2D({ SIDE * 0.4, SIDE / 3 }, { {0, 0}, {SIDE / 5, SIDE / 5}, {0, SIDE / 2.5} }, 1., 1)), "triangle2"); // {0, SIDE / 2.5}, {SIDE / 5, SIDE / 5}

    world.addObject2D(std::make_shared<Object2D>(Object2D({ SIDE * 0.8, SIDE / 6 }, { {0, 0}, {SCALE, 0}, {SCALE, SCALE}, {0, SCALE} }, 0.5)), "cube1");
    world.addObject2D(std::make_shared<Object2D>(Object2D({ SIDE * 0.8, SIDE * 0.5 }, { {0, 0}, {SCALE * 3, 0}, {SCALE * 3, SCALE * 3}, {0, SCALE * 3} }, 1)), "cube2");
    world.addObject2D(std::make_shared<Object2D>(Object2D({ SIDE * 0.60, SIDE * 0.73 }, { {0, 0}, {SCALE * 3, 0}, {SCALE * 3, SCALE * 3}, {0, SCALE * 3} })), "cube3");
    world.addObject2D(std::make_shared<Object2D>(Object2D({ SCALE * 3, SIDE * 0.60 }, { {0, 0}, {SIDE * 0.22, 0}, {SIDE * 0.22, SIDE * 0.22}, {0, SIDE * 0.22} }, 1, 0, WALL_TEXTURE2, 1)), "cube4");

    // 2d objs
    world.addObject2D(std::make_shared<FlatObject>(FlatObject({ SIDE * 0.25, SIDE * 0.25 })), "BALDI");

    camera->SoundsPause();

    sf::Clock dt_clock;

    // test
    //updateCam(test.get(), menu.getSettings(), test->getSensivity(), online);

    // Game loop
    if(DEBUG)
        cout << "Started Main Loop" << "	";
    while (window.isOpen())
    {
        double d_elapsedTime = dt_clock.restart().asSeconds();

        window_position = { window.getPosition() };
        window_size = { static_cast<int>(window.getSize().x), static_cast<int>(window.getSize().y) };
        window_rect = { window_position.x, window_position.y+30, window_size.x, window_size.y };

        //std::string title = "Pseudo3DEngine ";

        //// Title update
        //if (d_elapsedTime > 0) {
        //    title += format_value(static_cast<double>(1 / d_elapsedTime, '0', 3, 3) + "fps.";
        //}
        //if (camera)
        //    title += " x:" + std::to_string(camera->x()) + ", y:" + std::to_string(camera->y()) + ", health: " + std::to_string(camera->health()) + ", kills: " + std::to_string(camera->getKills()) + ", deaths: " + std::to_string(camera->getDeaths());
        //window.setTitle(title);

        if (b_serv) {
            if (online) {
                if (!server->isBinded()) {
                    server->start();
                    draw_load(window, font, text_style, "Restarting...");
                }
            }
            else {
                if (server && server->isBinded()) {
                    server->stop();
                }
            }
        }
        else {
            if (online) {
                if (!client->isConnected()) {
                    client->connect();
                }
            }
            else {
                if (client && client->isConnected()) {
                    client->disconnect();
                }
            }
        }
        // Close event search
        sf::Event event;

        if(DEBUG)
            cout << "Started pollEvent" << "	";
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyReleased) {
                switch (event.key.code)
                {
                case sf::Keyboard::Key::Escape:
                    window.setMouseCursorVisible(true);
                    camera->SoundsPause();

                    menu.to_main();
                    break;
                }
            }
        }
        if(DEBUG)
            cout << "Ended pollEvent" << endl;

        // Actually game
        if (!menu.getState())
            window.close();
        else if (menu.getState() == Tabs::Play) {
            window.clear();
            window.setMouseCursorVisible(false);
            camera->SoundsResume();
            if (MIRROR_DEBUG) {
                world.draw(window);
                camera->draw_map(window);
                camera->startFrameProcessing();
                //camera->drawCameraView(window);
                camera->endFrameProcessing();
            }
            else {
                //test->startFrameProcessing();
                //test->drawCameraView(window, static_cast<int>(d_elapsedTime * 1000));
                //test->endFrameProcessing();

                if (DEBUG)
                    cout << "Started Frame Processing" << " ";

                camera->startFrameProcessing();
                camera->endFrameProcessing();

                if(DEBUG)
                    cout << "Ended Frame Processing" << endl;

                if (DEBUG)
                    cout << "Started Drawing" << " ";
                camera->drawCameraView(window, static_cast<int>(d_elapsedTime * 1000), menu.getFont());
                if (DEBUG)
                    cout << "Ended Drawing" << endl;

                if (DEBUG)
                    cout << "Started Drawing World" << " ";
                

                if (camera->get2D_map()) {
                    world.draw(window);
                }

                if (DEBUG) {
                    cout << "Ended Drawing World" << endl;
                    cout << "Started Drawing Map ";
                }

                camera->draw_map(window);

                if (DEBUG)
                    cout << "Ended Drawing Map\n";
            }
        }
        else {
            window.clear(sf::Color::White);

            camera->SoundsPause();
            updateCam(camera.get(), menu.getSettings(), menu.getSensivity(), online);

            menu.update(window, Mouse::getPosition(window), (b_serv) ? server->getGlobalIp() : IpAddress::None);
        }
        window.display();

        if (window.hasFocus() && window_rect.contains(sf::Mouse::getPosition()) && menu.getState() == Tabs::Play)
            camera->keyboardControl(d_elapsedTime, window.getPosition(), window);

        if (online) {
            if (b_serv)
                server->process();
            else
                client->process();
        }
    }
    if (DEBUG) {
        cout << "Ended Main Loop" << endl;
        cout << "Finishing..." << endl;
    }

    if (!b_serv) {
        if(client)
            client->disconnect();
    }
    else {
        if(server)
            server->stop();
    }

    ResourceManager::unloadAllResources();

    camera->~Camera();

    if(LOG && DEBUG)
        cout.rdbuf(old);
    log.close();

    //SetFileAttributes("log.txt", FILE_ATTRIBUTE_HIDDEN);

    return 0;
}
