#ifndef INPUT_H
#define INPUT_H

#include "pieces.h"
#include <vector>

namespace input {

Checkers::MoveCords
getPlayerMove(const std::vector<Checkers::MoveCords> possibleMoves);

} // namespace input

#endif