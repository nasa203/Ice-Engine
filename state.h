#pragma once
#include <vector>
#include <utility>
typedef bool pieceColor;
struct Position{
    std::array<std::array<char, 8>, 8> board;
    bool whiteToMove;
    bool castle[4]; // wk, wq, bk, bq
    int eX, eY; // the peasant
    std::pair<int, int> wk;
    std::pair<int, int> bk;
};


struct Move {
    int x, y, nextX, nextY;
    char promotion = ' ';  
    bool doubleMoved = false;
    bool isPeasant = false;
    bool isCastle = false;

    Move(int x, int y, int nextX,  int nextY,    char promotion = ' ', bool doubleMoved = false, bool isPeasant = false, bool isCastle = false)
        : x(x), y(y), nextX(nextX), nextY(nextY), promotion(promotion), doubleMoved(doubleMoved), isPeasant(isPeasant), isCastle(isCastle) {}
};