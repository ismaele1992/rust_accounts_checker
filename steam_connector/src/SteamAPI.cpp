/*
 * SteamAPI.cpp
 *
 *  Created on: Apr 15, 2021
 *      Author: isma
 */

#include "SteamAPI.h"

SteamAPI::SteamAPI() {
	// TODO Auto-generated constructor stub

}

unsigned char* SteamAPI::QueryPlayers(const char *ip_address, int port, int & packet_size) {
	sockaddr_in serveraddr;
	int sizeof_response = 8192;
	unsigned char response[sizeof_response];
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0){
		perror("Cannot open socket.");
		return NULL;
	}
	else{
		serveraddr.sin_family = AF_INET;
		serveraddr.sin_addr.s_addr = inet_addr(ip_address);
		serveraddr.sin_port = htons(port);
		cout << "Sending challenge..." << endl;
		if (sendto(fd, CHALLENGE, SIZE_CHALLENGE, 0, (sockaddr*)&serveraddr, sizeof(serveraddr)) < 0){
			perror("Cannot send UDP message");
			close(fd);
			return NULL;
		}
		recv(fd, response, sizeof_response, 0);
		cout << "Received response to challenge..." <<endl;
		unsigned char modified_challenge[SIZE_CHALLENGE];
		for (int i = 0; i < SIZE_CHALLENGE; i++){
			modified_challenge[i] = CHALLENGE[i];
		}
		while(response[4] != 0x44){
			for (int i = 0; i < SIZE_CHALLENGE; i++){
				if (i >= 5){
					modified_challenge[i] = response[i];
				}
			}
			cout << "Sending modified challenge..." << endl;
			if (sendto(fd, modified_challenge, sizeof(modified_challenge), 0, (sockaddr*)&serveraddr, sizeof(serveraddr)) < 0){
				perror("Cannot send UDP message");
				close(fd);
				return NULL;
			}
			flushBuffer(response, sizeof_response);
			packet_size = recv(fd, response, sizeof_response, 0);
			cout << "Received response from the modified challenge..." << endl;
		}
		close(fd);
		return response;
	}
}

void SteamAPI::flushBuffer(unsigned char * buffer, int sizeof_buffer){
	unsigned char * b = buffer;
	for (int i = 0; i < sizeof_buffer; i++){
		*b = 0x00;
		b++;
	}
}

SteamAPI::~SteamAPI() {
	// TODO Auto-generated destructor stub
}

