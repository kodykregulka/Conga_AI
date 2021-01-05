//RandomAgent.h
#pragma once
#include "Agent.h"
#include <random>
#include <chrono>

class RandomAgent :
	public Agent
{
	typedef std::mt19937 rng_type;
	std::uniform_int_distribution<rng_type::result_type> lengthDist;
	std::uniform_int_distribution<rng_type::result_type> directionDist;
	rng_type rng;

public:
	RandomAgent(Color setColor);
	Board* takeTurn(Board* board = nullptr);
};

