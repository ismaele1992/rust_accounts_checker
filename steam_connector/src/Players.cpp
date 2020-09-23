/*
 * Players.cpp
 *
 *  Created on: 22 sept. 2020
 *      Author: isma
 */

#include "Players.h"

Players::Players() {
	// TODO Auto-generated constructor stub
	L_Players.players = NULL;
	L_Players.num_players = 0;
}

int Players::getNumberPlayers(){
	return number_players;
}

void Players::setNumberPlayers(int numberPlayers = 0) {
	number_players = numberPlayers;
}

bool Players::sendPlayersRequest(const char* ip_address, int port){
	sockaddr_in serveraddr;
	int sizeof_response = 8192;
	unsigned char response[sizeof_response];
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
		if (sendto(fd, CHALLENGE, SIZE_CHALLENGE, 0, (sockaddr*)&serveraddr, sizeof(serveraddr)) < 0){
			perror("Cannot send UDP message");
			close(fd);
			return false;
		}
		recv(fd, response, sizeof_response, 0);
		cout << "Received response to challenge..." <<endl;
		//show_buffer(response, sizeof_response);
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
				return false;
			}
			//show_buffer(modified_challenge, sizeof(modified_challenge));
			flushBuffer(response, sizeof_response);
			packet_size = recv(fd, response, sizeof_response, 0);
			cout << "Received response from the modified challenge..." << endl;
			//show_buffer(response, packet_size);
		}
		close(fd);
		storePlayersInfo(response, packet_size);
		return true;
	}
}

void Players::flushBuffer(unsigned char * buffer, int sizeof_buffer){
	for (int i = 0; i < sizeof_buffer; i++)
		buffer[i] = 0x00;
}

void Players::storePlayersInfo(unsigned char * buffer, int message_length){
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
	L_Players.players = new Player;
	L_Players.players->next_player = NULL;
	L_Players.num_players++;
}

void Players::getVPlayers(Player * players){
	players = this->V_Players;
}

void Players::Show_Players(){

}

Players::~Players() {
	// TODO Auto-generated destructor stub
}

