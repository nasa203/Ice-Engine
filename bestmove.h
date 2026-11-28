#pragma once
#include "state.h"
#include <string>
std::string bestMove(const Position& position);
int negamaxPlain(const Position& position, int depth);
int negamax(const Position& position, int depth, int alpha, int beta);
int negamaxOrdered(const Position& position, int depth, int alpha, int beta, bool useQ);
int evaluatePST(const Position& position);
extern long long nodes; //test
extern std::string engineName;
