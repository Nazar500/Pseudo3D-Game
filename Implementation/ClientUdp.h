#ifndef CLIENTUDP_H
#define CLIENTUDP_H

#include "Sockets.h"

using namespace Sockets;

class ClientUdp {
private:
	UdpSocket _socket;
	World& _world;
	shared_ptr<Camera> _localPlayer;
	map<IpAddress, shared_ptr<Camera>> _players;
	unsigned char errors;
	bool accepted;

	IpAddress _ip;
	unsigned short _port;

	bool connected;

	bool check_errors(const Socket::Status& status);
	
public:
	explicit ClientUdp(World& world, const shared_ptr<Camera>& player, const IpAddress& ip, const unsigned short& port);

	void connect();
	void disconnect();
	void process();

	bool isConnected() const;
	
	~ClientUdp();
};

#endif