#include <bits/stdc++.h>
#include "state.h"
using namespace std;
// toMove == isWhite
// piece.value.color == isWhite
bool isSquareAttacked(const Position& position, int x, int y, bool whiteToMove) {
    auto isFriendly  = [&](char c){ return c != ' ' && (bool)isupper(c) == position.whiteToMove;};

    int dir = (whiteToMove) ? 1 : -1;
    //pawn
    for (int i : {1, -1}){
        if (tolower(position.board[x+i][y+dir]) == 'p' && !isFriendly(position.board[x+i][y+dir])) return true;
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
        if (tolower(position.board[nx][ny]) == 'n' && !isFriendly(position.board[nx][ny])) return true;
    }
    //cardinal sliders
    pair<int, int> cardinalMoves[] = {
        {0, 1},
        {0, -1},
        {1, 0},
        {-1, 0}
    };
    

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
        if (position.castle[0]){
            
        }
        if (position.castle[1]){

        }
    } else {
        if (position.castle[2]){

        }
        if (position.castle[3]){

        }
    }
    return moves;
}