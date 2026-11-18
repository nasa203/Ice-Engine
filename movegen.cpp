#include <bits/stdc++.h>
#include "state.h"
using namespace std;
// toMove == isWhite
// piece.value.color == isWhite
bool isSquareAttacked(const Position& position, int x, int y, bool byWhite) {
    auto at = [&](int a, int b, char p) {   // is there an attacker's piece 
        return a >= 0 && a < 8 && b >= 0 && b < 8 && position.board[a][b] == (byWhite ? toupper(p) : p);
    };
    int dir = (byWhite) ? -1 : 1;
    //pawn
    for (int i : {1, -1}){
        if (at(x+i, y+dir, 'p')) return true;
    }
    //knight
    pair<int, int> knightMoves[] = {
        {1, 2},
        {2, 1},
        {-1, 2},
        {2, -1},
        {-2, 1},
        {1, -2}, 
        {-1, -2},
        {-2, -1}
    };
    for (auto[i, j] : knightMoves){
        int nx = x + i;
        int ny = y + j;
        if (at(nx, ny, 'n')) return true;
    }
    //cardinal sliders
    pair<int, int> cardinalMoves[] = {
        {0, 1},
        {0, -1},
        {1, 0},
        {-1, 0}
    };
        for (auto [dx, dy] : cardinalMoves) {
            int nx = x + dx, ny = y + dy;
            while (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
                char c = position.board[nx][ny];
                if (c != ' ') {
                    if (at(nx, ny, 'r') || at(nx, ny, 'q')) return true;
                    break;  
                }
                nx += dx; ny += dy;
            }
        }
    //diagonal sliders
    pair<int, int> diagMoves[] = {
        {1, 1},
        {-1, -1},
        {1, -1},
        {-1, 1}
    };
    for (auto [dx, dy] : diagMoves) {
        int nx = x + dx, ny = y + dy;
        while (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
            char c = position.board[nx][ny];
            if (c != ' ') {
                if (at(nx, ny, 'b') || at(nx, ny, 'q')) return true;
                break;   
            }
            nx += dx; ny += dy;
        }
    }
    //king
    pair<int, int> kingMoves[] = {
        {1, 1},
        {-1, -1},
        {-1, 1},
        {1, -1},
        {0, 1},
        {1, 0}, 
        {-1, 0},
        {0, -1}
    };
    for (auto[i, j] : kingMoves){
        int nx = x + i;
        int ny = y + j;
        if (at(nx, ny, 'k')) return true;
    }
    return false;
}
vector<Move> pseudoLegalGen(const Position& position){
    auto isFriendly  = [&](char c){ return c != ' ' && (bool)isupper(c) == position.whiteToMove;};
    vector<Move> moves;
    for (int i = 0; i < position.board.size(); i++){
        for (int j = 0; j < position.board.size(); j++){
            if (position.board[i][j]==' ') continue;
            if ((isupper(position.board[i][j]) && position.whiteToMove)||(islower(position.board[i][j]) && !position.whiteToMove)){
                switch (position.board[i][j]){
                    case 'p':
                    case 'P': {
                        int dir = position.whiteToMove ? 1 : -1;
                        int sRank = position.whiteToMove ? 1 : 6;
                        int pRank = position.whiteToMove ? 7 : 0;
                        int ny = j + dir;
                        auto addPawnMove = [&](int nx, int ny) {
                            if (ny == pRank) {
                                for (auto k : {'r', 'n', 'q', 'b'})
                                    moves.emplace_back(i, j, nx, ny, (position.whiteToMove) ? toupper(k) : k);
                            } else moves.emplace_back(i, j, nx, ny, ' ');
                        };
                        // fwd
                        if (ny >= 0 && ny < 8 && position.board[i][ny] == ' ') {
                            addPawnMove(i, ny);
                            if (j == sRank && position.board[i][j + 2 * dir] == ' ') moves.emplace_back(i, j, i, j + 2 * dir, ' ' , true);
                        }
                        // captures and the peasant
                        for (int dx : {-1, 1}) {
                            int nx = i + dx;
                            if (nx < 0 || nx >= 8 || ny < 0 || ny >= 8) continue;
                            // capture
                            if (position.board[nx][ny] != ' ' && !isFriendly(position.board[nx][ny])) addPawnMove(nx, ny);
                            // the peasant
                            // moved out

                        }
                        break;
                    }
                    case 'R':
                    case 'r':{
                        pair<int, int> ranks[] = {
                            {1, 0},
                            {-1, 0},
                            {0, -1},
                            {0, 1}
                        };
                        for (auto[dx, dy]:ranks){
                            int nx = i, ny = j;
                            while(true){
                                nx = nx + dx;
                                ny = ny + dy;
                                if (nx > 7 || nx < 0 || ny > 7 || ny < 0) break;
                                if (position.board[nx][ny] == ' '){
                                    moves.emplace_back(i, j, nx, ny, ' ');
                                    continue;
                                }
                                if (position.board[nx][ny] != ' ' && !isFriendly(position.board[nx][ny])){
                                    moves.emplace_back(i, j, nx, ny, ' ');
                                    break;
                                }
                                if (isFriendly(position.board[nx][ny])) break;
                            }
                        }
                        break;
                    }
                    case 'n':
                    case 'N':{
                        pair<int, int> knightMoves[] = {
                            {1, 2},
                            {2, 1},
                            {-1, 2},
                            {2, -1},
                            {-2, 1},
                            {1, -2}, 
                            {-1, -2},
                            {-2, -1}
                        };
                        for (auto[dx, dy]:knightMoves){
                            int nx = i + dx, ny = j + dy;
                            if (nx < 0 || nx > 7 || ny < 0 || ny > 7) continue;
                            if (position.board[nx][ny] == ' ') moves.emplace_back(i, j, nx, ny, ' ');
                            if (position.board[nx][ny] != ' ' && !isFriendly(position.board[nx][ny])) moves.emplace_back(i, j, nx, ny, ' ');
                        }
                        break;
                    }
                    case 'b':
                    case 'B':{
                        //keep going in each diagonal, if hits a piece or edge stop
                        //if enemy piece then add that as a capture
                        pair<int, int> diagonals[] = {
                            {1, 1},
                            {-1, -1},
                            {1, -1},
                            {-1, 1}
                        };
                        for (auto[dx, dy] : diagonals){
                            int nx = i, ny = j;
                            while(true){
                                nx = nx + dx;
                                ny = ny + dy;
                                if (nx > 7 || nx < 0 || ny > 7 || ny < 0) break;
                                if (position.board[nx][ny] == ' '){
                                    moves.emplace_back(i, j, nx, ny, ' ');
                                    continue;
                                }
                                if (position.board[nx][ny] != ' ' && !isFriendly(position.board[nx][ny])){
                                    moves.emplace_back(i, j, nx, ny, ' ');
                                    break;
                                }
                                if (isFriendly(position.board[nx][ny])) break;
                            }
                        }
                        break;
                    }
                    case 'q':
                    case 'Q':{
                        pair<int, int> directions[] = {
                            {1, 1},
                            {-1, -1},
                            {1, -1},
                            {-1, 1},
                            {0, 1},
                            {1, 0},
                            {0, -1},
                            {-1, 0}
                        };
                        for (auto[dx, dy] : directions){
                            int nx = i, ny = j;
                            while(true){
                                nx = nx + dx;
                                ny = ny + dy;
                                if (nx > 7 || nx < 0 || ny > 7 || ny < 0) break;
                                if (position.board[nx][ny] == ' '){
                                    moves.emplace_back(i, j, nx, ny, ' ');
                                    continue;
                                }
                                if (position.board[nx][ny] != ' ' && !isFriendly(position.board[nx][ny])){
                                    moves.emplace_back(i, j, nx, ny, ' ');
                                    break;
                                }
                                if (isFriendly(position.board[nx][ny])) break;
                            }
                        }
                        break;
                    }
                    case 'k':
                    case 'K':{
                        pair<int, int> directions[] = {
                            {1, 1},
                            {-1, -1},
                            {1, -1},
                            {-1, 1},
                            {0, 1},
                            {1, 0},
                            {0, -1},
                            {-1, 0}
                        };
                        for (auto [dx, dy] : directions) {
                            int nx = i + dx, ny = j + dy;
                            if (nx < 0 || nx > 7 || ny < 0 || ny > 7) continue;
                            if (position.board[nx][ny] == ' ') moves.emplace_back(i, j, nx, ny, ' ');
                            else if (!isFriendly(position.board[nx][ny])) moves.emplace_back(i, j, nx, ny, ' ');
                        }
                        //castling
                        //moved out
                        break;
                    }
                    default: {cout << "something triggered default in the pseudolegalgen"; break;};
                }
            }
        }
    }
    //the peasant
    if (position.eX != -1) {
        int dir = position.whiteToMove ? 1 : -1;
        char myPawn = position.whiteToMove ? 'P' : 'p';
        for (int dx : {-1, 1}) {
            int fx = position.eX + dx, fy = position.eY - dir;
            if (fx >= 0 && fx < 8 && position.board[fx][fy] == myPawn)
                moves.emplace_back(fx, fy, position.eX, position.eY, ' ', false, true);
        }
    }
    //castling
    if (position.whiteToMove){
        if (!isSquareAttacked(position, 4, 0, !position.whiteToMove)){
            if (position.castle[0]){
                if (position.board[5][0] == ' ' && position.board[6][0] == ' ' &&
                     !isSquareAttacked(position, 5, 0, !position.whiteToMove)&&!isSquareAttacked(position, 6, 0, !position.whiteToMove))
                    moves.emplace_back(4, 0, 6, 0, ' ', false, false, true);
            }
            if (position.castle[1]){
                if (position.board[3][0] == ' ' && position.board[2][0] == ' ' && position.board[1][0] == ' ' && 
                    !isSquareAttacked(position, 3, 0, !position.whiteToMove)&&!isSquareAttacked(position, 2, 0, !position.whiteToMove))
                    moves.emplace_back(4, 0, 2, 0, ' ', false, false, true);
            }
        }
    } else {
        if (!isSquareAttacked(position, 4, 7, !position.whiteToMove)){
            if (position.castle[2]){
                if (position.board[5][7] == ' ' && position.board[6][7] == ' ' &&
                    !isSquareAttacked(position, 5, 7, !position.whiteToMove)&&!isSquareAttacked(position, 6, 7, !position.whiteToMove))
                    moves.emplace_back(4, 7, 6, 7, ' ', false, false, true);
            }
            if (position.castle[3]){
                if (position.board[3][7] == ' ' && position.board[2][7] == ' ' && position.board[1][7] == ' ' && 
                    !isSquareAttacked(position, 3, 7, !position.whiteToMove)&&!isSquareAttacked(position, 2, 7, !position.whiteToMove))
                    moves.emplace_back(4, 7, 2, 7, ' ', false, false, true);
            }
        }
    }
    return moves;
}





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


bool validateMove(){
    
}