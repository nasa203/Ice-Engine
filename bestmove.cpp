#include <bits/stdc++.h>
#include "movegen.h"
#include "state.h"
using namespace std;

string engineName = "material";
long long nodes;



constexpr int pawn_early[8][8] = {
    { 0,  0,  0,  0,  0,  0,  0,  0},
    {50, 50, 50, 50, 50, 50, 50, 50},
    {10, 10, 20, 30, 30, 20, 10, 10},
    {5,  5, 10, 25, 25, 10,  5,   5},
    {0,  0,  0, 20, 20,  0,  0,   0},
    {5, -5,-10,  0,  0,-10, -5,   5},
    {5, 10, 10,-20,-20, 10, 10,   5},
    {0,  0,  0,  0,  0,  0,  0,   0}
};
constexpr int pawn_late[8][8] = {
    {90, 90, 90, 90, 90, 90, 90, 90},
    {80, 80, 80, 80, 80, 80, 80, 80},
    {50, 50, 50, 60, 60, 50, 50, 50},
    {30, 30, 40, 50, 50, 40, 30, 30},
    {20, 20, 30, 40, 40, 30, 20, 20},
    {10, 10, 20, 30, 30, 20, 10, 10},
    { 5,  5, 10, 20, 20, 10,  5,  5},
    { 0,  0,  0,  0,  0,  0,  0,  0}
};

constexpr int knight[8][8] = {
    {-50,-40,-30,-30,-30,-30,-40,-50},
    {-40,-20,  0,  0,  0,  0,-20,-40},
    {-30,  0, 10, 15, 15, 10,  0,-30},
    {-30,  5, 15, 20, 20, 15,  5,-30},
    {-30,  0, 15, 20, 20, 15,  0,-30},
    {-30,  5, 10, 15, 15, 10,  5,-30},
    {-40,-20,  0,  5,  5,  0,-20,-40},
    {-50,-40,-30,-30,-30,-30,-40,-50}
};
constexpr int bishop[8][8] = {
    {-20,-10,-10,-10,-10,-10,-10,-20},
    {-10,  0,  0,  0,  0,  0,  0,-10},
    {-10,  0,  5, 10, 10,  5,  0,-10},
    {-10,  5,  5, 10, 10,  5,  5,-10},
    {-10,  0, 10, 10, 10, 10,  0,-10},
    {-10, 10, 10, 10, 10, 10, 10,-10},
    {-10,  5,  0,  0,  0,  0,  5,-10},
    {-20,-10,-10,-10,-10,-10,-10,-20}
};
constexpr int rook[8][8] = {
    {  0,  0,  0,  0,  0,  0,  0,  0},
    {  5, 10, 10, 10, 10, 10, 10,  5},
    { -5,  0,  0,  0,  0,  0,  0, -5},
    { -5,  0,  0,  0,  0,  0,  0, -5},
    { -5,  0,  0,  0,  0,  0,  0, -5},
    { -5,  0,  0,  0,  0,  0,  0, -5},
    { -5,  0,  0,  0,  0,  0,  0, -5},
    {  0,  0,  0,  5,  5,  0,  0,  0}
};
constexpr int queen_early[8][8] = {
    {-10,-10,-10,-10,-10,-10,-10,-10},
    {-10, -5, -5, -5, -5, -5, -5,-10},
    { -5, -5,  0,  0,  0,  0, -5, -5},
    { -5,  0,  0,  5,  5,  0,  0, -5},
    {  0,  0,  5,  5,  5,  5,  0,  0},
    {  0,  5,  5,  5,  5,  5,  5,  0},
    {  0,  0,  0,  0,  0,  0,  0,  0},
    {  0,  0,  0,  0,  0,  0,  0,  0}
};
constexpr int queen_late[8][8] = {
    {-10, -5,  0,  5,  5,  0, -5, -10},
    { -5,  0,  5, 10, 10,  5,  0, -5},
    {  0,  5, 10, 10, 10, 10,  5,  0},
    {  5, 10, 10, 15, 15, 10, 10,  5},
    {  5, 10, 10, 15, 15, 10, 10,  5},
    {  0,  5, 10, 10, 10, 10,  5,  0},
    { -5,  0,  5,  5,  5,  5,  0, -5},
    {-10, -5,  0,  0,  0,  0, -5, -10}
};

constexpr int king_early[8][8] = {
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-20,-30,-30,-40,-40,-30,-30,-20},
    {-10,-20,-20,-20,-20,-20,-20,-10},
    { 20, 20,  0,  0,  0,  0, 20, 20},
    { 20, 30, 10,  0,  0, 10, 30, 20},
};

constexpr int king_late[8][8] = {
    {-50,-40,-30,-20,-20,-30,-40,-50},
    {-30,-20,-10,  0,  0,-10,-20,-30},
    {-30,-10, 20, 30, 30, 20,-10,-30},
    {-30,-10, 30, 40, 40, 30,-10,-30},
    {-30,-10, 30, 40, 40, 30,-10,-30},
    {-30,-10, 20, 30, 30, 20,-10,-30},
    {-30,-30,  0,  0,  0,  0,-30,-30},
    {-50,-30,-30,-30,-30,-30,-30,-50},
};

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

