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
	branchesPruned = 0;

	int alpha = INT_MIN;
	int beta =  INT_MAX;
	
	//int maxTurnValue = INT_MIN;
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

						nextBoard = board->genSuccessors(col, row, hor, vir);
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
							int currentTurnValue = minTurn(depth+1, nextBoard, alpha, beta);
							if (currentTurnValue > alpha)
							{
								alpha = currentTurnValue;
								if (alpha >= beta)
								{
									branchesPruned++;
									goto FINISH_MAX_TURN; //gross, but I need to break out of multiple loops
								}
							}
							delete nextBoard;
						}
					}
				}
			}
		}
	}
	//if trap condition, make preference for most current traps
	FINISH_MAX_TURN: 
	if (alpha >= INT_MAX - maxDepth)
		return alpha - 1;
	if (alpha <= INT_MIN + maxDepth)
		return alpha + 1;
	else
		return alpha;
}

int MiniMaxAgent::minTurn(int depth, Board* board, int alpha, int beta)
{
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
							int currentTurnValue = maxTurn(depth+1, nextBoard, alpha, beta);
							if (currentTurnValue < beta)
							{
								beta = currentTurnValue;
								if (alpha >= beta)
								{
									branchesPruned++;
									goto FINISH_MIN_TURN;
								}
							}
							delete nextBoard;
						}
					}
				}
			}
		}
	}
	//if trap condition, make preference for most current traps
	FINISH_MIN_TURN: 
	if (beta >= INT_MAX - maxDepth)
		return beta - 1;
	if (beta <= INT_MIN + maxDepth)
		return beta + 1;
	else
		return beta;
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


