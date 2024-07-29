#include "Camera.h"

Camera::Camera(World& world, Point2D position, sf::RenderWindow& window, double vPos, double height, double health, std::string texture, double fieldOfView, double angle, double eyesHeight, double depth, double walkSpeed, double jumpSpeed, double viewSpeed, int reflection_limit)
	: W_world(world), d_eyesHeight(eyesHeight), d_depth(depth), d_walkSpeed(walkSpeed), d_jumpSpeed(jumpSpeed), d_viewSpeed(viewSpeed), Player(position, height), d_direction(angle), sc(window), d_reflection_limit(reflection_limit)
{
	// angle configuration
	setFieldOfView(fieldOfView);

	oldFrame.distances.resize(DISTANCES_SEGMENTS);
	oldFrame.collisions.resize(COLLISION_SEGMENTS);
	curFrame.distances.resize(DISTANCES_SEGMENTS);
	curFrame.collisions.resize(COLLISION_SEGMENTS);

	if (THREADED) {
		threadCount = thread::hardware_concurrency();
		threads.resize(threadCount);

		startM.lock();
		work = 1;
		finished = 0;
		for (int i = 0; i < threadCount; i++) {
			threads[i] = std::make_shared<std::thread>(&Camera::updateThread, this, i, threadCount, std::ref(window));
		}
		startM.unlock();
		startCV.notify_all();
	}

	// 2d map
	if (b_2d_map) {
		fov.setPointCount(CONVEX_NUMBER + 1);
	}
}

//Camera::Camera(const Camera& other) : Player(other), W_world(other.W_world), sc(other.sc)
//{
//	this->b_collision = other.b_collision;
//	this->b_hadFocus = other.b_hadFocus;
//	this->b_mirror = other.b_mirror;
//	this->b_textured = other.b_textured; this->b_2d_map = other.b_2d_map;
//
//	this->d_depth = other.d_depth;
//	this->d_aspectRatio = other.d_aspectRatio;
//	this->d_eyesHeight = other.d_eyesHeight;
//	this->d_fieldOfView = other.d_fieldOfView;
//	this->d_jumpSpeed = other.d_jumpSpeed;
//
//	this->d_viewSpeed = other.d_viewSpeed;
//	this->d_walkSpeed = other.d_walkSpeed;
//
//	this->p_pos = other.p_pos;
//}

Camera::~Camera() {

	startM.lock();
	work = -1;
	startM.unlock();

	startCV.notify_all();

	for (auto t : threads)
		if (t->joinable())
			t->join();
}

 void Camera::setThreadAffinity(std::thread& thread, int core_id) {
 	HANDLE handle = thread.native_handle();
 	DWORD_PTR mask = static_cast<DWORD_PTR>(1) << core_id;
 	SetThreadAffinityMask(handle, mask);
}

void Camera::updateThread(int i, int n, sf::RenderTarget& window)
{
	int step1 = static_cast<int>(curFrame.distances.size() / n);
	int step2 = static_cast<int>(HIDDEN_SEGMENTS / n);

	int from1 = i * step1;
	int from2 = i * step2;
	int to1 = from1 + step1;
	int to2 = from2 + step2;
	
	if (i == n - 1)
	{
		to1 = static_cast<int>(curFrame.distances.size());
		to2 = HIDDEN_SEGMENTS;
	}

	signed char lastWork = 1;
	std::unique_lock<std::mutex> lk(startM);
	while (true)
	{
		startCV.wait(lk, [this, lastWork] {return work != lastWork; });
		lastWork = work;
		lk.unlock();

		// Camera destructor
		if (work == -1)
			return;

		// Work
		updateDistances(from1, to1);
		updateHiddenDistances(from2, to2);

		// Notify main thread about finished work
		++finished;
		endCV.notify_all();
		lk.lock();
	}
}

void Camera::startFrameProcessing()
{
	srand(static_cast<unsigned int>(time(NULL)));

	signed char newWork = 1 - work;
	if (THREADED) {
		finished = 0;
		work = newWork;
		startCV.notify_all();
	}
	else {
		updateDistances(0, DISTANCES_SEGMENTS);
		updateHiddenDistances(0, HIDDEN_SEGMENTS);
	}
}

