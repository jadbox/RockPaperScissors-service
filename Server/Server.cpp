#include "Server.h"
#include "shared\NetSys.h"
#include <string>
#include <iostream>

Server::Server()
{
	logger.log("Server Starting");
}

// Main server game loop on the network
void Server::startEventLoop() {
	//  Prepare our context and socket
	zmq::context_t * context = new zmq::context_t(1);
	zmq::socket_t socket(*context, ZMQ_REP);
	socket.bind("tcp://*:" PORT);

	while (true) {
		auto stream = NetSys::getStream(&socket);
		std::string token1;
		stream >> token1;
		std::string msg = "";
		
		// Client joined command
		if (token1 == TOKEN_JOIN) {
			logger.log("Client Joined");
			msg = TOKEN_OK;
		}
		// Client quit command
		else if (token1 == TOKEN_QUIT) {
			logger.log("Client Quiting");
			msg = TOKEN_OK;
		}
		// Client waiting for second player to give their move and requesting status.
		else if (token1 == TOKEN_STAT) {
			int token2;
			stream >> token2;
			auto room = rooms.findRoom(token2);
			// Check if the room has another player
			if (room->isFull() == false) {
				msg = "wait " + std::to_string(room->id);
			} else {
				// Room has another player now and a game outcome can be resolved.
				auto result = winCondition(room->firstMove, room->lastMove);
				if (result == Player_A) msg = TOKEN_WON;
				else if (result == Player_B) msg = TOKEN_LOST;
				else msg = TOKEN_DRAW;
				msg += " " + toStringCode(room->lastMove); // send other player's move

				// Log that all clients have resolved.
				if (result != DRAW) {
					gameWins++;
					logger.log("Two clients finished a game as one wins and one lost. Total win/loses: " + std::to_string(gameWins));
				}
				else {
					gameDraws++;
					logger.log("Two clients finished a game in a draw. Total draws: " + std::to_string(gameDraws) );
				}
			}
		}
		// Client making a play choice. Once done, the client is assigned a room.
		else if (token1 == TOKEN_PLAY) {
			auto room = rooms.findRoomToJoin();
			if (room == nullptr) exit(1);
			char token2;
			stream >> token2;
			
			auto move = toGameMove(token2);
			// If room is empty, put in my move and wait until another player joins and plays.
			// This client will check back with the "stat" command every few seconds.
			if (room->isEmpty()) {
				logger.log("Client joined empty room.");
				room->firstMove = move;
				msg = TOKEN_WAIT + " " + std::to_string(room->id);
			} else {
			// If room already has a player, look at their move and immediately determine if I won.
				room->lastMove = move;
				auto result = winCondition(move, room->firstMove);
				if (result == Player_A) msg = TOKEN_WON;
				else if (result == Player_B) msg = TOKEN_LOST;
				else msg = TOKEN_DRAW;
				msg += " " + toStringCode(room->firstMove); // send other player's move
			}
			room->addPlayer();
			logger.log("Recieved Play command: " + gameMoveToWord(move));
		} else {
			logger.log("Invalid client token: " + token1);
			msg = "invalid_client_token_error";
		}

		//logger.log("Sending: " + msg); // For debugging
		NetSys::sendString(&socket, msg); // Paired REQ response to read
		//NetSys::usleep(2000); //  Debugging limiter
	}
	socket.close();
}

Server::~Server()
{
	
}
