#include <bits/stdc++.h>
#include "state.h"
using namespace std;
Position makeMove(const Position& position, const Move& m){
    Position next = position;
    // disable castling, remove peasant rights
    auto check = [&](int x, int y) {
        if (x == 4 && y == 0) {next.castle[0] = next.castle[1] = false;}
        if (x == 4 && y == 7) {next.castle[2] = next.castle[3] = false;}
        if (x == 7 && y == 0) next.castle[0] = false;
        if (x == 0 && y == 0) next.castle[1] = false;
        if (x == 7 && y == 7) next.castle[2] = false;
        if (x == 0 && y == 7) next.castle[3] = false;
    };
    check(m.x, m.y);
    check(m.nextX, m.nextY);
    next.eX = next.eY = -1;

    if (m.doubleMoved) { next.eX = m.x; next.eY = (m.y + m.nextY) / 2; }

    next.board[m.nextX][m.nextY] = next.board[m.x][m.y];
    next.board[m.x][m.y] = ' ';

    if (m.isCastle){
        if (m.nextX > m.x){
            next.board[m.nextX-1][m.y] = next.board[7][m.y];
            next.board[7][m.y] = ' ';

        } else {
            next.board[m.nextX+1][m.y] = next.board[0][m.y];
            next.board[0][m.y] = ' ';

        }
    }
    if (m.isPeasant) next.board[m.nextX][m.y] = ' ';
    if (m.promotion != ' ') next.board[m.nextX][m.nextY] = m.promotion;    
    next.whiteToMove = !next.whiteToMove;
    return next;
}