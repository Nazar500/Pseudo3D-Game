#ifndef SOCKETS_H
#define SOCKETS_H

#include "Settings.h"
#include "Camera.h"

#include <SFML/Network.hpp>

#include <map>
#include <vector>
#include <utility>
#include <stdexcept>

using namespace std;
using namespace sf;

namespace Sockets {
	struct IData {
		IData(const IpAddress& ip = IpAddress::None, const unsigned short& port = Socket::AnyPort) {
			m_ip = ip;
			m_port = port;
			m_errors = 0;
			m_messages = 0;
			m_accepted = 1;
		}
		IpAddress m_ip;
		unsigned short m_port;
		bool m_accepted;
		short m_messages;
		unsigned char m_errors;
	};

	bool check_sock(Socket::Status a, bool error = 0, const string& str = "");
	string fstring(const string& str, vector<string> param);
	bool find(const map<IData, shared_ptr<Camera>>& _arr, const IpAddress& _ip);

	enum MsgType {
		Connect,
		Disconnect,
		Update,
		Shoot,
		Kills,
		WorldUpdate,
		Accept
	};

	Packet& operator << (Packet& packet, const Point2D& point);
	Packet& operator >> (Packet& packet, Point2D& point);

	Packet& operator << (Packet& packet, const IpAddress& obj);
	Packet& operator >> (Packet& packet, IpAddress& obj);

	Packet& operator << (Packet& packet, const IData& obj);
	Packet& operator >> (Packet& packet, IData& obj);
}

#endif
