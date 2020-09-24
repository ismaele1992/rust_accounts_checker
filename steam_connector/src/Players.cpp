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

Player* Players::storePlayerInfo(int player_id, char * player_name, long player_score, float player_duration){

	Player * p = new Player;
	p -> player_id = player_id;
	int i = 0;
	while(player_name[i] != '\n'){
		p -> player_name[i] = player_name[i];
		i++;
	}
	p -> player_duration = player_duration;
	p -> next_player = NULL;
	if (L_Players.players == NULL){
		L_Players.players = p;
	}
	else{
		Player * p_aux = L_Players.players;
		while(p_aux->next_player != NULL){
			p_aux = p_aux->next_player;
		}
		p_aux->next_player = p;
	}
	return L_Players.players;
}

void Players::storePlayersInfo(unsigned char * buffer, int message_length){
	int i = 6;
	L_Players.num_players = int(buffer[5]);
	if (L_Players.num_players > 0){
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

void Players::getVPlayers(Player * players){
	players = this->V_Players;
}

void Players::showPlayer(Player p){
	cout << "ID of the player : " << p.player_id << endl;
	cout << "Name of the player : " << p.player_name << endl;
	cout << "Score of the player : " << p.player_score << endl;
	cout << "Log time on server of the player : " << p.player_duration << endl;
	cout << endl;
}

void Players::showPlayers(){
	Player * p = L_Players.players;
	cout << "First iteration ..." << endl;
	for(int i = 0; i < this->L_Players.num_players; i++){
		showPlayer(*p);
		p = p->next_player;
	}
	p = L_Players.players;
	cout << "Second iteration ..." << endl;
	while(p != NULL){
		showPlayer(*p);
		p = p->next_player;
	}
}

Players::~Players() {
	// TODO Auto-generated destructor stub
	Player * aux = L_Players.players;
	while(aux != 0){
		Player * next = aux -> next_player;
		delete aux;
		aux = next;
	}
	L_Players.players = NULL;
	L_Players.num_players = 0;
}

