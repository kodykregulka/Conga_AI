//MiniMaxAgent.cpp
#include "MiniMaxAgent.h"

MiniMaxAgent::MiniMaxAgent(Color setColor, int max_depth, int (*evalFunction)(Board*, Color))
{
	color = setColor;
	opColor = (color == Color::black ? Color::white : Color::black);
	maxDepth = max_depth;
	evaluate = evalFunction;
}

Board* MiniMaxAgent::takeTurn(Board* board)
{
	//set/overhead up for ai turn
	//acts like a maxTurn, but needs to return board state to main game loop
	//takeTurn is only run once per turn, whereas min and max are run many times
	
	auto start = std::chrono::high_resolution_clock::now();
	nodesExplored = 0;
	branchesPruned = 0;

	int alpha = INT_MIN;
	int beta =  INT_MAX;

	Board* alphaBoard = nullptr;

	if (board->isTrapped(Color::black) || board->isTrapped(Color::white))
		return nullptr;

	for (int row = 0; row < board->MAX_LENGTH; row++)
	{
		for (int col = 0; col < board->MAX_LENGTH; col++)
		{
			//check every board square for a possible move
			Color currentColor = board->getColor(col, row);
			if (color == currentColor)
			{
				//if agents color, then find a move in that square
				Board* nextBoard;

				for (int hor = -1; hor < 2; hor++)
				{
					for (int vir = -1; vir < 2; vir++)
					{
						if (hor == 0 && vir == 0)
							continue;

						nextBoard = board->genSuccessor(col, row, hor, vir);
						if (nextBoard != nullptr)
						{
							//successor found, explore it
							nodesExplored++;
							int currentTurnValue = minTurn(1, nextBoard, alpha, beta);
							if (currentTurnValue > alpha)
							{
								alpha = currentTurnValue;
								if (alphaBoard != nullptr) delete alphaBoard;
								alphaBoard = nextBoard;
								
								if (alpha >= beta)
								{
									branchesPruned++;
									goto FINISH_TAKE_TURN; //gross, but I need to break out of multiple loops
								}
							}
							else
								delete nextBoard;
						}
					}
				}
			}
		}
	}
	FINISH_TAKE_TURN: 
	auto stop = std::chrono::high_resolution_clock::now();
	timeElapsed = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	timeElapsedTotal += timeElapsed;
	turnCount++;
	nodesExploredTotal += nodesExplored;
	branchesPrunedTotal += branchesPruned;
	return alphaBoard;
}

int MiniMaxAgent::maxTurn(int depth, Board* board, int alpha, int beta)
{
	//ai turn, returns maximumn score
	//stops processing if score is greater than or equal to beta

	if (board->isTrapped(color))
		return INT_MIN + depth; //prefer recent traps

	if (depth >= maxDepth)
	{
		//process leaf
		return evaluate(board, color);
	}

	for (int row = 0; row < board->MAX_LENGTH; row++)
	{
		for (int col = 0; col < board->MAX_LENGTH; col++)
		{
			//check every board square for a possible move
			Color currentColor = board->getColor(col, row);
			if (color == currentColor)
			{
				//if agents color, then find a move in that square
				Board* nextBoard;

				for (int hor = -1; hor < 2; hor++)
				{
					for (int vir = -1; vir < 2; vir++)
					{
						if (hor == 0 && vir == 0)
							continue;

						nextBoard = board->genSuccessor(col, row, hor, vir);
						if (nextBoard != nullptr)
						{
							//successor found, explore it
							nodesExplored++;
							int currentTurnValue = minTurn(depth+1, nextBoard, alpha, beta);
							if (currentTurnValue > alpha)
							{
								alpha = currentTurnValue;
								if (alpha >= beta)
								{
									branchesPruned++;
									return alpha;
								}
							}
							delete nextBoard;
						}
					}
				}
			}
		}
	}
	return alpha;
}

int MiniMaxAgent::minTurn(int depth, Board* board, int alpha, int beta)
{
	//opponent turn, returns minimumn score
	//stops processing if score is less than or equal to alpha
	
	if (board->isTrapped(opColor))
		return INT_MAX - depth; //prefer recent traps

	if (depth >= maxDepth)
	{
		//process leaf
		return evaluate(board, color);
	}

	for (int row = 0; row < board->MAX_LENGTH; row++)
	{
		for (int col = 0; col < board->MAX_LENGTH; col++)
		{
			//check every board square for a possible move
			Color currentColor = board->getColor(col, row);
			if (opColor == currentColor)
			{
				//if agents color, then find a move in that square
				Board* nextBoard;

				for (int hor = -1; hor < 2; hor++)
				{
					for (int vir = -1; vir < 2; vir++)
					{
						if (hor == 0 && vir == 0)
							continue;

						nextBoard = board->genSuccessor(col, row, hor, vir);
						if (nextBoard != nullptr)
						{
							//successor found, explore it
							nodesExplored++;
							int currentTurnValue = maxTurn(depth+1, nextBoard, alpha, beta);
							if (currentTurnValue < beta)
							{
								beta = currentTurnValue;
								if (alpha >= beta)
								{
									branchesPruned++;
									return beta;
								}
							}
							delete nextBoard;
						}
					}
				}
			}
		}
	}
	return beta;	
}



