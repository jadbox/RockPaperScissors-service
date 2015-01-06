// Client.cpp : Defines the entry point for the console application.

//  Rock, Paper, Scissors client in C++
//  by Jonathan Dunlap
//  Connects REQ socket to tcp://localhost:5555
//

#include <string>
#include <iostream>
#include "Client.h"
#include <windows.h> 

BOOL CtrlHandler(DWORD fdwCtrlType){
	printf("Caught signal %d.\nThis WILL cause the current game to enter a failed state.\n", fdwCtrlType);
	exit(1);
}

int main()
{
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE);
	//  Prepare our context and socket
	std::cout << "Starting Client" << std::endl;
	Client client{};
	client.startEventLoop();
	std::cout << "Client ended" << std::endl;
	return 0;
}