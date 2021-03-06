//Board.cpp
#include "Board.h"

Board::Board()
{
	for (int row = 0; row < MAX_LENGTH; row++)
	{
		for (int col = 0; col < MAX_LENGTH; col++)
		{
			data[col][row] = 0;
		}
	}
	data[0][0] = 10;
	data[MAX_LENGTH-1][MAX_LENGTH - 1] = -10;
}

Board::Board(const Board& b)
{
	for (int row = 0; row < MAX_LENGTH; row++)
	{
		for (int col = 0; col < MAX_LENGTH; col++)
		{
			data[col][row] = b.data[col][row];
		}
	}
}

Board::Board(short d[MAX_LENGTH][MAX_LENGTH])
{
	for (int row = 0; row < MAX_LENGTH; row++)
	{
		for (int col = 0; col < MAX_LENGTH; col++)
		{
			data[col][row] = d[col][row];
		}
	}
}

/* c x r
	(0,0) (1,0) (2,0) (3,0)
	(0,1) (1,1) (2,1) (3,1)
	(0,1) (1,1) (2,1) (3,1)
	(0,1) (1,1) (2,1) (3,1)
*/

void Board::print()
{
	for (int row = 0; row < MAX_LENGTH; row++)
	{
		for (int col = 0; col < MAX_LENGTH; col++)
		{
			if (data[col][row] < 0)
			{ //neg = white
				std::cout << " -" << std::hex << -data[col][row] << " ";
			}
			else
			{ //pos = black
				std::cout << "  " << std::hex << data[col][row] << " ";
			}
		}
		std::cout << std::endl;
	}
}

Color Board::getColor(short col, short row)
{
	if (data[col][row] > 0)
		return Color::black;
	else if (data[col][row] < 0)
		return Color::white;
	else
		return Color::empty;
}

bool Board::isValidPlacement(Color color, short col, short row)
{
	//is given a board square, determines if that square is playable to the given color
	if (col < MAX_LENGTH && col >= 0 && row < MAX_LENGTH && row >= 0)
	{
		Color squareColor = getColor(col, row);
		if (squareColor == color || squareColor == Color::empty)
			return true;
		else
			return false;
	}
	else
		return false;
}

Board* Board::genSuccessor(short col, short row, short hor, short vir)
{
	//assumes square originally pointed to is valid
	//if move not possible, return nullptr

	Color color = getColor(col, row);

	//checks if at least one space is avalible, NEEDED
	if (!isValidPlacement(color, col+hor, row+vir))
		return nullptr; //not a valid move

	//Now we know the move is valid, create board with that move (We do not want to mutate original board)
	Board *nextBoard = new Board(*this);
	int pileSize = nextBoard->data[col][row]; //how many stones are avalible
	nextBoard->data[col][row] = 0; //every move always removes all stones from origin square
	int modifier = (color == Color::black) ? 1 : -1; //black is positive, white negitive

	int nextCol = col;
	int nextRow = row;

	for (int i = 1; i < MAX_LENGTH; i++)
	{
		//update next coordinates
		nextCol += hor;
		nextRow += vir;

		//check validity
		if (!nextBoard->isValidPlacement(color, nextCol, nextRow))
		{
			//dump in last valid spot
			nextCol -= hor;
			nextRow -= vir;
			nextBoard->data[nextCol][nextRow] += pileSize;
			pileSize = 0;
			break;
		}
		
		//make placement
		int transferWanted = i * modifier;
		if (pileSize*modifier <= transferWanted*modifier) //compare absolute values
		{
			//not enough in pile
			//give all of pilesize and break
			nextBoard->data[nextCol][nextRow] += pileSize;
			pileSize = 0;
			break;
		}
		//enough in pile, transfer wanted then next move
		pileSize -= transferWanted;
		nextBoard->data[nextCol][nextRow] += transferWanted;
		
	}
	//dump remainder in last legal square
	if (pileSize != 0)
	{
		nextBoard->data[nextCol][nextRow] += pileSize;
	}

	return nextBoard;
}

bool Board::isTrapped(Color color)
{
	//checks if given color is trapped in board
	//not trapped if
		//there are any connected piles
		//there are more than 3 non-connected piles

	//check if 3 or less piles have any valid moves

	//arrays to record pile locations
	int x[3]; //columns
	int y[3]; //rows
	int size = 0;

	//loop to record pile locations
	for (int row = 0; row < MAX_LENGTH; row++) 
	{
		for (int col = 0; col < MAX_LENGTH; col++)
		{
			if (getColor(col, row) == color) 
			{
				if (size >= 3)
					return false; //more than 3 piles found and cannot be trapped
				
				x[size] = col;
				y[size] = row;
				size++;
			}
		}
	}

	//see if pile locations have any valid move
	for (int i = 0; i < size; i++)
	{
		for (int hor = -1; hor < 2; hor++)
		{
			for (int vir = -1; vir < 2; vir++)
			{
				//cycle through surrounding squares, if any valid then player is not trapped
				if (hor == 0 && vir == 0)
					continue;
				if (isValidPlacement(color, x[i] + hor, y[i] + vir))
					return false;
			}
		}
	}
	return true;
}

int Board::eval_zero(Board* board, Color color)
{
	//baseline heuristic that just evaluates leaf nodes to 0
	return 0;
}

int Board::eval_pileCount(Board* board, Color color)
{
	//heuristic that favors more piles of stones than opponenet
	//lightweight, but does not encorage owning empty space, just owning squares with stones
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

int Board::eval_blobCount(Board* board, Color color)
{
	//heuristic that values owning squares with stones as well as empty space
	//counts piles like pileCount, but always counts blobs of empty space
	//blobs are expanded and their areas are summed
	//blobs also track which color owns/has access to the blob
	//a shared blob accounts to zero net total
	int maxCount = 0;
	int minCount = 0;

	bool** visit = new bool* [Board::MAX_LENGTH]; //tracks which squares have been blob counted
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
			else if (!visit[col][row])
			{
				//is empty and not explored yet, now blob explore and count
				//set up flag pointers
				bool isMax = false;
				bool isMin = false;

				//conduct search
				int blobCount = board->exploreBlob(color, col, row, visit, &isMax, &isMin);
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

int Board::exploreBlob(Color color, short col, short row, bool** visit, bool* isMax, bool* isMin)
{
	//assumes that given square is empty
	//explores all connected empty squares, counting total number
	//set isMax and isMin flags if a square with specified color is connected
	int count = 1;
	visit[col][row] = true;
	for (int hor = -1; hor < 2; hor++)
	{
		for (int vir = -1; vir < 2; vir++)
		{
			if (hor == 0 && vir == 0)
				continue;
			int nextCol = col + hor;
			int nextRow = row + vir;
			if (nextCol < MAX_LENGTH && nextCol >= 0 && nextRow < MAX_LENGTH && nextRow >= 0)
			{
				Color currentColor = getColor(nextCol, nextRow);
				if (currentColor == color) //max color
					*isMax = true;
				else if (currentColor != Color::empty) //min color
					*isMin = true;
				else if (!visit[nextCol][nextRow]) //empty and not visited
				{
					count += exploreBlob(color, nextCol, nextRow, visit, isMax, isMin);
				}
			}

		}
	}

	return count;
}