void Camera::endFrameProcessing()
{
	if (THREADED) {
		std::unique_lock<std::mutex> lk(endM);
		while (finished != threadCount) {
			endCV.wait(lk, [this] { return finished == threadCount; });
		}
	}
	std::swap(oldFrame, curFrame);
	oldFrame.direction = d_direction;
	oldFrame.position = p_pos;

	// check for exceptions

	//if (std::fetestexcept(FE_DIVBYZERO)) { throw std::runtime_error("Error"); }
}

void Camera::keyboardControl(double dt, sf::Vector2i position) {
	double sina = static_cast<double>(sin(this->d_direction));
	double cosa = static_cast<double>(cos(this->d_direction));

	double dx = 0, dy = 0;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
		dx += cosa;
		dy += sina;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
		dx -= cosa;
		dy -= sina;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
		dx += sina;
		dy -= cosa;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		dx -= sina;
		dy += cosa; 
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
		dx -= sina;
		dy -= cosa;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
		dx += sina;
		dy += cosa;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
		this->d_eyesHeight *= 1.01;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
		this->d_eyesHeight *= 0.99;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
		this->d_walkSpeed = temp_walkSpeed * 2;
	}
	else
		this->d_walkSpeed = temp_walkSpeed;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		if (weapon.shoot()) {
			this->fire();
		}
	}

	// mouse 
	Point2D position_p(position);
	mouse = (Point2D(sf::Mouse::getPosition()) - Point2D(position_p)).to_sfi();

	int offset = mouse.x - SCREEN_WIDTH / 2;
	d_direction += offset * d_viewSpeed;

	sf::Mouse::setPosition((Point2D(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2) + Point2D(position_p)).to_sfi());

	// update
	Point2D move(dx, dy);

	move.normalize();
	move *= d_walkSpeed * dt;

	shift_col(move);

	//lookAt("player1");

	//d_elapsedTime = dt;
	//d_eyesHeight = min(5., max(d_eyesHeight, 0.15));
}

void Camera::lookAt(const std::string& name)
{
	Object2D* obj = W_world.findObject2D(name).get();
	Point2D obj_middle = obj->position() + obj->loc_middle();
	d_direction = atan2(obj_middle.y - y(), obj_middle.x - x());
}

void Camera::fire()
{
	pair<Point2D, Point2D> ray = { position(), Point2D(x() + d_depth * cos(oldFrame.direction), y() + d_depth * sin(oldFrame.direction)) };
	vector<RayCastStructure> v_rayCast;

	objectsRayCrossed(ray, v_rayCast, this);

	if (!v_rayCast.empty()) {
		RayCastStructure rayCast = v_rayCast[v_rayCast.size() - 1];
		Object2D* obj = rayCast.object;
		Player* playerObj = dynamic_cast<Player*>(obj);
		if (playerObj != nullptr) {
			//double height = (obj->height() == 0.5) ? 2. : 0.5f;
			//obj->setHeight(height);

			double damage = weapon.getDamage() / max((double)(rayCast.distance / d_depth * 15), (double)1);

			
			if(playerObj->reduceHealth(damage, oldFrame.direction)){
				this->oneMoreKill();
			}
		}
	}
}

void Camera::setTextured(bool active)
{
	b_textured = active;
}

bool Camera::getTextured()
{
	return b_textured;
}

void Camera::setCollision(bool active)
{
	b_collision = active;
}

bool Camera::getCollision()
{
	return b_collision;
}

void Camera::set2D_map(bool active)
{
	b_2d_map = active;
}

bool Camera::get2D_map() {
	return b_2d_map;
}

void Camera::setFieldOfView(double angle)
{
	if (angle < 0)
		d_fieldOfView = PI / 4;
	else if (angle >= PI * 2)
		d_fieldOfView = PI * 2;
	else
		d_fieldOfView = angle;

	for (int i = 0; i < DISTANCES_SEGMENTS; i++)
	{
		double halfWidth = tan(d_fieldOfView / 2) * ((double)SCREEN_WIDTH / SCREEN_HEIGHT);
		double offset = ((i * 2.0 / (DISTANCES_SEGMENTS - 1.0)) - 1.0) * halfWidth;
		Point2D dir = { 1, 1 * offset };
		dir = dir.normalize();
		double angle = atan2(dir.y, dir.x);
		horizontalCos[i] = cos(angle);
		horizontalSin[i] = sin(angle);
	}
	for (int i = 0; i < SCREEN_HEIGHT; i++)
	{
		double halfWidth = tan(d_fieldOfView / 2);
		double offset = ((i * 2.0 / SCREEN_HEIGHT) - 1.0) * halfWidth;
		Point2D dir = { 1, 1 * offset };
		dir.normalize();
		verticalTan[i] = tan(atan2(dir.y, dir.x));
	}
}

