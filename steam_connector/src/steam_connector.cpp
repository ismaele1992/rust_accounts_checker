//============================================================================
// Name        : steam_connector.cpp
// Author      : isma_L
// Version     :
// Copyright   : Todos baneados putos
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
using namespace std;

char* RUSTYMOOSE = "139.99.144.50";
int RUSTYMOOSE_PORT = 28009;
const int MESSAGE_SIZE = 9;
unsigned char challenge[MESSAGE_SIZE] = {0xFF, 0xFF, 0xFF, 0xFF, 0x55, 0xFF, 0xFF, 0xFF, 0xFF};

//Example server: IP 151.80.10.204 PORT 28166

void show_player_name(unsigned char * buffer, int message_length){
	int index_player = 0;
	char player_name[128];
	char *p_player = player_name;
	long score = 0;
	float duration = 0;
	int i = 6;
	cout << "Number of players : " << int(buffer[5]) <<endl;
	while(i < message_length){
		index_player = int(buffer[i]);
		i++;
		while(buffer[i] != 0x00){
			*p_player = buffer[i];
			p_player++;
			i++;
		}
		*p_player = '\n';
		p_player = player_name;
		int shifting = -1;
		for (int j = 1; j < 9; j++){
			if (shifting < 0){
				shifting = 24;
			}
			if (j < 4){
				score += buffer[i + j] << shifting;
				shifting -= 8;
			}
			else{
				duration += buffer[i + j] << shifting;
				shifting -= 8;
			}
		}
		cout << "Index of the player : " << index_player << endl;
		cout << "Player name : " ;
		while (*p_player != '\n'){
			cout << *p_player;
			p_player++;
		}
		cout << endl;
		cout << "Score of the player : " << score << endl;
		cout << "Duration of the player in the server : " << duration << endl;
		p_player = player_name;
		i = i + 9;
	}
}

void flush_buffer(unsigned char *response, int size_response){
	for (int i = 0; i < size_response; i++)
		response[i] = 0x00;
}

void show_buffer(unsigned char *buffer, int size_response){
	cout << buffer[7] << endl;
	for (int i = 0; i < size_response; i++){
		cout<< buffer[i];
	}
	cout << endl;
	for (int i = 0; i < size_response; i++){
		cout<<char(int(buffer[i]));
	}
}

bool get_challenge(const char* ip_address, int port, unsigned char* response, int sizeof_response){
	sockaddr_in serveraddr;
	int packet_size = 0;
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0){
		perror("Cannot open socket.");
		return false;
	}
	else{
		serveraddr.sin_family = AF_INET;
		serveraddr.sin_addr.s_addr = inet_addr(ip_address);
		serveraddr.sin_port = htons(port);
		cout << "Sending challenge..." << endl;
		if (sendto(fd, challenge, sizeof(challenge), 0, (sockaddr*)&serveraddr, sizeof(serveraddr)) < 0){
			perror("Cannot send UDP message");
			close(fd);
			return false;
		}
		recv(fd, response, sizeof_response, 0);
		cout << "Received response to challenge..." <<endl;
		//show_buffer(response, sizeof_response);
		unsigned char modified_challenge[MESSAGE_SIZE];
		for (int i = 0; i < MESSAGE_SIZE; i++){
			modified_challenge[i] = challenge[i];
		}
		while(response[4] != 0x44){
			for (int i = 0; i < MESSAGE_SIZE; i++){
				if (i >= 5){
					modified_challenge[i] = response[i];
				}
			}
			cout << "Sending modified challenge..." << endl;
			if (sendto(fd, modified_challenge, sizeof(modified_challenge), 0, (sockaddr*)&serveraddr, sizeof(serveraddr)) < 0){
				perror("Cannot send UDP message");
				close(fd);
				return false;
			}
			//show_buffer(modified_challenge, sizeof(modified_challenge));
			flush_buffer(response, sizeof_response);
			packet_size = recv(fd, response, sizeof_response, 0);
			cout << "Received response from the modified challenge..." << endl;
			//show_buffer(response, packet_size);
		}
		close(fd);
		show_player_name(response, packet_size);
		return true;
	}
}

void get_steam_info(){
	int response_size = 8192;
	unsigned char response[response_size];
	//show_buffer(challenge, MESSAGE_SIZE);
	flush_buffer(response, response_size);
	//show_buffer(response, response_size);
	get_challenge(RUSTYMOOSE, RUSTYMOOSE_PORT, response, response_size);
}

int main() {
	get_steam_info();
	return 0;
}
