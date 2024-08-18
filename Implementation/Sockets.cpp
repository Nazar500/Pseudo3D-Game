#include "Sockets.h"

namespace Sockets {
	bool check_sock(Socket::Status a, bool error, const string& str) {
		if (a != Socket::Done) {
			if (error) {
				cout << str;
				throw runtime_error("Socket Error");
			}
			else {
				return false;
			}
		}
		else {
			return true;
		}
	}
	string fstring(const string& str, vector<string> param) {
		string res = "";
		unsigned char count = 0, last_index = 0, param_size = (unsigned char)param.size();

		for (int i = 0; i < str.length() - 1; i++) {
			if (str[i] == '{' && str[i + 1] == '}') {
				res += str.substr(last_index, i - last_index) + ((param_size > count) ? param[count++] : "{}");
				last_index = i + 2;
				if (count == param_size) {
					res += str.substr(last_index, str.length() - last_index);
				}
			}
		}

		return res;
	}

	Packet& operator << (Packet& packet, const Point2D& point) {
		return packet << point.x << point.y;
	}

	Packet& operator >> (Packet& packet, Point2D& point) {
		return packet >> point.x >> point.y;
	}
}