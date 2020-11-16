#include "Agent.h"

Agent::Agent(Color setColor, int max_depth)
{
	color = setColor;
	maxDepth = max_depth;
	nodesExplored = 0;
	nodesExploredTotal = 0;
	branchesPruned = 0;
	branchesPrunedTotal = 0;

}

std::string Agent::colorToString()
{
	if (color == Color::black)
		return "Black(+)";
	else
		return "White(-)";
}

void Agent::printTurnStats()
{
	if (timeElapsed.count() > 0)
	{
		std::cout.imbue(std::locale(""));
		std::cout << "Elapsed time: " << std::dec << ((double)timeElapsed.count() / 1000000) << " seconds" << std::endl;
		std::cout << "Nodes explored: " << std::dec << nodesExplored << std::endl;
		std::cout << "Branches pruned: " << std::dec << branchesPruned << std::endl;
		std::cout << "Nodes per second: " << std::dec << nodesExplored / ((double)timeElapsed.count() / 1000000) << " nps" << std::endl;
	}
}

void Agent::printGameStats()
{
	if (turnCount > 0)
	{
		std::cout.imbue(std::locale(""));
		std::cout << colorToString() << " Stats" << std::endl;
		std::cout << "Turns taken: " << std::dec << turnCount << std::endl;
		std::cout << "Total turn time: " << std::dec << ((double)timeElapsedTotal.count()/1000000) << " seconds" << std::endl;
		std::cout << "Turn time average: " << std::dec << ((double)(timeElapsedTotal.count())/1000000) / turnCount << " seconds" << std::endl;
		std::cout << "Total nodes explored: " << std::dec << nodesExploredTotal << std::endl;
		std::cout << "Total branches pruned: " << std::dec << branchesPrunedTotal << std::endl;
	}
}
