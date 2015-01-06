#pragma once

#include "zmq.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include <exception>

#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#endif

// Contains utilities for ZeroMQ messages
namespace NetSys {
	static void usleep(int val) {
		#ifndef _WIN32
		sleep(val);
		#else
		Sleep(val);
		#endif
	}

	static const std::string msgToString(zmq::message_t* msg) {
		std::string response = std::string(
			static_cast<const char*>(msg->data()),
			msg->size());
		return response;
	}

	static std::istringstream msgToStream(zmq::message_t* msg) {
		return std::istringstream(std::string( static_cast<char*>(msg->data()), msg->size() ));
	}

	static const std::string getString(zmq::socket_t* socket) {
		zmq::message_t reply;
		try
		{
			socket->recv(&reply);
		}
		catch (std::exception& e)
		{
			std::cout << "Error: " << e.what() << std::endl;
			usleep(2000);
			exit(1);
		}
		return msgToString(&reply);
	}

	static std::istringstream getStream(zmq::socket_t* socket) {
		zmq::message_t reply;
		socket->recv(&reply);
		return msgToStream(&reply);
	}

	static zmq::message_t stringToMsg(const std::string& txt) {
		zmq::message_t reply(txt.length());
		memcpy((void *)reply.data(), txt.c_str(), txt.length());
		return reply;
	}

	static void sendString(zmq::socket_t* sock, const std::string& txt) {
		sock->send(stringToMsg(txt));
	}

}