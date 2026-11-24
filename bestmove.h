#pragma once
#include "state.h"
#include <string>
std::string bestMove(const Position& position);
int negamaxPlain(const Position& position, int depth);
int negamax(const Position& position, int depth, int alpha, int beta);
extern std::string engineName;