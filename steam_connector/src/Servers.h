/*
 * Servers.h
 *
 *  Created on: 22 sept. 2020
 *      Author: isma
 */

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <list>
using namespace std;

#ifndef SERVERS_H_
#define SERVERS_H_

struct Server{
	char server_name [256];
	char server_ip[40];
	int server_port;
};

class Servers {
	list<Server> l_servers;
public:
	Servers();
	void storeServer(char * server_name, char * server_ip, int server_port);
	void storeServers(unsigned char * buffer, int buffer_size);
	void showServer(Server s);
	void showServers();
	list<Server> getServers();
	void setServerInfo();
	virtual ~Servers();
};

#endif /* SERVERS_H_ */
