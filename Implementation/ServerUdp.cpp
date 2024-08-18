#include "ServerUdp.h"

ServerUdp::ServerUdp(World& world, shared_ptr<Player> player, const IpAddress& ip, const unsigned short& port) : _world(world), _localPlayer(player), _port(port)
{
	_ip = IpAddress::getLocalAddress();
	g_ip = IpAddress::getPublicAddress(milliseconds(5000));
	binded = false;

	_socket.setBlocking(false);
}

ServerUdp::~ServerUdp()
{
	_socket.unbind();

	for (auto&& p : _players) {
		p.second.reset();
	}
}

void ServerUdp::start()
{
	check_sock(_socket.bind(_port, _ip), 1, fstring("Cannot bind the server to port: {}, and ip: {} | in ServerUdp class and line 20\n", { to_string(_port), _ip.toString() }));
	binded = true;
	_ip = IpAddress::getLocalAddress();
	g_ip = IpAddress::getPublicAddress(milliseconds(5000));

	cout << "Server Started" << endl;
}

void ServerUdp::process()
{
	if (!binded) { return; }

	Packet recv_packet, send_packet, extra_packet;
	IpAddress _address;
	unsigned short _port;

	if (!check_sock(_socket.receive(recv_packet, _address, _port))) { return; }

	int type;
	double buf[2];
	Point2D pos;
	shared_ptr<Player> player;
	recv_packet >> type;

	switch ((MsgType)type) {
	case MsgType::Connect:
		send_packet << MsgType::WorldUpdate << _address.toString() << recv_packet;
		for (auto&& player : _players) {
			_socket.send(send_packet, player.first, _port);
		}
		recv_packet >> pos >> buf[0] >> buf[1];
		//cout << "data received	" << pos.to_str() << endl;

		player = make_shared<Player>(pos, buf[1], buf[0]);
		_players.emplace(_address, player);
		_world.addObject2D(player, "Player " + _address.toString());

		extra_packet << MsgType::Accept;
		_socket.send(extra_packet, _address, _port);

		send_packet.clear();
		send_packet << MsgType::WorldUpdate << _ip.toString() << _localPlayer->position() << _localPlayer->health() << _localPlayer->height();
		_socket.send(send_packet, _address, _port);

		break;

	case MsgType::Update:
		double health;
		send_packet << MsgType::Update << _address.toString() << recv_packet;
		extra_packet << MsgType::Update << _ip.toString() << _localPlayer->position() << _localPlayer->health();
		recv_packet >> pos >> health;
		if (_players.count(_address)) {
			_players[_address]->setPosition(pos);
			_players[_address]->setHealth(health);
		}

		/*cout << "Updated" << endl;*/

		for (auto&& p : _players) {
			if (p.first != _address) {
				_socket.send(send_packet, p.first, _port);
			}
			else {
				_socket.send(extra_packet, p.first, _port);
			}
		}
		break;

	case MsgType::Disconnect:
		_players.erase(_address);
		_world.removeObject2D("Player " + _address.toString());
	}

}

void ServerUdp::stop()
{
	binded = false;
	_socket.unbind();

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

