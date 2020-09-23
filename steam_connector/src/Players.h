/*
 * Players.h
 *
 *  Created on: 22 sept. 2020
 *      Author: isma
 */

#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
using namespace std;
#ifndef PLAYERS_H_
#define PLAYERS_H_

const int MAX_PLAYERS = 500;
const int SIZE_CHALLENGE = 9;
typedef struct player{
	int player_id;
	char * player_name[128];
	long player_score;
	float player_duration;
	player * next_player;
} Player;

class Players {
	const unsigned char CHALLENGE[SIZE_CHALLENGE] = {0xFF, 0xFF, 0xFF, 0xFF, 0x55, 0xFF, 0xFF, 0xFF, 0xFF};
	int number_players = 0;
	Player V_Players[MAX_PLAYERS];
	struct l_Players{
		Player * players;
		int num_players;
	} L_Players;
public:
	Players();
	int getNumberPlayers();
	void setNumberPlayers(int numberPlayers);
	bool sendPlayersRequest(const char* ip_address, int port);
	void storePlayersInfo(unsigned char * buffer, int message_length);
	void flushBuffer(unsigned char * buffer, int sizeof_buffer);
	void getVPlayers(Player * players);
	void Show_Players();
	virtual ~Players();
};

#endif /* PLAYERS_H_ */