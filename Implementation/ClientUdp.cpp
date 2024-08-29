#include "ClientUdp.h"

bool ClientUdp::check_errors(const Socket::Status& status)
{
	if (status != Socket::Status::Done) {
		if (errors >= 60) {
			disconnect();
		}
		else {
			errors++;
		}
		return true;
	}
	errors = 0;
	accepted = true;
	return false;
}

ClientUdp::ClientUdp(World& world, const shared_ptr<Camera>& player, const IpAddress& ip, const unsigned short& port) : _world(world), _localPlayer(player), _ip(ip), _port(port), connected(false), errors(0), accepted(true)
{
	_socket.setBlocking(false);
}

void ClientUdp::connect() {
	IpAddress ip;
	unsigned short port;

	Packet packet;
	packet << MsgType::Connect;

	_socket.send(packet, _ip, _port);
	//cout << "data sended	" << _localPlayer->getStartPos().to_str() << endl;

	packet.clear();
	_socket.receive(packet, ip, port);

	int type;
	double dir;
	Point2D pos;
	packet >> type;

	if ((MsgType)type == MsgType::Accept) {
		packet >> pos >> dir;
		connected = true;
		_localPlayer->setPosition(pos);
		_localPlayer->setStartPos(pos);
		_localPlayer->setDirection(dir);
	}
}

void ClientUdp::disconnect()
{
	Packet packet;
	packet << MsgType::Disconnect;

	_socket.send(packet, _ip, _port);

	connected = false;
	errors = 0;

	for (auto& el : _players) {
		_world.removeObject2D("Player " + el.first.toString());
	}

	_localPlayer->setDeaths(0);
	_localPlayer->setKills(0);
	_localPlayer->setHealth(_localPlayer->getStartHealth());
	_localPlayer->setPosition(_localPlayer->getStartPos());
}

void ClientUdp::process()
{
	if (!connected) { return; }

	IpAddress ip;
	unsigned short port;

	Packet send_packet, recv_packet, extra_packet;
	if (_localPlayer->getShoot()) {
		send_packet << MsgType::Shoot;
	}
	else {
		send_packet << MsgType::Update << _localPlayer->position() << _localPlayer->getRotation();
	}

	//cout << accepted << endl;
	if (accepted) {
		_socket.send(send_packet, _ip, _port);
		accepted = false;
	}

	if (errors >= 45) { accepted = true; }
	if (check_errors(_socket.receive(recv_packet, ip, port))) { return; }

	int type, temp;
	double n_dir, n_health;
	recv_packet >> type;

	IpAddress n_ip;
	Point2D n_pos;

	switch ((MsgType)type) {
	case MsgType::WorldUpdate:

		recv_packet >> n_ip >> n_pos >> n_dir;

		_players.emplace(n_ip, shared_ptr<Camera>(new Camera(_world, n_pos)));
		_players[n_ip]->setDirection(n_dir);
		_world.addObject2D(_players[n_ip], "Player " + n_ip.toString());

		break;
	case MsgType::Update:
		recv_packet >> n_ip >> n_pos >> n_health >> n_dir;

		if (_players.count(n_ip)) {
			_players[n_ip]->setPosition(n_pos);
			_players[n_ip]->setHealth(n_health);
			_players[n_ip]->setDirection(n_dir);
			_players[n_ip]->rotation(n_dir);
		}
		if (n_ip == _ip) {
			recv_packet >> n_health;
			_localPlayer->setHealth(n_health);
		}
		break;

	case MsgType::Shoot:
		recv_packet >> n_pos >> temp;

		_localPlayer->setPosition(n_pos);
		_localPlayer->setDeaths(temp);

		break;

	case MsgType::Kills:
		recv_packet >> temp;
		_localPlayer->setKills(temp);

		break;

	case MsgType::Disconnect:
		disconnect();

		break;
	}
}

bool ClientUdp::isConnected() const
{
	return connected;
}

ClientUdp::~ClientUdp()
{
	/*for (auto& p : _players) {
		p.second.reset();
	}*/
	disconnect();
}