#ifndef SERVERUDP_H
#define SERVERUDP_H

#include "Sockets.h"
#include "World.h"
#include "Player.h"

#include <fstream>

using namespace Sockets;

class ServerUdp {
private:
	UdpSocket _socket;
	map<IpAddress, shared_ptr<Player>> _players;
	shared_ptr<Player> _localPlayer;

	IpAddress _ip;
	IpAddress g_ip;
	unsigned short _port;

	World& _world;

	bool binded;

public:
	explicit ServerUdp(World& world, shared_ptr<Player> player, const IpAddress& ip, const unsigned short& port);
	~ServerUdp();

	void start();
	void process();
	void stop();

	bool isBinded() const;
	IpAddress getGlobalIp() const;
};

#endif