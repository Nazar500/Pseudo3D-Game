#include "ClientUdp.h"

ClientUdp::ClientUdp(World& world, shared_ptr<Player>& player, const IpAddress& ip, const unsigned short& port) : _world(world), _localPlayer(player), _ip(ip), _port(port), connected(false)
{
	_socket.setBlocking(false);
}

void ClientUdp::connect() {
	IpAddress ip;
	unsigned short port;

	Packet packet;
	packet << MsgType::Connect << _localPlayer->getStartPos() << _localPlayer->health() << _localPlayer->height();

	_socket.send(packet, _ip, _port);
	//cout << "data sended	" << _localPlayer->getStartPos().to_str() << endl;

	packet.clear();
	_socket.receive(packet, ip, port);

	int type;
	packet >> type;

	if ((MsgType)type == MsgType::Accept) {
		connected = true;
		//cout << "Accepted!" << endl;
		//cout << "Connected" << endl;
	}
}

void ClientUdp::disconnect()
{
	Packet packet;
	packet << MsgType::Disconnect;

	_socket.send(packet, _ip, _port);

	connected = false;
	//cout << "Disconnected" << endl;
}

void ClientUdp::process()
{
	if (!connected) { return; }

	IpAddress ip;
	unsigned short port;

	Packet send_packet, recv_packet;
	send_packet << MsgType::Update << _localPlayer->position() << _localPlayer->health();

	_socket.send(send_packet, _ip, _port);

	if (!check_sock(_socket.receive(recv_packet, ip, port))) { return; }

	int type;
	double buf[2];
	recv_packet >> type;

	string n_ip = "";
	Point2D n_pos;
	shared_ptr<Player> n_player;

	switch ((MsgType)type) {
	case MsgType::WorldUpdate:

		recv_packet >> n_ip >> n_pos >> buf[0] >> buf[1];

		n_player = make_shared<Player>(n_pos, buf[1], buf[0]);

		_players.emplace(n_ip, n_player);
		_world.addObject2D(n_player, "Player " + n_ip);

		//cout << "Player " + n_ip << endl;

		break;
	case MsgType::Update:
		recv_packet >> n_ip >> n_pos >> buf[0];
		//cout << "Update: " << n_ip << endl;
		_players[n_ip]->setPosition(n_pos);
		_players[n_ip]->setHealth(buf[0]);

		//cout << "Players: " << n_pos.to_str() << endl;
		//cout << _world.findObject2D("Player " + n_ip)->position().to_str() << endl;
		break;
	}
}

bool ClientUdp::isConnected() const
{
	return connected;
}

ClientUdp::~ClientUdp()
{
	for (auto&& p : _players) {
		p.second.reset();
	}
}