void Camera::shift_col(Point2D vector)
{
	if (!b_collision) {
		shift(vector);
		return;
	}

	for (auto& c : oldFrame.collisions)
	{
		if (dynamic_cast<FlatObject*>(c.object) != nullptr) { continue; }
		Point2D edgeVector = c.edge.second - c.edge.first;
		Point2D normal = { -edgeVector.y, edgeVector.x };
		normal.normalize();

		Point2D toWallVector = c.edge.first + c.edge.second - p_pos * 2;
		if (normal * toWallVector > 0)
			normal = normal * -1;

		double scalar = vector * normal;

		if (scalar < 0 && abs(c.distance - abs(scalar)) < COLLISION_DISTANCE/* && vPos()/* + d_eyesHeight < c.height*/)
		{
			vector.x -= normal.x * scalar;
			vector.y -= normal.y * scalar;
		}

	}

	shift(vector);

	/*
	Point2D direction(oldFrame.direction);

	Point2D col_vector(vector);
	bool can_shift = true;
	for (auto& el : oldFrame.collisions) 
	{
		Point2D wall_vector = el.edge.second - el.edge.first;
		Point2D norm_vector = Point2D(- wall_vector.y, wall_vector.x).normalized();

		Point2D wall_dist = (el.point - oldFrame.position);
		
		if (vector.norm_length() + COLLISION_DISTANCE < wall_dist.length()) {
			can_shift = true;
		}
		else {
			can_shift = false;
			//shift(wall_vector.normalized());

			cout << (oldFrame.position + vector).length() << " " << wall_dist.length() << endl;

			break; 
		}
	}

	if (can_shift) {
		shift(vector);
	}
	*/
}

void Camera::shift(Point2D vector)
{
	p_pos += vector;
}


void Camera::updateHiddenDistances(int from, int to)
{
	double right = oldFrame.direction + d_fieldOfView / 2;
	for (int i = from; i < to; i++) {
		double direction = right + (2 * PI - d_fieldOfView) * i / HIDDEN_SEGMENTS;
		double cosa = cos(direction), sina = sin(direction);

		//cout << direction << " " << i << endl;

		pair<Point2D, Point2D> segment = { position(), { x() + COLLISION_AREA * cosa, y() + COLLISION_AREA * sina } };
		CollisionInfo collision;

		hiddenObjectsRayCrossed(segment, collision, this);

		curFrame.collisions[(const unsigned int)(i + SHOWN_SEGMENTS)] = collision;
	}
}

void Camera::updateDistances(int from, int to)
{
	double left = d_direction - d_fieldOfView / 2;

	// Visible for player segments
	for (int i = from; i < to; i++)
	{
		double direction = left + d_fieldOfView * i / DISTANCES_SEGMENTS;
		double cosa = cos(direction), sina = sin(direction);

		pair<Point2D, Point2D> segment1;

		segment1 = { {x(), y()}, { x() + d_depth * cosa, y() + d_depth * sina }};

		std::vector<RayCastStructure> v_rayCastStructure;
		CollisionInfo* coll = new CollisionInfo();

		// debug
		//std::cout << "Ray: " << i << "... ";

		objectsRayCrossed(segment1, v_rayCastStructure, this, coll);

		if (v_rayCastStructure.empty())
			v_rayCastStructure.emplace_back(RayCastStructure{ d_depth, 0, nullptr, 0 });
		else if (CORRECTION) {
			for (auto& el : v_rayCastStructure) {
				el.distance *= cos(direction - oldFrame.direction);
			}
		}

		curFrame.distances[i] = v_rayCastStructure;
		//cout << i << " " << (int)(i * COLLISION_SEGMENTS / DISTANCES_SEGMENTS) << endl;
		if(COLLISION_SEGMENTS > 0)
			curFrame.collisions[(int)(i * COLLISION_SEGMENTS / DISTANCES_SEGMENTS)] = *coll;

		// clean
		delete coll;

		// 2d map
		if (!b_2d_map) { continue; }
		size_t dist_index = static_cast<size_t>(i * DISTANCES_SEGMENTS / CONVEX_NUMBER);

		if (dist_index < DISTANCES_SEGMENTS && !oldFrame.distances[0].empty()) {
			size_t index = oldFrame.distances[dist_index].size() - 1;
			sf::Vector2f point(static_cast<float>(oldFrame.distances[dist_index][index].distance * cos(left + d_fieldOfView * dist_index / DISTANCES_SEGMENTS)), static_cast<float>(oldFrame.distances[dist_index][index].distance * sin(left + d_fieldOfView * dist_index / DISTANCES_SEGMENTS)));

			point.x /= MAP_SCALE;
			point.y /= MAP_SCALE;

			fov.setPoint(i + 1, point);
		}
	}
}

