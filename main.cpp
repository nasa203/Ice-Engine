#include <bits/stdc++.h>
#include "state.h"
#include "movegen.h"
#include "bestmove.h"
using namespace std;
Position mainPosition;

template <typename T>
void print2D(const std::vector<std::vector<T>>& matrix) {
    for (const auto& row : matrix) {
        for (const auto& elem : row) {
            cerr << elem << " ";
        }
        cerr << endl;
    }
}



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
    /*
    string rank = peasant.substr(1, 1); 
    string file = peasant.substr(0, 1); 
    */
    if (peasant.size() == 2) {
        position.eX = peasant[0] - 'a';
        position.eY = peasant[1] - '1';
    } else position.eX = position.eY = -1;
    
}

long long perft(const Position& position, int depth){
    if (depth <= 1) return perft(position, depth);
    vector<Move> moves = legalMoves(position);
    vector<long long> results(moves.size());   
    vector<thread> threads;
    for (size_t i = 0; i < moves.size(); i++){
        threads.emplace_back([&, i]() {
            results[i] = perft(makeMove(position, moves[i]), depth - 1);
        });
    }
    for (thread& t : threads) t.join();        
    long long total = 0;
    for (long long r : results) total += r;
    return total;
}

bool applyUciMove(Position& pos, const string& uci){
    int startX = uci[0] - 'a';
    int startY = uci[1] - '1';
    int toX = uci[2] - 'a';
    int toY = uci[3] - '1';
    char promotion = (uci.size() == 5) ? uci[4] : ' ';
    vector<Move> moves = legalMoves(pos);
    for (int i = 0; i < moves.size(); i++){
        if (startX == moves[i].x && startY == moves[i].y && toX == moves[i].nextX && toY == moves[i].nextY){
            if (tolower(promotion) == tolower(moves[i].promotion)){
                pos = makeMove(pos, moves[i]);
                return true;
            }
        }
    }
    return false;
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
            istringstream ss(command);
            string word;
            ss >> word; ss >> word;
            if (word == "startpos"){
                resetState(mainPosition, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
                ss >> word;
            }
            else if (word == "fen"){
                string fenStr;
                while (ss >> word && word != "moves")
                    fenStr += (word + " ");
                resetState(mainPosition, fenStr);
            }
            if (word == "moves")
                while (ss>>word)
                    if (!applyUciMove(mainPosition, word)) {
                        cerr << "movegen missed a move at " << word << " with position " ;
                        print2D(mainPosition.board);
                    }

        } else if (command.rfind("go", 0) == 0) {
            // Run search and output bestmove
            cout << "bestmove " << bestMove(mainPosition) << endl;
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
