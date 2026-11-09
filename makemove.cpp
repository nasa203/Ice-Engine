#include <bits/stdc++.h>
#include "piece.h"
using namespace std;
typedef vector<vector<optional<Piece>>> Board;
Board makeMove(const Board& board, const Move& m){
    Board next = board;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (next[i][j] != nullopt) next[i][j]->justDoubleMoved = false;    

    next[m.nextX][m.nextY] = next[m.x][m.y];
    next[m.x][m.y] = nullopt;
    next[m.nextX][m.nextY]->hasMoved = true;
    next[m.nextX][m.nextY]->justDoubleMoved = m.doubleMoved;
    if (m.isCastle){
        if (m.nextX > m.x){
            next[m.nextX-1][m.y] = next[7][m.y];
            next[7][m.y] = nullopt;

        } else {
            next[m.nextX+1][m.y] = next[0][m.y];
            next[0][m.y] = nullopt;

        }
    }
    if (m.isPeasant) next[m.nextX][m.y] = nullopt;
    if (m.promotion != nullopt) next[m.nextX][m.nextY]->piece = m.promotion->piece;    
    return next;
}