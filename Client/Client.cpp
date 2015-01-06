#include "Client.h"

Client::Client()
{
}

void Client::start() {
	std::cout << "Welcome to Rock, Paper, Scissors!" << std::endl;

}

void Client::startEventLoop() {
	ucontext = std::unique_ptr<zmq::context_t>(new zmq::context_t(1));
	usocket = std::unique_ptr<zmq::socket_t>(new zmq::socket_t(*ucontext.get(), ZMQ_REQ));
	auto socket = usocket.get();
	//socket = new zmq::socket_t(*context, ZMQ_REQ);

	std::cout << "Connecting to server..." << std::endl;
	socket->connect("tcp://localhost:" PORT);
	// Join the server
	NetSys::sendString(socket, TOKEN_JOIN);
	auto connectStr = NetSys::getString(socket);
	std::cout << "Recieved " << connectStr << " status from server." << std::endl;
	// Pick your move
	char move = pickMove();
	std::string cmd = TOKEN_PLAY + " " + std::string(1, move);

	NetSys::sendString(socket, cmd);
	// Wait on opponent's move

	std::cout << "Waiting for second player..." << std::endl;
	std::string moveRtn;
	do {
		auto stream = NetSys::getStream(socket);
		stream >> moveRtn;
		if (moveRtn == TOKEN_WON || moveRtn == TOKEN_LOST || moveRtn == TOKEN_DRAW) {
			char token2;
			stream >> token2;
			std::cout << "Opponent chose: " << gameMoveToWord(toGameMove(token2)) << std::endl;
			if (moveRtn == TOKEN_DRAW) std::cout << std::endl << "Game ended in a draw." << std::endl;
			else std::cout << std::endl << "You " << moveRtn << " this game match." << std::endl;
		}
		else if (moveRtn == TOKEN_WAIT) {
			int room;
			stream >> room;
			std::cout << ".";
			NetSys::usleep(1000);
			std::string stat = TOKEN_STAT + " " + std::to_string(room);
			NetSys::sendString(socket, stat);
		}
		else {
			std::cout << "Recieved odd server token: " << moveRtn << std::endl;
			NetSys::usleep(5000);
			std::exit(1);
		}
	} while (moveRtn == TOKEN_WAIT);

	NetSys::sendString(socket, TOKEN_QUIT);
	NetSys::getString(socket); // ignore quit response
	socket->close();
	std::cout << "Press enter to exit." << std::endl;
	std::cin.sync();
	std::getchar();
}

// Prompts user to pick a character for their move. Press the letter and hit enter.
char Client::pickMove() {
	char retVal = 0;
	std::cout << "It's your turn. Pick:" << std::endl;
	std::cout << "r) rock" << std::endl;
	std::cout << "p) paper" << std::endl;
	std::cout << "s) scissors" << std::endl;
	{
		char ic;
		std::cin >> ic;
		if (ic != 'r' && ic != 'p' && ic != 's') {
			std::cout << "Invalid selection. Rerun." << std::endl;
		}
		else retVal = ic;
		std::cin.ignore(INT_MAX, '\n');
		std::cin.clear();
	} while (retVal == 0); // input validation

	std::cout << "Okay, you picked: " << gameMoveToWord(toGameMove(retVal)) << std::endl;
	return retVal;
}

// REMOVE?
void Client::doAction(GameMove m) {
	std::cout << gameMoveToWord(m);
}


Client::~Client()
{
	if (usocket->connected()) {
		NetSys::sendString(usocket.get(), TOKEN_QUIT);
		NetSys::getString(usocket.get()); // ignore quit response
		usocket->close();
	}
	//std::cout << "client exiting";
}