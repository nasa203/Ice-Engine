#pragma once
#include "state.h"
#include <vector>
bool isSquareAttacked(const Position& position, int x, int y, bool byWhite);
std::vector<Move> pseudoLegalGen(const Position& position);
Position makeMove(const Position& position, const Move& m);
std::vector<Move> legalMoves(const Position& position);
bool validateMove(const Position& position, const Move& m);