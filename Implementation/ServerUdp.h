#ifndef SERVERUDP_H
#define SERVERUDP_H

#include "Sockets.h"
#include "Camera.h"

using namespace Sockets;

class ServerUdp {
private:
	UdpSocket _socket;
	map<IpAddress, pair<IData, shared_ptr<Camera>>> _players;
	shared_ptr<Camera> _localPlayer;

	IpAddress _ip;
	IpAddress g_ip;
	unsigned short server_port;

	World& _world;

	bool binded;

	bool check_error(const Socket::Status& status, const IpAddress& ip);
	void disconnectPlayer(const IpAddress& ip);
	map<IpAddress, pair<IData, shared_ptr<Camera>>>::iterator disconnectPlayer(map<IpAddress, pair<IData, shared_ptr<Camera>>>::iterator it);

	IData _find(Camera* ptr);

public:
	explicit ServerUdp(World& world, shared_ptr<Camera>& player, const IpAddress& ip, const unsigned short& port);
	~ServerUdp();

	void start();
	void process();
	void stop();

	bool isBinded() const;
	IpAddress getGlobalIp() const;
};

#endif