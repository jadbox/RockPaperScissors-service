// RockPaperScissors Server Entry Point
// by Jonathan Dunlap
//
//  Binds REP socket to tcp://*:5555
//

#include "Server.h"

int main() {
	std::cout << "Rock, Scissors, Paper: Server Starting" << std::endl;
	Server server{};
	server.startEventLoop();

	return 0;
}