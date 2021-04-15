/*
 * Players.h
 *
 *  Created on: 22 sept. 2020
 *      Author: isma
 */

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <list>
#include "SteamAPI.h"
using namespace std;
#ifndef PLAYERS_H_
#define PLAYERS_H_

const int MAX_PLAYERS = 500;
const int SIZE_CHALLENGE = 9;
typedef struct player{
	int player_id;
	char * player_name;
	long player_score;
	float player_duration;
} Player;

class Players {
	const unsigned char CHALLENGE[SIZE_CHALLENGE] = {0xFF, 0xFF, 0xFF, 0xFF, 0x55, 0xFF, 0xFF, 0xFF, 0xFF};
	int number_players = 0;
	list<Player> l_players;
public:
	Players();
	int getNumberPlayers();
	void setNumberPlayers(int numberPlayers);
	bool sendPlayersRequest(const char* ip_address, int port);
	void storePlayerInfo(int player_id, char * player_name, long player_score, float player_duration);
	void storePlayersInfo(unsigned char * buffer, int message_length);
	void flushBuffer(unsigned char * buffer, int sizeof_buffer);
	void showPlayer(Player player);
	void showPlayers();
	virtual ~Players();
};

#endif /* PLAYERS_H_ */
