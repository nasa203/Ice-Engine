#include <bits/stdc++.h>
#include "state.h"
#include "movegen.h"
using namespace std;
Position mainPosition;


void resetState(Position& position, string fenString){
    position.board.assign(8, std::vector<char>(8, ' '));

    istringstream ss(fenString);
    string boardFen, side, castlingRights, peasant;
    ss >> boardFen >> side >> castlingRights >> peasant;

    int x, y;
    x = 0; y = 7;
    for (int i = 0; i < boardFen.size(); i++){
        if (boardFen[i]=='/'){
            y--; x=0;
            continue;
        }
        if (isdigit(boardFen[i])){
            x += boardFen[i] - '0';
            continue;
        }
        position.board[x][y] = boardFen[i];
        x++;
    }

    position.whiteToMove = (side == "w");

    for (int i = 0; i < 4; i++) position.castle[i] = false;
    if (castlingRights.find("K")!=string::npos) position.castle[0] = true;
    if (castlingRights.find("Q")!=string::npos) position.castle[1] = true;
    if (castlingRights.find("k")!=string::npos) position.castle[2] = true;
    if (castlingRights.find("q")!=string::npos) position.castle[3] = true;

    string rank = peasant.substr(1, 1); 
    string file = peasant.substr(0, 1); 
    if (peasant.size() == 2) {
        position.eX = peasant[0] - 'a';
        position.eY = peasant[1] - '1';
    } else position.eX = position.eY = -1;
    
}

long long perft(const Position& position, int depth, bool isRoot = true){
    if (depth == 0) return 1;
    long long count = 0;
    if (isRoot){
        vector<thread> threads;
        vector<long long> results;
        for (const Move& m:legalMoves(position)){
            results.emplace_back(0);
            size_t i = results.size() - 1;
            threads.emplace_back([&, m, i]() {
                results[i] = perft(makeMove(position, m), depth - 1, false);
            });
        }
        for (auto& t : threads){
            t.join();
        }
        for (long long result : results) count += result;
    } else {
        for (const Move& m:legalMoves(position)) 
            count += perft(makeMove(position, m), depth - 1, false);
    }
    return count;
}


int main() {
    string command;
    
    while (getline(cin, command)) {
        if (command == "uci") {
            cout << "id name IceEngine\n" << "id author me\n" << "uciok" << endl;
        } else if (command == "isready") {
            cout << "readyok"<< endl;
        } else if (command == "ucinewgame") {
            resetState(mainPosition, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        } else if (command.rfind("position", 0) == 0) {
            // parse player move
        } else if (command.rfind("go", 0) == 0) {
            // Run search and output bestmove
        } else if (command == "perft"){
            //test
            int depth;
            cin >> depth;
            resetState(mainPosition, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
            cout << perft(mainPosition, depth) << endl;
        } else if (command == "quit") {
            break;
        }
    }
}
