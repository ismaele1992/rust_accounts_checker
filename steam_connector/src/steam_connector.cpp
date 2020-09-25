//============================================================================
// Name        : steam_connector.cpp
// Author      : isma_L
// Version     :
// Copyright   : Todos baneados putos
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Players.h"
#include "Servers.h"

char* RUSTYMOOSE = "139.99.144.50";
int RUSTYMOOSE_PORT = 28009;

int main() {
	/*Players p = Players();
	p.sendPlayersRequest(RUSTYMOOSE, RUSTYMOOSE_PORT);
	p.showPlayers();*/
	Servers s = Servers();
	s.sendServerRequest("208.64.200.39", 27011);
	s.showServers();
	return 0;
}
