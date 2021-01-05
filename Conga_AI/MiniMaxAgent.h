//MiniMaxAgent.h
#pragma once
#include "Agent.h"

class MiniMaxAgent :
	public Agent
{
	Color opColor;

	int maxTurn(int depth, Board* board, int alpha, int beta);
	int minTurn(int depth, Board* board, int alpha, int beta);

	int (*evaluate)(Board* board, Color color);

public:
	MiniMaxAgent(Color setColor, int max_depth, int (*evalFunction)(Board*, Color));
	Board* takeTurn(Board* board = nullptr);

};