void Camera::objectsRayCrossed(std::pair<Point2D, Point2D> ray, std::vector<RayCastStructure>& v_raycast, const Object2D* caster, CollisionInfo* collision, int reflections) {
	std::pair<Point2D, Point2D> wall;
	Point2D point;

	// for other info
	Point2D ray_dir = Point2D(ray.second - ray.first).normalized();

	double dist;
	double closest = d_depth;
	double len = 0;

	for (auto& el : W_world.objects()) {
		FlatObject* obj_2d = dynamic_cast<FlatObject*>(el.second.get());
		Object2D* obj = el.second.get();
		if (obj == caster) { continue; }

		// update camera_angle if FlatObject crossed
		if (obj_2d) {
			obj_2d->update(oldFrame.direction);
		}

		vector<RayCastStructure> v_mirrorRayCast;
		
		if (obj->cross(ray, wall, point, len)) {
			dist = (point - ray.first).length();
			if (obj->isMirror() && reflections < d_reflection_limit) {
				Point2D wall_vector = wall.second - wall.first;
				double norm_vector = Point2D(wall_vector.y, -wall_vector.x).normalized().vect2Rad();

				double ray_dir_p = Point2D(ray.first - point).normalize().vect2Rad();

				double delta = ray_dir_p - norm_vector;

				Point2D new_ray_dir = Point2D::rad2Vect(norm_vector - delta);

				pair<Point2D, Point2D> new_ray = { {point}, {point.x + d_depth * new_ray_dir.x, point.y + d_depth * new_ray_dir.y} };

				// debug
				if (abs(ray_dir.vect2Rad() - d_direction) < 0.0001 && MIRROR_DEBUG) {
					//cout << ray_dir_p << " " << norm_vector << " " << delta << endl;

					sf::VertexArray line(sf::Lines, 2);
					sf::VertexArray line1(sf::Lines, 2);
					for (int i = 0; i < 2; i++) { line[i].color = sf::Color::Black; line1[i].color = sf::Color::Red; }
					line[0].position = (ray.first / MAP_SCALE).to_sff();
					line[1].position = (ray.second / MAP_SCALE).to_sff();
					line1[0].position = (new_ray.first / MAP_SCALE).to_sff();
					line1[1].position = (new_ray.second / MAP_SCALE).to_sff();

					unique_lock<mutex> lk(renderM);
					sc.draw(line);
					sc.draw(line1);
					lk.unlock();
				}

				
				objectsRayCrossed(new_ray, v_mirrorRayCast, obj, nullptr, reflections + 1);
				IncreaseDistance(v_mirrorRayCast, (ray.first - point).length());

				std::sort(v_mirrorRayCast.begin(), v_mirrorRayCast.end(), [](const RayCastStructure& lh, const RayCastStructure& rh) { return lh.distance > rh.distance; });

				if (abs(ray_dir.vect2Rad() - d_direction) < 0.0001 && MIRROR_DEBUG && !v_mirrorRayCast.empty()) {
					for (auto& el : v_mirrorRayCast) {
						cout << el.distance << " ";
					}
					cout << endl;
				}
	 		}
			
			v_raycast.push_back({ dist, len, obj, obj->height(), v_mirrorRayCast });

			if (dist <= COLLISION_AREA && b_collision && collision) {
				collision->point = point;
				collision->edge = move(wall);
				collision->distance = dist;
				collision->height = obj->height();
				collision->object = obj;
			}
		}
		std::sort(v_raycast.begin(), v_raycast.end(), [](const RayCastStructure& lh, const RayCastStructure& rh) { return lh.distance > rh.distance; });
	}
}

