#include "RandomAgent.h"

RandomAgent::RandomAgent(Color setColor)
{
	color = setColor;
	
	lengthDist = std::uniform_int_distribution<rng_type::result_type>(0, 3);
	directionDist = std::uniform_int_distribution<rng_type::result_type>(0, 2);
	rng_type::result_type const seed = std::chrono::system_clock::now().time_since_epoch().count();
	rng.seed(seed);
}

Board* RandomAgent::takeTurn(Board* board)
{
	turnCount++;
	rng_type::result_type random_row = lengthDist(rng);
	rng_type::result_type random_col = lengthDist(rng);
	rng_type::result_type random_hor = directionDist(rng);
	rng_type::result_type random_vir = directionDist(rng);

	for (int row = 0; row < board->MAX_LENGTH; row++)
	{
		for (int col = 0; col < board->MAX_LENGTH; col++)
		{
			//check every board square starting at a random square
			short r = (row + random_row) % board->MAX_LENGTH;
			short c = (col + random_col) % board->MAX_LENGTH;
			Color currentColor = board->getColor(c, r);
			if (color == currentColor)
			{
				//if agents color, then find a move in that square
				Board* nextBoard;
				for (int hor = 0; hor <= 2; hor++)
				{
					for (int vir = 0; vir <= 2; vir++)
					{
						//check every direction of that square
						short h = ((hor + random_hor) % 3) -1;
						short v = ((vir + random_vir) % 3) -1;
						 
						if (h == 0 && v == 0)
							continue;
						nextBoard = board->genSuccessor(c, r, h, v);
						if (nextBoard != nullptr)
						{
							//successor found
							return nextBoard;
						}
					}
				}
			}
		}
	}

	//no valid play avalible, agent lost
	return nullptr;
}
