#pragma once 

#ifndef CLIENTUDP_H
#define CLIENTUDP_H

#include "Settings.h"
#include "World.h"

#include <SFML/Network.hpp>

#include <fstream>

using namespace sf;

class ClientUdp {
private:
	IpAddress _ip;
	unsigned short _port;

	UdpSocket _socket;
	World& _world;
	
public:
	explicit ClientUdp(World& world, const string& connectFile = CONNECT_FILE);

};

#endif