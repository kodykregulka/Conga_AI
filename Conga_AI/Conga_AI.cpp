// Conga_AI.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "RandomAgent.h"
#include "MiniMaxAgent.h"
#include "Board.h"
#include "Enums.h"

using namespace std;

int main()
{
    short testData[Board::MAX_LENGTH][Board::MAX_LENGTH] = 
    {
        {0, 0, 0, 0},
        {0, 1, 1, 1},
        {0, 1, 0, 0},
        {5, 1, 0, -10},
    };
    /*Board* testBoard = new Board(testData);
    
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

    bool isMax = false;
    bool isMin = false;
    int blobCount = testBoard->exploreBlob(Color::black,2,2,visit,&isMax,&isMin);
    if (isMax)
        maxCount += blobCount;
    if (isMin)
        minCount += blobCount;

    //clean up memory
    for (int i = 0; i < Board::MAX_LENGTH; i++)
    {
        delete[] visit[i];
    }
    delete[] visit;

    cout << maxCount - minCount << endl;

    return 0;
    //------------------------
    */
    Board* board = new Board();
    Board* nextBoard = nullptr;
    //h=5 is good
    Agent* blackAgent = new MiniMaxAgent(Color::black, 8, MiniMaxAgent::eval_blobCount); //goes first
    Agent* whiteAgent = new RandomAgent(Color::white); //goes second
    Agent* currentAgent = nullptr;

    board->print();
    cout << endl;
    int turnCount = 0;
    do
    {
        if (currentAgent == blackAgent)
            currentAgent = whiteAgent;
        else            
            currentAgent = blackAgent;
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
        
        //system("pause");

        board->print();
        currentAgent->printTurnStats();
        cout << endl;
    } while(!board->isTrapped(currentAgent->color == Color::black ? Color::white : Color::black));

    cout << "========================================" << endl;

    if (board->isTrapped(Color::black))
        cout << "Game Ended: " + whiteAgent->colorToString() + " has won." << endl;
    else if (board->isTrapped(Color::white))
        cout << "Game Ended: " + blackAgent->colorToString() + " has won." << endl;
    else
        cout << "Game Ended: Error, it was a draw or game ended prematurely." << endl;

    cout << endl;
    board->print();
    cout << endl;

    cout << "Total turns: " << std::dec << turnCount << endl <<endl;

    blackAgent->printGameStats();
    cout << endl;
    whiteAgent->printGameStats();


   
    //possibly manage a limited stack of recent game boards
    //print stack when game ends revealing last n boards before end
}

