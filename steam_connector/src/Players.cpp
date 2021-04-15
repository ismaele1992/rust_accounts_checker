/*
 * Players.cpp
 *
 *  Created on: 22 sept. 2020
 *      Author: isma
 */

#include "Players.h"

Players::Players() {
	// TODO Auto-generated constructor stub
	this->l_players.clear();
	this->number_players = 0;
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
			flushBuffer(response, sizeof_response);
			packet_size = recv(fd, response, sizeof_response, 0);
			cout << "Received response from the modified challenge..." << endl;
		}
		close(fd);
		storePlayersInfo(response, packet_size);
		return true;
	}
}

void Players::flushBuffer(unsigned char * buffer, int sizeof_buffer){
	unsigned char * b = buffer;
	for (int i = 0; i < sizeof_buffer; i++){
		*b = 0x00;
		b++;
	}
}

void Players::storePlayerInfo(int player_id, char * player_name, long player_score, float player_duration){

	Player p;
	p.player_id = player_id;
	p.player_name = new char[128];
	p.player_score = player_score;
	strcpy(p.player_name, player_name);
	p.player_duration = player_duration;
	this->l_players.push_back(p);
}

void Players::storePlayersInfo(unsigned char * buffer, int message_length){
	const char * ip_address;
	int port = 0;
	unsigned char * buffer;
	buffer = SteamAPI::QueryPlayers(ip_address, port, 0);
	int i = 6;
	this->number_players = int(buffer[5]);
	if (this->number_players > 0){
		while(i < message_length){
			int player_id = (int)buffer[i];
			i++;
			char player_name[128];
			char * p_player = player_name;
			while(buffer[i] != 0x00){
				*p_player = buffer[i];
				p_player++;
				i++;
			}
			*p_player = '\n';
			long score = 0;
			float duration = 0;
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
			storePlayerInfo(player_id, player_name, duration, score);
			i = i + 9;
		}
	}
}

void Players::showPlayer(Player p){
	cout << "ID of the player : " << p.player_id << endl;
	cout << "Name of the player : " << p.player_name << endl;
	cout << "Score of the player : " << p.player_score << endl;
	cout << "Log time on server of the player : " << p.player_duration << endl;
	cout << endl;
}

void Players::showPlayers(){
	for(auto p = this->l_players.begin(); p != l_players.end(); ++p){
		this->showPlayer(*p);
	}
}

Players::~Players() {
	// TODO Auto-generated destructor stub
	for(auto p = this->l_players.begin(); p != this->l_players.end(); ++p){
		delete[] p->player_name;
	}
	this->l_players.clear();
	this->number_players = 0;
}

