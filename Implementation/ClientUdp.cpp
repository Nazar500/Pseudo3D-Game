#include "ClientUdp.h"

ClientUdp::ClientUdp(World& world, const string& connectFile) : _world(world), _ip(IpAddress::None), _port(Socket::AnyPort)
{
	ifstream file(connectFile);

	string temp;
	if (file.is_open()) {
		getline(file, temp);
		_ip = temp;

		getline(file, temp);
		_port = (unsigned short)(stoi(temp));
	}

	file.close();
}
