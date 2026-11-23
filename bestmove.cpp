#include <bits/stdc++.h>
#include "movegen.h"
#include "state.h"
using namespace std;

string bestMove(const Position& position){
    //temporary move picker
    vector<Move> moves = legalMoves(position);
    if (moves.empty()) return "0000";
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, moves.size() - 1);
    long long i = dist(gen);
    string result;
    result += char(moves[i].x + 'a');
    result += char(moves[i].y + '1');
    result += char(moves[i].nextX + 'a');
    result += char(moves[i].nextY + '1');
    if (moves[i].promotion != ' ') result += moves[i].promotion;
    return result;

}