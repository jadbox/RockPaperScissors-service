#pragma once
#include <list>
#include <iostream>

// Network string tokens used
const std::string TOKEN_PLAY = "play";
const std::string TOKEN_WON = "won";
const std::string TOKEN_LOST = "lost";
const std::string TOKEN_DRAW = "draw";
const std::string TOKEN_WAIT = "wait";
const std::string TOKEN_JOIN = "join";
const std::string TOKEN_STAT = "stat";
const std::string TOKEN_QUIT = "quit";
const std::string TOKEN_OK = "ok";

// Enumerated types
enum Outcome { Player_A, Player_B, DRAW };
enum GameMove { Rock, Paper, Scissors, None };
static const std::string GameMoveStrings[] = { "Rock", "Paper", "Scissors", "None" };

// Check win condition
static const Outcome winCondition(GameMove a, GameMove b) {
	if (a == Rock && b == Scissors) return Player_A;
	else if (a == Rock && b == Paper) return Player_B;
	else if (a == Paper && b == Rock) return Player_A;
	else if (a == Paper && b == Scissors) return Player_B;
	else if (a == Scissors && b == Rock) return Player_B;
	else if (a == Scissors && b == Paper) return Player_A;
	return DRAW;
}

// Char to GameMove enum
static const GameMove toGameMove(char move) {
	if (move == 'r') return Rock;
	else if (move == 'p') return Paper;
	else if (move == 's') return Scissors;
	else return None;
}

// GameMove to string
static const std::string toStringCode(GameMove move) {
	if (move == Rock) return "r";
	else if (move == Paper) return "p";
	else if (move == Scissors) return "s";
	else return "none";
}

// GameMove enum to string value
static const std::string gameMoveToWord(GameMove move) {
	return GameMoveStrings[move];
}