int pstMG(char c, int x, int y){
    switch (c){
        case ' ':
            return 0;
        case 'p':
        case 'P':
            return pawn_early[isupper(c) ? 7 - y : y][x];
        case 'n':
        case 'N':
            return knight[isupper(c) ? 7 - y : y][x];
        case 'b':
        case 'B':
            return bishop[isupper(c) ? 7 - y : y][x];
        case 'r':
        case 'R':
            return rook[isupper(c) ? 7 - y : y][x];
        case 'q':
        case 'Q':
            return queen_early[isupper(c) ? 7 - y : y][x];
        case 'k':
        case 'K':
            return king_early[isupper(c) ? 7 - y : y][x];
        default:
            cerr << "something triggered default on pstMG";
            return 0;
    }
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
    char mover = position.board[m.x][m.y];
    score += pstMG(mover, m.nextX, m.nextY) - pstMG(mover, m.x, m.y);
    return score;
}

int evaluatePST(const Position& position){
    int mg = 0, eg = 0;
    int phase = 0;
    int valueOfPiece;
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            char c = position.board[i][j];
            if (c==' ') continue;
            bool white = isupper(c);
            int sign = white ? 1 : -1;
            valueOfPiece = (white ? pieceValue(c) : -pieceValue(c));
            switch (c){
                case 'p':
                case 'P':
                    mg += sign*pstMG(c, i, j) + valueOfPiece;
                    eg += sign*pawn_late[white ? 7-j : j][i] + valueOfPiece;
                    break;
                case 'n':
                case 'N':
                    mg += sign*pstMG(c, i, j) + valueOfPiece;
                    eg += sign*pstMG(c, i, j) + valueOfPiece;
                    phase += 1;
                    break;
                case 'b':
                case 'B':
                    mg += sign*pstMG(c, i, j) + valueOfPiece;
                    eg += sign*pstMG(c, i, j) + valueOfPiece;
                    phase += 1;
                    break;
                case 'r':
                case 'R':
                    mg += sign*pstMG(c, i, j) + valueOfPiece;
                    eg += sign*pstMG(c, i, j) + valueOfPiece;
                    phase += 2;
                    break;
                case 'q':
                case 'Q':
                    mg += sign*pstMG(c, i, j) + valueOfPiece;
                    eg += sign*queen_late[white ? 7-j : j][i] + valueOfPiece;
                    phase += 4;
                    break;
                case 'k':
                case 'K':
                    mg += sign*pstMG(c, i, j) + valueOfPiece;
                    eg += sign*king_late[white ? 7-j : j][i] + valueOfPiece;
                    break;
                default:
                    cerr << "something triggered default in evalPST";
            }
                
            
        }
    }
    phase = min(phase, 24);
    int score = (mg * phase + eg * (24 - phase)) / 24;
    return (position.whiteToMove) ? score : -score;
}

int quiesce(const Position& position, int alpha, int beta){
    int standPat = evaluatePST(position);
    if (standPat >= beta) return standPat;
    alpha = max(alpha, standPat);
    vector<Move> moveList = pseudoLegalGen(position);
    vector<pair<int, Move>> scored;
    for (const Move& m : moveList)
        if ((position.board[m.nextX][m.nextY] != ' ' || m.isPeasant || m.promotion != ' ') && validateMove(position, m))
            scored.push_back({scoreMove(position, m), m});
    stable_sort(scored.begin(), scored.end(), [](const auto& a, const auto& b){ return a.first > b.first;});

    int score;
    for (int i = 0; i < scored.size(); i++){
        score = -quiesce(makeMove(position, scored[i].second), -beta, -alpha);
        if (score >= beta) return score;
        alpha = max(alpha, score);
    }
    return alpha;
    
}


int negamaxOrdered(const Position& position, int depth, int alpha, int beta, bool useQ){
    nodes++;
    pair<int, int> king = position.whiteToMove ? position.wk : position.bk;
    bool inCheck = isSquareAttacked(position, king.first, king.second, !position.whiteToMove);
    if (depth == 0) {
        if (!inCheck)
            return useQ ? quiesce(position, alpha, beta):evaluatePST(position);
    }
    vector<Move> moves = legalMoves(position);
    if (moves.size() == 0)
        return (inCheck) ? -MATE-depth : 0; 
    vector<pair<int, Move>> scored;
    for (const Move& m : moves) scored.push_back({scoreMove(position, m), m});
    stable_sort(scored.begin(), scored.end(), [](const auto& a, const auto& b){ return a.first > b.first;});
    if (depth == 0)
        return useQ ? quiesce(position, alpha, beta):evaluatePST(position);
    int best = -INF;
    int score = -INF;
    for (const auto& entry : scored){
        const Move& m = entry.second;
        score = -negamaxOrdered(makeMove(position, m), depth - 1, -beta, -alpha, useQ);
        best = max(best, score);
        alpha = max(alpha, score);
        if (alpha>=beta) break;
    }
    return best;
}



int snowflake(const Position& position, const vector<Move>& baseMoves, int depth, bool version){ //engine 3
    int bestScore = -INF;
    vector<int> bestIndices;
    for (int i = 0; i < (int)baseMoves.size(); i++){
        int score = -negamaxOrdered(makeMove(position, baseMoves[i]), depth-1, -INF, INF, version);
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
    if (engineName == "snowflake")  return snowflake(position, moves, 5, false);
    if (engineName == "iceflake")  return snowflake(position, moves, 5, true);
    cerr << "unknown engine '" << engineName << "', falling back to iceflake\n";
    return snowflake(position, moves, 5, true);
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

