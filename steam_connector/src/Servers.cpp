/*
 * Servers.cpp
 *
 *  Created on: 22 sept. 2020
 *      Author: isma
 */

#include "Servers.h"

Servers::Servers() {
	// TODO Auto-generated constructor stub
	L_Servers.num_servers = 0;
	L_Servers.servers = NULL;
}

void Servers::storeServer(char * server_name, char * server_ip, int server_port) {
	Server * s = new Server;
	strcpy(s->server_name, server_name);
	strcpy(s->server_ip, server_ip);
	s->server_port = server_port;
	s->next_server = NULL;
	if (L_Servers.servers == NULL){
		L_Servers.servers = s;
	}
	else{
		Server * aux = L_Servers.servers;
		while(aux->next_server != NULL){
			aux = aux->next_server;
		}
		aux -> next_server = s;
	}
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
	Server * s = L_Servers.servers;
	while(s->next_server != NULL){
		showServer(*s);
		s = s->next_server;
	}
}

l_Servers Servers::getServers() {
	return L_Servers;
}

void Servers::setServerInfo() {
}

Servers::~Servers() {
	// TODO Auto-generated destructor stub
	Server * aux = L_Servers.servers;
	while(aux != 0){
		Server * aux_next = aux->next_server;
		delete aux;
		aux = aux_next;
	}
	L_Servers.servers = NULL;
	L_Servers.num_servers = 0;
}

