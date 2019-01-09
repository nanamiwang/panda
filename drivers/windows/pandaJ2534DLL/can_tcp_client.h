#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#include <winsock2.h>
#include "log.h"

#pragma comment(lib, "ws2_32")

class can_tcp_client {
public:
	can_tcp_client() {

	}

	bool init() {
		WSADATA WSAData;
		SOCKADDR_IN addr;

		WSAStartup(MAKEWORD(2, 0), &WSAData);
		m_sock = socket(AF_INET, SOCK_STREAM, 0);

		addr.sin_addr.s_addr = inet_addr("192.168.1.100");
		addr.sin_family = AF_INET;
		addr.sin_port = htons(9999);

		if (connect(m_sock, (SOCKADDR *)&addr, sizeof(addr)) == SOCKET_ERROR) {
			logA("Connect to tcp server failed");
			return false;
		}
		logA("Connected to tcp server");
		return true;
	}

	void exit() {
		closesocket(m_sock);
	}

	int sendall(const char *buf, unsigned long len)
	{
		int total = 0; // how many bytes we've sent
		int bytesleft = len; // how many we have left to send
		int n = 0;
		while (total < len) {
			n = send(m_sock, buf + total, bytesleft, 0);
			if (n == -1) {
				/* print/log error details */
				break;
			}
			total += n;
			bytesleft -= n;
		}
		return n == -1 ? -1 : 0; // return -1 on failure, 0 on success
	}

	bool send_can_msg(const char *data, unsigned long len) {
		if (sendall(data, len) == -1) {
			logA("Send error");
			return false;
		}

	}

	bool recv_can_msg(unsigned char *out_buf) {
		char buf[1024];
		int len = sizeof(buf);
		int recved = 0;
		int desired = sizeof(unsigned short) * 2;
		while (recved < desired) {
			int ret = recv(m_sock, buf + recved, desired - recved, 0);
			if (ret <= 0) {
				logA("recv error %d", ret);
				return false;
			}
		}
		recved = 0;
		desired = 12;
		while (recved < desired) {
			int ret = recv(m_sock, (char *)(out_buf + recved), desired - recved, 0);
			if (ret <= 0) {
				logA("recv error %d", ret);
				return false;
			}
		}
		return true;
	}

private:
	SOCKET m_sock;
};