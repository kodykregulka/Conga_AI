#pragma once
#include "Agent.h"

class MiniMaxAgent :
	public Agent
{
	Color opColor;

	int maxTurn(int depth, Board* board);
	int minTurn(int depth, Board* board);

	int (*evaluate)(Board* board, Color color);

public:
	MiniMaxAgent(Color setColor, int max_depth, int (*evalFunction)(Board*, Color));
	Board* takeTurn(Board* board = nullptr);

	static int eval_zero(Board* board, Color color);
	static int eval_pileCount(Board* board, Color color);
	static int eval_blobCount(Board* board, Color color);

};

