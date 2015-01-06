#pragma once
#define PORT "5555"

#include "GameState.h"
#include "NetSys.h"
#include <stdio.h>
#include <iostream>
#include <limits>
#include <memory>

// Handles game prompts and network communication.
class Client
{
public:
	Client();
	virtual ~Client();
	void doAction(GameMove);
	void start();
	char pickMove();
	void startEventLoop();
private:
	std::unique_ptr<zmq::socket_t> usocket;
	std::unique_ptr<zmq::context_t> ucontext;
};

