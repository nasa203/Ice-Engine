#include <bits/stdc++.h>
#include "movegen.h"
#include "state.h"
using namespace std;

string engineName = "material";
long long nodes;

int pieceValue(char p){
    switch (p){
                case 'p':
                case 'P':
                    return 100;
                case 'n':
                case 'N':
                    return 320;
                case 'b':
                case 'B':
                    return 330;
                case 'r':
                case 'R':
                    return 500;
                case 'q':
                case 'Q':
                    return 900;
                case 'k':
                case 'K':
                case ' ':
                    return 0;
            }
    cerr << "somethign triggered default in pieceValue";
    return 0;
}

int evaluate(const Position& position){
    int score = 0;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++){/*
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
            }*/
           score += islower(position.board[i][j]) ? -pieceValue(position.board[i][j]) : pieceValue(position.board[i][j]);
        }
    score *= position.whiteToMove ? 1 : -1;
    return score;
}


const int INF = 1000000;
const int MATE = 20000;
int negamaxPlain(const Position& position, int depth){
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
        best = max(best, -negamaxPlain(makeMove(position, m), depth - 1));
    return best;
}

int negamax(const Position& position, int depth, int alpha, int beta){
    nodes++;
    pair<int, int> king = position.whiteToMove ? position.wk : position.bk;
    bool inCheck = isSquareAttacked(position, king.first, king.second, !position.whiteToMove);
    if (depth == 0) {
        if (!inCheck)
            return evaluate(position);
    }
    vector<Move> moves = legalMoves(position);
    if (moves.size() == 0)
        return (inCheck) ? -MATE-depth : 0; 
    if (depth == 0)
        return evaluate(position);
    int best = -INF;
    int score = -INF;
    for (const Move& m : moves){
        score = -negamax(makeMove(position, m), depth - 1, -beta, -alpha);
        best = max(best, score);
        alpha = max(alpha, score);
        if (alpha>=beta) break;
    }
    return best;
}



int random(const vector<Move>& baseMoves){ //engine 1
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, baseMoves.size()-1);
    return dis(gen);
}

int mostMaterial(const Position& position, const vector<Move>& baseMoves, int depth){ //engine 2
    int bestScore = -INF;
    vector<int> bestIndices;
    for (int i = 0; i < (int)baseMoves.size(); i++){
        int score = -negamax(makeMove(position, baseMoves[i]), depth-1, -INF, INF);
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




int scoreMove(const Position& position, const Move& m){
    int score = 0;
    char target = position.board[m.nextX][m.nextY];
    if (m.promotion != ' ')
        score +=  800 + pieceValue(m.promotion);
    if (target != ' ')
        score +=  (10 * pieceValue(target)) - pieceValue(position.board[m.x][m.y]); 
    else if (m.isPeasant)
        score += 10 * pieceValue('p') - pieceValue('p');
    return score;
}

int negamaxOrdered(const Position& position, int depth, int alpha, int beta){
    nodes++;
    pair<int, int> king = position.whiteToMove ? position.wk : position.bk;
    bool inCheck = isSquareAttacked(position, king.first, king.second, !position.whiteToMove);
    if (depth == 0) {
        if (!inCheck)
            return evaluate(position);
    }
    vector<Move> moves = legalMoves(position);
    if (moves.size() == 0)
        return (inCheck) ? -MATE-depth : 0; 
    vector<pair<int, Move>> scored;
    for (const Move& m : moves) scored.push_back({scoreMove(position, m), m});
    stable_sort(scored.begin(), scored.end(), [](const auto& a, const auto& b){ return a.first > b.first;});
    if (depth == 0)
        return evaluate(position);
    int best = -INF;
    int score = -INF;
    for (const auto& entry : scored){
        const Move& m = entry.second;
        score = -negamaxOrdered(makeMove(position, m), depth - 1, -beta, -alpha);
        best = max(best, score);
        alpha = max(alpha, score);
        if (alpha>=beta) break;
    }
    return best;
}



int snowflake(const Position& position, const vector<Move>& baseMoves, int depth){ //engine 3
    int bestScore = -INF;
    vector<int> bestIndices;
    for (int i = 0; i < (int)baseMoves.size(); i++){
        int score = -negamaxOrdered(makeMove(position, baseMoves[i]), depth-1, -INF, INF);
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
    if (engineName == "random")   return random(moves);
    if (engineName == "material") return mostMaterial(position, moves, 4);
    if (engineName == "materialLite") return mostMaterial(position, moves, 3);
    if (engineName == "materialHeavy") return mostMaterial(position, moves, 5);
    if (engineName == "snowflake")  return snowflake(position, moves, 6);
    cerr << "unknown engine '" << engineName << "', falling back to random\n";
    return random(moves);
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

