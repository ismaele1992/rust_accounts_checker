/*
 * Servers.cpp
 *
 *  Created on: 22 sept. 2020
 *      Author: isma
 */

#include "Servers.h"

Servers::Servers() {
	// TODO Auto-generated constructor stub
	this->l_servers.clear();
}

void Servers::storeServer(char * server_name, char * server_ip, int server_port) {
	Server s;
	strcpy(s.server_name, server_name);
	strcpy(s.server_ip, server_ip);
	s.server_port = server_port;
	this->l_servers.push_back(s);
}

void Servers::storeServers(unsigned char *buffer, int buffer_size) {
}

void Servers::showServer(Server s) {
	cout << "Name of the server : " << s.server_name;
	cout << "IP of the server : " << s.server_ip;
	cout << "Port of the server : " << s.server_port;
	cout << endl;
}

void Servers::showServers() {
	for(auto s = this->l_servers.begin(); s!=this->l_servers.end(); ++s){
		this->showServer(*s);
	}
}

list<Server> Servers::getServers() {
	return this->l_servers;
}

void Servers::setServerInfo() {
}

Servers::~Servers() {
	// TODO Auto-generated destructor stub
	this->l_servers.clear();
}

