#include "pieces.h"

namespace Checkers {

// Define the constant variables
const Bitboard ROW_1 = 0xF0000000;
const Bitboard ROW_8 = 0xF;

const Bitboard COL_1 = 0x88888888;
const Bitboard COL_1_2 = 0xCCCCCCCC;
const Bitboard COL_4 = 0x11111111;
const Bitboard COL_3_4 = 0x33333333;

// Implement the MoveCords operator==
bool MoveCords::operator==(const MoveCords &other) const {
  return x1 == other.x1 && y1 == other.y1 && x2 == other.x2 && y2 == other.y2;
}

} // namespace Checkers