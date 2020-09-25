/*
 * Servers.h
 *
 *  Created on: 22 sept. 2020
 *      Author: isma
 */

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
using namespace std;

#ifndef SERVERS_H_
#define SERVERS_H_

struct Server{
	char server_name [128];
	char server_ip[40];
	int server_port;
	Server * next_server;
};

struct l_Servers{
	Server * servers;
	int num_servers;
};
const int CHALLENGE_SIZE = 25;
const unsigned char CHALLENGE[CHALLENGE_SIZE] = {0xFF, 0xFF, 0xFF, 0xFF, 0x54, 0x53, 0x6F, 0x75, 0x72, 0x63, 0x65, 0x20, 0x45, 0x6E, 0x67, 0x69, 0x6E, 0x65, 0x20, 0x51, 0x75, 0x65, 0x72, 0x79, 0x00};

class Servers {
	l_Servers L_Servers;
	unsigned char MASTER_SERVER_QUERY[128] = {0x31, 0xFF, 0x30, 0x2E, 0x30, 0x2E, 0x30, 0x2E, 0x30, 0x3A, 0x30, 0x00};
public:
	Servers();
	void createServerQuery(char * query);
	void storeServer(char * server_name, char * server_ip, int server_port);
	void storeServers(unsigned char * buffer, int buffer_size);
	void showServer(Server s);
	void showServers();
	bool sendServerRequest(char * ipaddress, int port);
	l_Servers getServers();
	void setServerInfo();
	virtual ~Servers();
};

#endif /* SERVERS_H_ */
