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

Board* Board::genSuccessors(short col, short row, short hor, short vir)
{
	//assumes square originally pointed to is valid
	//if move not possible, return nullptr

	Color color = getColor(col, row);

	//checks if at least one space is avalible, NEEDED
	if (!isValidPlacement(color, col+hor, row+vir))
		return nullptr; //not a valid move

	//Now we know the move is valid, create board with that move
	Board *nextBoard = new Board(*this);
	int pileSize = nextBoard->data[col][row];
	nextBoard->data[col][row] = 0;
	int modifier = color == Color::black ? 1 : -1;

	int nextCol = col;
	int nextRow = row;

	for (int i = 1; i < MAX_LENGTH; i++)
	{
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
		if (pileSize*modifier <= transferWanted*modifier)
		{
			//not enough in pile
			//give all of pilesize and break
			nextBoard->data[nextCol][nextRow] += pileSize;
			pileSize = 0;
			break;
		}
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
					return false; //more than 3 piles found
				
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

int Board::exploreBlob(Color color, short col, short row, bool** visit, bool* isMax, bool* isMin)
{
	//assumes that given square is empty
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


