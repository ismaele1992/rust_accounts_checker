/*
 * Servers.h
 *
 *  Created on: 22 sept. 2020
 *      Author: isma
 */

#include <iostream>
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

class Servers {
	l_Servers L_Servers;
public:
	Servers();
	void storeServer(char * server_name, char * server_ip, int server_port);
	void storeServers(unsigned char * buffer, int buffer_size);
	void showServer(Server s);
	void showServers();
	l_Servers getServers();
	void setServerInfo();
	virtual ~Servers();
};

#endif /* SERVERS_H_ */
