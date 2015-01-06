#include "Logger.h"

// Opens a connection to server_log.txt
Logger::Logger()
{
	fs = std::unique_ptr<std::fstream>(new std::fstream(""));
	fs->open("server_log.txt", std::fstream::out | std::fstream::app);
}

// Appends to server_log.txt
void Logger::log(std::string val) {
	std::cout << "Log: " << val << std::endl; // For debugging
	*fs << val << std::endl;
}

Logger::~Logger()
{
	fs->close();
}