void Camera::hiddenObjectsRayCrossed(pair<Point2D, Point2D>& ray, CollisionInfo& collision, const Object2D* caster)
{
	pair<Point2D, Point2D> edge;
	Point2D point;
	double dist;
	double len = 0;

	for (auto& el : W_world.objects()) {
		Object2D* obj = el.second.get();

		if (obj == caster) { continue; }

		if (obj->cross(ray, edge, point, len)) {
			dist = (point - ray.first).length();

			if (b_collision && dist <= COLLISION_AREA) {
				collision.point = point;
				collision.edge = move(edge);
				collision.distance = dist;
				collision.height = obj->height();
				collision.object = obj;
				//cout << point.to_str() << " " << edge.first.to_str() << " " << edge.second.to_str() << " " << dist << endl;
			}
		}
	}
}

void Camera::IncreaseDistance(vector<RayCastStructure>& v_RayCastStructure, double dist)
{
	for (auto& el : v_RayCastStructure) {
		el.distance += dist;
		IncreaseDistance(el.v_mirrorRayCast, dist);
	}
}

void Camera::rayDraw(sf::RenderTarget& window, std::vector<RayCastStructure>& v_raycast, int& shift) {
	
	int index = 0;

	auto it = find_if(v_raycast.begin(), v_raycast.end(), [](const RayCastStructure& obj) { return (obj.object != nullptr && obj.object->isMirror()); });
	if (it != v_raycast.end()) {
		for (int i = 0; i < v_raycast.size(); i++) {
			if (v_raycast[i].object == nullptr) { continue; }

			if (v_raycast[i].object->isMirror()) {
				index = i;
			}
		}
	}
	
	for (int i = 0; i < v_raycast.size(); i++) {
		if (i >= index) {
			drawVerticalStrip(window, v_raycast[i], shift);
		}
		if (!v_raycast[i].v_mirrorRayCast.empty()) {
			rayDraw(window, v_raycast[i].v_mirrorRayCast, shift);
		}
	}

	/*for (int i = index; i < v_raycast.size(); i++) {
		if (shift == DISTANCES_SEGMENTS / 2) {
			cout << W_world.getObject2DName(v_raycast[i].object) << " " << i << " ";
		}
		if (shift == DISTANCES_SEGMENTS / 2 && i == v_raycast.size() - 1) {
			cout << endl << "INDEX: " << index << endl;
		}
	}*/
	
}

sf::Vector2f Camera::scaling(const sf::Vector2u& size_before, const sf::Vector2u& size_after) {
	return sf::Vector2f((float)size_after.x / size_before.x, (float)size_after.y / size_before.y);
}

sf::Vector2f Camera::scaling(const sf::IntRect& size_before, const sf::Vector2u& size_after) {
	return sf::Vector2f((float)size_after.x / size_before.width, (float)size_after.y / size_before.height);
}

