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
	createServerQuery("\\gamedir\\rust\\empty\\1");
}

void Servers::createServerQuery(char * query){
	int j = 0;
	while(MASTER_SERVER_QUERY[j] != '\0')
		j++;
	j++;
	for (int i = 0; i < (int)strlen(query); i++){
		MASTER_SERVER_QUERY[j] = (unsigned char)query[i];
		j++;
	}
	cout << j << endl;
}

void Servers::storeServer(char * server_name, char * server_ip, int server_port) {
	Server s;
	strcpy(s.server_name, server_name);
	strcpy(s.server_ip, server_ip);
	s.server_port = server_port;
	this->l_servers.push_back(s);
}

void Servers::storeServers(unsigned char *buffer, int buffer_size) {
	int iter = 6;
	int number_servers = 0;
	while(iter < buffer_size){
		char ip_server[16]= "";
		strcat(ip_server, to_string(int(buffer[iter])).c_str());
		strcat(ip_server, ".");
		strcat(ip_server, to_string(int(buffer[iter+1])).c_str());
		strcat(ip_server, ".");
		strcat(ip_server, to_string(int(buffer[iter+2])).c_str());
		strcat(ip_server, ".");
		strcat(ip_server, to_string(int(buffer[iter+3])).c_str());
		int port = int(0x00 << 24 | 0x00 << 16 | (unsigned int)(buffer[iter+4]) << 8 | (unsigned int)(buffer[iter+5]));
		cout << ip_server << ":" << port << endl;
		storeServer("", ip_server, port);
		iter= iter + 6;
		number_servers++;
	}
	cout << "Number of found servers : " << number_servers << endl;
}

void Servers::showServer(Server s) {
	cout << "Name of the server : " << s.server_name << endl;
	cout << "IP of the server : " << s.server_ip << endl;
	cout << "Port of the server : " << s.server_port << endl;
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

bool Servers::sendServerRequest(char * ipaddress, int port){
	sockaddr_in serveraddr;
	int sizeof_response = 8192;
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	unsigned char response[sizeof_response];
	if (fd < 0){
		perror("Cannot open socket to connect to Master Server.");
		return false;
	}
	else{
		serveraddr.sin_family = AF_INET;
		serveraddr.sin_addr.s_addr = inet_addr(ipaddress);
		serveraddr.sin_port = htons(port);
		if (sendto(fd, MASTER_SERVER_QUERY, 33, 0, (sockaddr*)&serveraddr, sizeof(serveraddr)) < 0){
			perror("Cannot send packet to Master Server socket");
			close(fd);
			return false;
		}
		int a = recv(fd, response, sizeof_response, 0);
		storeServers(response, a);
		return false;
	}
}

void Servers::setServerInfo() {
}

Servers::~Servers() {
	// TODO Auto-generated destructor stub
	this->l_servers.clear();
}

