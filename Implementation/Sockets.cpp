#include "Sockets.h"

using namespace Settings;

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

	bool find(const map<IData, shared_ptr<Camera>>& _arr, const IpAddress& _ip)
	{
		for (auto&& p : _arr) {
			if (p.first.m_ip == _ip) {
				return true;
			}
		}
		return false;
	}

	Packet& operator << (Packet& packet, const Point2D& point) {
		return packet << point.x << point.y;
	}

	Packet& operator >> (Packet& packet, Point2D& point) {
		return packet >> point.x >> point.y;
	}

	Packet& operator << (Packet& packet, const IpAddress& obj) {
		return packet << obj.toString();
	}

	Packet& operator >> (Packet& packet, IpAddress& obj) {
		string ipString = obj.toString();
		packet >> ipString;
		obj = IpAddress(ipString);
		return packet;
	}

	Packet& operator << (Packet& packet, const IData& obj) {
		return packet << obj.m_ip << obj.m_port << obj.m_errors;
	}

	Packet& operator >> (Packet& packet, IData& obj) {
		return packet >> obj.m_ip >> obj.m_port >> obj.m_errors;
	}

}