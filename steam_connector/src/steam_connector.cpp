//============================================================================
// Name        : steam_connector.cpp
// Author      : isma_L
// Version     :
// Copyright   : Todos baneados putos
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Players.h"

char* RUSTYMOOSE = "139.99.144.50";
int RUSTYMOOSE_PORT = 28009;

int main() {
	Players p = Players();
	//p.sendPlayersRequest(RUSTYMOOSE, RUSTYMOOSE_PORT);
	p.storePlayerInfo(20, "aaaa", 20, 3.9);
	p.storePlayerInfo(30, "bbbb", 20, 2.9);
	p.showPlayers();
	return 0;
}
