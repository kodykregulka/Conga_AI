#pragma once
#include "Board.h"
#include <iostream>
#include <string>
#include <chrono> 

class Agent
{
protected:
	int maxDepth;
	unsigned long long nodesExplored = 0;
	unsigned long long nodesExploredTotal = 0;
	unsigned long long branchesPruned = 0;
	unsigned long long branchesPrunedTotal = 0;
	
	std::chrono::microseconds timeElapsed = (std::chrono::microseconds)0;
	std::chrono::microseconds timeElapsedTotal = (std::chrono::microseconds)0;
	long turnCount = 0;

public:
	Agent(Color setColor = Color::black, int max_depth = 0);
	virtual Board* takeTurn(Board* board) = 0;
	Color color;
	std::string colorToString();

	void printTurnStats();
	void printGameStats();
};

