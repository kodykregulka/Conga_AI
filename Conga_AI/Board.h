#pragma once
#include <string>
#include <iostream>
#include "Enums.h"


class Board
{
public:
    static const int MAX_LENGTH = 4;
private:
    short data[MAX_LENGTH][MAX_LENGTH];
    bool isValidPlacement(Color color, short col, short row);
public:  
    Board();
    Board(const Board &b);
    Board(short d[MAX_LENGTH][MAX_LENGTH]);

    void print();
    Color getColor(short col, short row);
    
    Board* genSuccessors(short col, short row, short hor, short vir);
    bool isTrapped(Color color);

    int exploreBlob(Color color, short col, short row, bool** visit, bool* isMax, bool* isMin);
};

