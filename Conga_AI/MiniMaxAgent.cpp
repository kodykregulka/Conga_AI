#include "MiniMaxAgent.h"

MiniMaxAgent::MiniMaxAgent(Color setColor, int max_depth, int (*evalFunction)(Board*, Color))
{
	color = setColor;
	opColor = (color == Color::black ? Color::white : Color::black);
	maxDepth = max_depth;
	evaluate = evalFunction;

	//timeElapsedTotal = (std::chrono::microseconds)0;
	//turnCount = 0;

}

Board* MiniMaxAgent::takeTurn(Board* board)
{
	auto start = std::chrono::high_resolution_clock::now();
	nodesExplored = 0;
	
	int maxTurnValue = INT_MIN;
	Board* maxTurnBoard = nullptr;

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

						nextBoard = board->genSuccessors(col, row, hor, vir);
						if (nextBoard != nullptr)
						{
							//successor found, explore it
							nodesExplored++;
							int currentTurnValue = minTurn(1, nextBoard);
							if (currentTurnValue > maxTurnValue)
							{
								maxTurnValue = currentTurnValue;
								if (maxTurnBoard != nullptr) delete maxTurnBoard;
								maxTurnBoard = nextBoard;
							}
							else
								delete nextBoard;
						}
					}
				}
			}
		}
	}
	auto stop = std::chrono::high_resolution_clock::now();
	timeElapsed = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	timeElapsedTotal += timeElapsed;
	turnCount++;
	nodesExploredTotal += nodesExplored;
	return maxTurnBoard;
}

int MiniMaxAgent::maxTurn(int depth, Board* board)
{
	int maxTurnValue = INT_MIN;

	if (board->isTrapped(color))
		return INT_MIN;

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

						nextBoard = board->genSuccessors(col, row, hor, vir);
						if (nextBoard != nullptr)
						{
							//successor found, explore it
							nodesExplored++;
							int currentTurnValue = minTurn(depth+1, nextBoard);
							if (currentTurnValue > maxTurnValue)
							{
								maxTurnValue = currentTurnValue;
							}
							delete nextBoard;
						}
					}
				}
			}
		}
	}
	//if trap condition, make preference for most current traps
	if (maxTurnValue >= INT_MAX - maxDepth)
		return maxTurnValue - 1;
	if (maxTurnValue <= INT_MIN + maxDepth)
		return maxTurnValue + 1;
	else
		return maxTurnValue;
}

int MiniMaxAgent::minTurn(int depth, Board* board)
{
	int minTurnValue = INT_MAX;

	if (board->isTrapped(opColor))
		return INT_MAX;

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

						nextBoard = board->genSuccessors(col, row, hor, vir);
						if (nextBoard != nullptr)
						{
							//successor found, explore it
							nodesExplored++;
							int currentTurnValue = maxTurn(depth+1, nextBoard);
							if (currentTurnValue < minTurnValue)
							{
								minTurnValue = currentTurnValue;
							}
							delete nextBoard;
						}
					}
				}
			}
		}
	}
	//if trap condition, make preference for most current traps
	if (minTurnValue >= INT_MAX - maxDepth)
		return minTurnValue - 1;
	if (minTurnValue <= INT_MIN + maxDepth)
		return minTurnValue + 1;
	else
		return minTurnValue;
}

int MiniMaxAgent::eval_zero(Board* board, Color color)
{
	return 0;
}

int MiniMaxAgent::eval_pileCount(Board* board, Color color)
{
	int score = 0;
	for (int row = 0; row < Board::MAX_LENGTH; row++)
	{
		for (int col = 0; col < Board::MAX_LENGTH; col++)
		{
			Color currentColor = board->getColor(col, row);
			if (currentColor == color)
				score++;
			else if (currentColor != Color::empty) //color is opponent
				score--;
		}
	}
	return score;
}

int MiniMaxAgent::eval_blobCount(Board* board, Color color)
{
	int maxCount = 0;
	int minCount = 0;
	bool** visit = new bool* [Board::MAX_LENGTH];
	for (int i = 0; i < Board::MAX_LENGTH; i++)
	{
		visit[i] = new bool[Board::MAX_LENGTH];
		for (int j = 0; j < Board::MAX_LENGTH; j++)
		{
			visit[i][j] = false;
		}
	}

	for (int row = 0; row < Board::MAX_LENGTH; row++)
	{
		for (int col = 0; col < Board::MAX_LENGTH; col++)
		{
			Color currentColor = board->getColor(col, row);
			if (currentColor == color)
			{
				maxCount++;
				visit[col][row] = true;
			}
			else if (currentColor != Color::empty)
			{
				minCount++;
				visit[col][row] = true;
			}
			else if(!visit[col][row])
			{
				//is empty and not explored yet, now blob explore and count
				//set up flag pointers
				bool isMax = false;
				bool isMin = false;

				//conduct search
				int blobCount = board->exploreBlob(color,col, row, visit, &isMax, &isMin);
				if (isMax)
					maxCount += blobCount;
				if (isMin)
					minCount += blobCount;
			}
		}
	}
	//clean up memory
	for (int i = 0; i < Board::MAX_LENGTH; i++)
	{
		delete[] visit[i];
	}
	delete[] visit;

	return maxCount - minCount;
}


