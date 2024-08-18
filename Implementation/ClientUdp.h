#ifndef CLIENTUDP_H
#define CLIENTUDP_H

#include "Sockets.h"
#include "World.h"
#include "Player.h"

#include <fstream>

using namespace Sockets;

class ClientUdp {
private:
	UdpSocket _socket;
	World& _world;
	shared_ptr<Player> _localPlayer;
	map<IpAddress, shared_ptr<Player>> _players;

	IpAddress _ip;
	unsigned short _port;

	bool connected;
	
public:
	explicit ClientUdp(World& world, shared_ptr<Player>& player, const IpAddress& ip, const unsigned short& port);

	void connect();
	void disconnect();
	void process();

	bool isConnected() const;
	
	~ClientUdp();
};

#endif