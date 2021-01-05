// Conga_AI.cpp 

#include <iostream>
#include "RandomAgent.h"
#include "MiniMaxAgent.h"
#include "Board.h"
#include "AgentColor.h"

using namespace std;

int main()
{
   
    Board* board = new Board();
    Board* nextBoard = nullptr;
    Agent* agent_1 = new MiniMaxAgent(Color::black, 8, Board::eval_blobCount); //goes first
    Agent* agent_2 = new RandomAgent(Color::white); //goes second
    Agent* currentAgent = nullptr;

    board->print();
    cout << endl;
    int turnCount = 0;
    do
    {
        if (currentAgent == agent_1)
            currentAgent = agent_2;
        else            
            currentAgent = agent_1;
        cout << std::dec << ++turnCount << " " << currentAgent->colorToString() << endl;
            
        nextBoard = currentAgent->takeTurn(board);
        if (nextBoard != nullptr)
        {
            delete board;
            board = nextBoard;
        }
        else
        {
            cout << "Error generating " + currentAgent->colorToString() + "'s turn" << endl;
            break; //game ended
        }

        board->print();
        currentAgent->printTurnStats();
        cout << endl;
    } while(!board->isTrapped(currentAgent->color == Color::black ? Color::white : Color::black));

    cout << "========================================" << endl;

    //verfiy game has ended correctly
    if (board->isTrapped(currentAgent->color == Color::black ? Color::white : Color::black))
        cout << "Game Ended: " + currentAgent->colorToString() + " has won." << endl;
    else
        cout << "Game Ended: Error, it was a draw or game ended prematurely." << endl;

    cout << endl;
    board->print();
    cout << endl;

    cout << "Total turns: " << std::dec << turnCount << endl <<endl;

    agent_1->printGameStats();
    cout << endl;
    agent_2->printGameStats();

    system("pause");
}

