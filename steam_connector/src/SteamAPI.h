/*
 * SteamAPI.h
 *
 *  Created on: Apr 15, 2021
 *      Author: isma
 */
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;
#ifndef STEAMAPI_H_
#define STEAMAPI_H_

const int SIZE_CHALLENGE = 9;

class SteamAPI {
	const unsigned char CHALLENGE[SIZE_CHALLENGE] = {0xFF, 0xFF, 0xFF, 0xFF, 0x55, 0xFF, 0xFF, 0xFF, 0xFF};
public:
	SteamAPI();
	unsigned char* QueryPlayers(const char * ip_address, int port, int & packet_size);
	void flushBuffer(unsigned char * buffer, int sizeof_buffer);
	virtual ~SteamAPI();
};

#endif /* STEAMAPI_H_ */
