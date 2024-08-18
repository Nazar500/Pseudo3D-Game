#ifndef SOCKETS_H
#define SOCKETS_H

#include "Settings.h"
#include "Point2D.h"
#include "Player.h"

#include <SFML/Network.hpp>

#include <vector>
#include <map>
#include <stdexcept>

using namespace std;
using namespace sf;

namespace Sockets {
	bool check_sock(Socket::Status a, bool error = 0, const string& str = "");
	string fstring(const string& str, vector<string> param);

	/*struct Data {
		Data(const MsgType& type, const Point2D& position, double direction) {
			_type = type;
			_pos = position;
			_direction = direction;
		}
		
		MsgType _type;
		Point2D _pos;
		double _direction;
	};*/

	enum MsgType {
		Connect,
		Disconnect,
		Update,
		Shoot,
		WorldUpdate,
		Accept
	};

	Packet& operator << (Packet& packet, const Point2D& point);
	Packet& operator >> (Packet& packet, Point2D& point);
}

#endif
