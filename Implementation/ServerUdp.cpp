#include "ServerUdp.h"

bool ServerUdp::check_error(const Socket::Status& status, const IpAddress& ip) {
	if (_players.count(ip)) {
		if (status != Socket::Status::Done) {
			++_players[ip].first.m_errors;
			return true;
		}
		else {
			_players[ip].first.m_errors = 0;
		}
	}
	return false;
}

void ServerUdp::disconnectPlayer(const IpAddress& ip)
{
	Packet packet;
	packet << MsgType::Disconnect;
	_socket.send(packet, ip, _players[ip].first.m_port);

	_world.removeObject2D("Player " + ip.toString());
	_players.erase(ip);
}

map<IpAddress, pair<IData, shared_ptr<Camera>>>::iterator ServerUdp::disconnectPlayer(map<IpAddress, pair<IData, shared_ptr<Camera>>>::iterator it)
{
	Packet packet;
	packet << MsgType::Disconnect;
	_socket.send(packet, it->first, it->second.first.m_port);

	_world.removeObject2D("Player " + it->first.toString());
	return _players.erase(it);
}

IData ServerUdp::_find(Camera* ptr)
{
	auto it = find_if(_players.begin(), _players.end(), [ptr](const pair<IpAddress, pair<IData, shared_ptr<Camera>>> a) {return a.second.second.get() == ptr; });
	return ((it != _players.end()) ? it->second.first : IData());
}

ServerUdp::ServerUdp(World& world, shared_ptr<Camera>& player, const IpAddress& ip, const unsigned short& port) : _world(world), _localPlayer(player), server_port(port)
{
	_ip = IpAddress::getLocalAddress();
	g_ip = IpAddress::getPublicAddress(milliseconds(5000));
	binded = false;

	_socket.setBlocking(false);
}

ServerUdp::~ServerUdp()
{
	if(this->isBinded())
		stop();

	/*for (auto& p : _players) {
		p.second.second.reset();
	}*/
}

void ServerUdp::start()
{
	check_sock(_socket.bind(server_port, _ip), 1, fstring("Cannot bind the server to port: {}, and ip: {} | in ServerUdp class and line 20\n", { to_string(server_port), _ip.toString() }));
	binded = true;
	_ip = IpAddress::getLocalAddress();
	g_ip == IpAddress::None;

	if (DEBUG)
		cout << "Started g_ip Loop" << "	";
	while (g_ip == IpAddress::None) {
		g_ip = IpAddress::getPublicAddress(milliseconds(5000));
	}
	if (DEBUG) {
		cout << "Ended g_ip Loop" << "	";
		cout << "Server Started" << endl;
	}
}

void ServerUdp::process()
{
	if (!binded) { return; }

	Packet recv_packet, send_packet, extra_packet;
	IpAddress _address;
	unsigned short _port;

	if (check_error(_socket.receive(recv_packet, _address, _port), _address) || _address == IpAddress::None || _address == IpAddress::getLocalAddress()) { return; }

	int type;
	double dir;
	Point2D pos, temp_pos;
	recv_packet >> type;

	if (_localPlayer->getShoot()) {
		pair<vector<Camera*>::iterator, vector<Camera*>::iterator> iters = _localPlayer->cameraRayCheck();

		for (auto& it = iters.first; it != iters.second;) {
			Camera* ptr = *(it++);
			IData temp_i = _find(ptr);
			if (temp_i.m_ip != IpAddress::None) {
				send_packet.clear();
				send_packet << MsgType::Shoot << ptr->position() << ptr->getDeaths();

				_socket.send(send_packet, temp_i.m_ip, temp_i.m_port);
			}
		}
		if (type != int(MsgType::Shoot)) { return; }
	}

	switch ((MsgType)type) {
	case MsgType::Connect:
		temp_pos = { _localPlayer->getStartPos() + Point2D(((int)_players.size() % 2) ? 400. : -400., 0) * ((int)_players.size() + 1) };
		pos = Point2D(temp_pos - _localPlayer->getStartPos()).normalized();
		dir = Point2D(pos.y, -pos.x).vect2Rad();
		send_packet << MsgType::WorldUpdate << _address.toString() << temp_pos << dir;

		for (auto& player : _players) {
			if (player.first != _address) {
				_socket.send(send_packet, player.first, player.second.first.m_port);
			}
			else {
				extra_packet << MsgType::WorldUpdate << _ip << _localPlayer->position() << _localPlayer->getRotation();
				_socket.send(extra_packet, _address, _port);
			}
		}

		_players.emplace(_address, pair<IData, shared_ptr<Camera>>( {_address, _port}, shared_ptr<Camera>(new Camera(_world, temp_pos))));
		_world.addObject2D(_players[_address].second, "Player " + _address.toString());

		extra_packet.clear();
		extra_packet << MsgType::Accept << temp_pos << dir;
		_socket.send(extra_packet, _address, _port);

		break;

	case MsgType::Update:
		recv_packet >> pos >> dir;
		send_packet << MsgType::Update << _address << pos << _players[_address].second->health() << dir;
		extra_packet << MsgType::Update << _ip << _localPlayer->position() << _localPlayer->health() << _localPlayer->getRotation() << _players[_address].second->health();
		if (_players.count(_address)) {
			_players[_address].second->setPosition(pos);
			_players[_address].second->updateFrame();
			_players[_address].second->setDirection(dir);
		}

		for (auto& it = _players.begin(); it != _players.end();) {
			auto& ip = it->first;
			auto& data = it->second.first;

			if (ip != _address) {
				_socket.send(send_packet, ip, data.m_port);
			}
			else {
				_socket.send(extra_packet, ip, data.m_port);
			}

			if (data.m_errors >= 60) {
				it = disconnectPlayer(it);
			}
			else {
				++it;
			}
		}

		break;

	case MsgType::Shoot:
		_players[_address].second->updateFrame();
		_players[_address].second->cameraRayCheck();

		extra_packet << MsgType::Kills << _players[_address].second->getKills();

		_socket.send(extra_packet, _address, _players[_address].first.m_port);

		break;

	case MsgType::Disconnect:
		disconnectPlayer(_address);
		break;
	}

	// finishing

	

}

void ServerUdp::stop()
{
	binded = false;
	_socket.unbind();

	for (auto& it = _players.begin(); it != _players.end();) {
		it = disconnectPlayer(it++);
	}

	if(DEBUG)
		cout << "Server Stopped" << endl;
}

bool ServerUdp::isBinded() const
{
	return binded;
}

IpAddress ServerUdp::getGlobalIp() const
{
	return g_ip;
}

