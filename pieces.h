#ifndef PIECES_H
#define PIECES_H
#include <cstdint>

namespace Checkers {

typedef uint32_t Bitboard;

// save row masks for moves
const Bitboard ROW_1 = 0xF0000000;
const Bitboard ROW_8 = 0xF;

// save column masks for moves
const Bitboard COL_1 = 0x88888888;
const Bitboard COL_1_2 = 0xCCCCCCCC;
const Bitboard COL_4 = 0x11111111;
const Bitboard COL_3_4 = 0x33333333;

enum PieceName { WhitePieces, BlackPieces, WhiteKings, BlackKings, None };

struct Pieces {
  Bitboard whitePieces;
  Bitboard blackPieces;
  Bitboard whiteKings;
  Bitboard blackKings;
};

struct MoveData {
  PieceName name1;
  PieceName name2;
  PieceName name3;
  Bitboard board1;
  Bitboard board2;
  Bitboard board3;
};

struct MoveCords {
  int x1;
  int y1;
  int x2;
  int y2;

  bool operator==(const MoveCords &other) const {
    return x1 == other.x1 && y1 == other.y1 && x2 == other.x2 && y2 == other.y2;
  }

  MoveCords() : x1(0), y1(0), x2(0), y2(0) {}
};

} // namespace Checkers

#endif