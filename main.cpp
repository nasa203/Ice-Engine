#include <bits/stdc++.h>
#include "state.h"
using namespace std;
Position mainPosition;


void resetState(Position& position, string fenString){
    position.board.resize(8, std::vector<char>(8));

    string boardFen = fenString.substr(0, fenString.find(' '));
    string castlingRights = fenString.substr(2, fenString.find(' '));
    string peasant = fenString.substr(3, fenString.find(' '));

    int x, y;
    x = 0; y = 7;
    for (int i = boardFen.size() - 1; i >= 0; i--){
        if (boardFen[i]=='/'){
            y--; x=0;
            continue;
        }
        position.board[x][y] = boardFen[i];
    }

    position.whiteToMove = ("w" == fenString.substr(1, fenString.find(' ')));

    for (int i = 0; i < 3; i++) position.castle[i] = false;
    if (castlingRights.find("K")!=string::npos) position.castle[0] = true;
    if (castlingRights.find("Q")!=string::npos) position.castle[1] = true;
    if (castlingRights.find("k")!=string::npos) position.castle[2] = true;
    if (castlingRights.find("q")!=string::npos) position.castle[3] = true;

    string rank = peasant.substr(0, 1);
    string file = peasant.substr(1, 1);
    if (peasant != "-") {
        position.eX = rank[0] - 'a';
        position.eY = file[0] - '1';
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
            resetState(mainPosition, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
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