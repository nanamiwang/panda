#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#include <winsock2.h>
#include "log.h"

#pragma comment(lib, "ws2_32")

#define PACKET_TYPE_CAN_FRAME 1U

typedef struct  {
	unsigned short length;
	unsigned short type;
}packet_header;

class can_tcp_client {
public:
	can_tcp_client() {

	}

	bool init() {
		WSADATA WSAData;
		SOCKADDR_IN addr;

		WSAStartup(MAKEWORD(2, 0), &WSAData);
		m_sock = socket(AF_INET, SOCK_STREAM, 0);

		addr.sin_addr.s_addr = inet_addr("192.168.1.101");
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
		logA("can tcp exit");
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
		logA("send_can_msg >>");
		packet_header hdr;
		hdr.length = htons(sizeof(hdr) + len);
		hdr.type = htons(PACKET_TYPE_CAN_FRAME);
		if (sendall((char *)&hdr, sizeof(hdr)) == -1) {
			logA("Send header error");
			return false;
		}
		if (sendall(data, len) == -1) {
			logA("Send payload error");
			return false;
		}
		logA("send_can_msg <<");
		return true;
	}

	bool recv_can_msg(char *out_buf) {
		char buf[1024];
		int len = sizeof(buf);
		int recved = 0;
		packet_header hdr;
		int desired = sizeof(hdr);
		while (recved < desired) {
			int ret = recv(m_sock,(char *)(&hdr) + recved, desired - recved, 0);
			if (ret <= 0) {
				logA("recv header error %d", ret);
				return false;
			}
			recved += ret;
		}
		recved = 0;
		logA("recved header, totoal len: %u, type: %u", ntohs(hdr.length), ntohs(hdr.type));
		desired = ntohs(hdr.length) - sizeof(hdr);
		while (recved < desired) {
			int ret = recv(m_sock, (out_buf + recved), desired - recved, 0);
			if (ret <= 0) {
				logA("recv packet body error %d", ret);
				return false;
			}
			recved += ret;
		}
		return true;
	}

private:
	SOCKET m_sock;
};