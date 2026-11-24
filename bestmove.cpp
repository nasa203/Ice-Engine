#include <bits/stdc++.h>
#include "movegen.h"
#include "state.h"
using namespace std;

string engineName = "material";

int evaluate(const Position& position){
    int score = 0;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++){
            switch (position.board[i][j]){
                case 'p':
                case 'P':
                    score += islower(position.board[i][j]) ? -100 : 100;
                    break;
                case 'n':
                case 'N':
                    score += islower(position.board[i][j]) ? -320 : 320;
                    break;
                case 'b':
                case 'B':
                    score += islower(position.board[i][j]) ? -330 : 330;
                    break;
                case 'r':
                case 'R':
                    score += islower(position.board[i][j]) ? -500 : 500;
                    break;
                case 'q':
                case 'Q':
                    score += islower(position.board[i][j]) ? -900 : 900;
                    break;
            }
        }
    score *= position.whiteToMove ? 1 : -1;
    return score;
}


const int INF = 1000000;
const int MATE = 100000;
int negamax(const Position& position, int depth){
    vector<Move> moves = legalMoves(position);
    if (moves.empty()){
        char king = position.whiteToMove ? 'K' : 'k';
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                if (position.board[i][j] == king &&
                    isSquareAttacked(position, i, j, !position.whiteToMove))
                    return -MATE - depth;
        return 0;  
    }
    if (depth == 0) return evaluate(position);
    int best = -INF;
    for (const Move& m : moves)
        best = max(best, -negamax(makeMove(position, m), depth - 1));
    return best;
}



int random(const Position& position, vector<Move> baseMoves){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, baseMoves.size()-1);
    return dis(gen);
}

int mostMaterial(const Position& position, const vector<Move>& baseMoves){
    int bestScore = -INF;
    vector<int> bestIndices;
    for (int i = 0; i < (int)baseMoves.size(); i++){
        int score = -negamax(makeMove(position, baseMoves[i]), 2);
        if (score > bestScore){
            bestScore = score;
            bestIndices.clear();
            bestIndices.push_back(i);
        } else if (score == bestScore){
            bestIndices.push_back(i);
        }
    }
    if (bestIndices.empty()){
        cerr << "mostMaterial found no moves\n";
        return 0;
    }
    static mt19937 gen(random_device{}());
    uniform_int_distribution<int> dis(0, bestIndices.size() - 1);
    return bestIndices[dis(gen)];
}





int pickIndex(const Position& position, const vector<Move>& moves){
    if (engineName == "random")   return random(position, moves);
    if (engineName == "material") return mostMaterial(position, moves);
    cerr << "unknown engine '" << engineName << "', falling back to random\n";
    return random(position, moves);
}

string bestMove(const Position& position){
    vector<Move> moves = legalMoves(position);
    if (moves.empty()) return "0000";
    int i = pickIndex(position, moves);
    if (i < 0 || i >= (int)moves.size()) i = 0; 

    string result;
    result += char(moves[i].x + 'a');
    result += char(moves[i].y + '1');
    result += char(moves[i].nextX + 'a');
    result += char(moves[i].nextY + '1');
    if (moves[i].promotion != ' ') result += tolower(moves[i].promotion);
    return result;
}

