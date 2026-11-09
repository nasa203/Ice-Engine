#pragma once
typedef bool pieceColor;
struct Position{
    vector<vector<char>> board;
    bool whiteToMove;
    bool castle[4]; // wk, wq, bk, bq
    int eX, eY; // the peasant
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