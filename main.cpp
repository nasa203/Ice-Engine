#include <bits/stdc++.h>
#include "state.h"
using namespace std;
typedef vector<vector<optional<Piece>>> Board;
Board mainBoard;


void resetState(Board board, string fenString){
    board.resize(8, std::vector<optional<Piece>>(8));
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            board[i][j] = Piece(true, _default[i][j]);
            if (i<3) board[i][j]->color = false;
        }
    }
}
int main() {
    string command;
    
    while (getline(cin, command)) {
        if (command == "uci") {
            cout << "id name myChessEngine\n" << "id author me\n" << "uciok" << endl;
        }
        else if (command == "isready") {
            cout << "readyok"<< endl;
        }
        else if (command == "ucinewgame") {
            resetState(mainBoard, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        }
        else if (command.rfind("position", 0) == 0) {
            // parse player move
        }
        else if (command.rfind("go", 0) == 0) {
            // Run search and output bestmove
        }
        else if (command == "quit") {
            break;
        }
    }
}