void Camera::drawVerticalStrip(sf::RenderTarget& window, RayCastStructure k, int shift)
{
	if (k.object == nullptr) { return; }
	// general
	float height = static_cast<float>((DIST * SCALE / max(k.distance, (double)1)) * k.height);

	float h1;
	if (k.height <= 1) {
		h1 = static_cast<float>((SCREEN_HEIGHT / 2 - height * (k.height - 0.5)));
	}
	else {
		h1 = static_cast<float>((SCREEN_HEIGHT / 2 - height * (k.height - 0.5) / 2));
	}

	int c = static_cast<int>(std::min(static_cast<int>(255 * (1 - k.distance / d_depth)), static_cast<int>(255)));
	int col_c = (int)((int)(c / FOG_SEGMENTS) * FOG_SEGMENTS / max((double)FOG_INTENSITY, 0.00001));
	if (k.object->isMirror() && c >= 210) {
		col_c = (int)((int)(210 / FOG_SEGMENTS) * FOG_SEGMENTS / max((double)FOG_INTENSITY, 0.00001));
	}

	// without textures
	sf::ConvexShape pol;
	sf::Sprite texture;
	pol.setPointCount(4);

	pol.setPoint(0, sf::Vector2f(0, h1));
	pol.setPoint(1, sf::Vector2f(0, h1 + height));
	pol.setPoint(2, sf::Vector2f(MONITOR_TILE, h1 + height));
	pol.setPoint(3, sf::Vector2f(MONITOR_TILE, h1));

	pol.setPosition(static_cast<float>(shift * MONITOR_TILE), (float)(d_verticalShift));

	if (b_textured) {
		pol.setFillColor({ 255, 255, 255, static_cast<sf::Uint8>(max((int)(255 - col_c), 0))});
	}
	else {
		pol.setFillColor({ 255, 174, 174, static_cast<sf::Uint8>(255 - c)});
	}

	if (OUTLINE_SEGMENTS) {
		pol.setOutlineColor(OUTLINE_SEGMENTS_COLOR);
		pol.setOutlineThickness(1);
	}

	if (k.distance < d_depth) {
		if (b_textured) {
			texture.setTexture(k.object->loadTexture(), true);

			// transform
			if (texture.getTexture()->getSize() != sf::Vector2u(0, 0)) {
				if(!k.object->isMirror()) {
					texture.setColor(sf::Color(c, c, c)); //texture.setColor(Point4D(sf::Color(c, c, c)).avarage(pol.getFillColor()).to_col());
					texture.setTextureRect(sf::IntRect(static_cast<int>(texture.getTexture()->getSize().x * k.progress), 0, MONITOR_TILE, (int)(texture.getTexture()->getSize().y * k.height)));

					sf::Vector2f ratio = scaling(texture.getTextureRect(), sf::Vector2u(static_cast<unsigned int>(MONITOR_TILE), static_cast<unsigned int>(height)));
					texture.setScale(ratio);

					/*float y = (h1 + d_verticalShift) / d_eyesHeight;
					if (d_eyesHeight > 7) {
						y = (SCREEN_HEIGHT / (h1 + d_verticalShift) * d_eyesHeight);
					}*/

					texture.setPosition({ pol.getPosition().x, (float)(h1 + d_verticalShift) }); // SCREEN_HEIGHT*5-d_eyesHeight*100 - (h1 + d_verticalShift) / d_eyesHeight)

					unique_lock<mutex> lk(renderM);
					window.draw(texture);
					lk.unlock();
				}
			}
			else
			{
				for (auto& el : W_world.objects()) {
					if (el.second.get()->loadTexture().getSize() != sf::Vector2u(0, 0)) {
						return;
					}
				}
				b_textured = false;
			}
		}
		else {
			unique_lock<mutex> lk(renderM);
			window.draw(pol);
			lk.unlock();
		}
	}

	unique_lock<mutex> lk(renderM);
	if (pol.getFillColor().a > 10 && FOG_INTENSITY > 0 && b_textured) {
		window.draw(pol);
	}
	lk.unlock();
}

void Camera::draw_map(sf::RenderTarget& window)
{
	if (!b_2d_map) { return; }

	fov.setOutlineColor(FILED_OF_VEW_OUTLINE_COLOR);
	fov.setFillColor(FILED_OF_VEW_COLOR);
	fov.setOutlineThickness(OUTLINE_CAMERA_THICKNESS);
	fov.setPosition(sf::Vector2f((float)(x() / MAP_SCALE), (float)(y() / MAP_SCALE)));

	player.setRadius(static_cast<float>(SCALE / MAP_SCALE));
	player.setOrigin(sf::Vector2f(player.getRadius(), player.getRadius()));
	player.setFillColor(FILL_CAMERA_COLOR);
	player.setOutlineColor(OUTLINE_CAMERA_COLOR);
	player.setOutlineThickness(OUTLINE_CAMERA_THICKNESS);
	player.setPointCount(CIRCLE_CONVEX_NUMBER);
	player.setPosition(sf::Vector2f((float)(x() / MAP_SCALE), (float)(y() / MAP_SCALE)));

	window.draw(fov);
	window.draw(player);
}

void Camera::drawCameraView(sf::RenderTarget& window)
{
	// sky
	if (b_textured) {
		sf::Sprite sky;
		sky.setTexture(W_world.skyTexture());
		sky.setTextureRect(sf::IntRect((int)(oldFrame.direction * SCREEN_WIDTH / 2), (int)(sky.getTextureRect().height - (SCREEN_HEIGHT / 2) - 540 + (int)d_verticalShift), SCREEN_WIDTH, SCREEN_HEIGHT));
		sky.setPosition(sf::Vector2f(0, 0));

		unique_lock<mutex> lk(renderM);
		window.draw(sky);
		lk.unlock();
	}

	// obviously view
	for (int i = 0; i < DISTANCES_SEGMENTS; i++) {
		rayDraw(window, oldFrame.distances[i], i);
	}

	// weapon
	unique_lock<mutex> lk(renderM);
	weapon.draw(window);
	lk.unlock();

}

