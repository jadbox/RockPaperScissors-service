#pragma once
#include <list>
#include "shared\GameState.h"
#include "Logger.h"
#define PORT "5555"

// Game room to track last move
struct Room {
	GameMove lastMove;
	GameMove firstMove;
	int id;
	int players;
	Room(int id) :players(0), id(id), lastMove(None), firstMove(None) {

	}
	bool isFull() {
		return players > 1;
	}
	bool isEmpty() {
		return players == 0;
	}
	void addPlayer() {
		players++;
	}
};

// Collection to manage Room objects
struct Rooms {
	std::list<Room> rooms;
	int numRooms;
	Rooms() : numRooms(0) {
	}
	// Looks for non-full room else creates one
	Room * findRoomToJoin() {
		Room * room = nullptr;
		for (auto&& current : rooms) {
			if (current.isFull() == false) {
				room = &current;
			}
		}
		if (room == nullptr) {
			rooms.push_back(Room(numRooms));
			numRooms++;
			room = &rooms.back();
		}
		return room;
	}

	// Finds a room by its ID else nullptr
	Room * findRoom(int id) {
		for (auto&& current : rooms) {
			if (current.id == id) {
				return &current;
			}
		}
		return nullptr;
	}
};

// Core server class handling network communication of game logic
class Server
{
public:
	Server();
	virtual ~Server();
	// Main server game loop on the network
	void startEventLoop();
private:
	Logger logger;
	Rooms rooms;
	int gameWins = 0;
	int gameDraws = 0;
};


