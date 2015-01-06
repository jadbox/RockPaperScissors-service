#pragma once
#include <fstream>
#include <string>
#include <memory>
#include <iostream>

// This class logs text to file server_log.txt
class Logger
{
public:
	Logger();
	~Logger();
	void log(std::string);
private:
	std::unique_ptr<std::fstream> fs;
};

