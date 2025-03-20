#ifndef PIECES_H
#define PIECES_H

#include <cstdint>

namespace Checkers {

typedef uint32_t Bitboard;

// save row masks for moves
extern const Bitboard ROW_1;
extern const Bitboard ROW_8;

// save column masks for moves
extern const Bitboard COL_1;
extern const Bitboard COL_1_2;
extern const Bitboard COL_4;
extern const Bitboard COL_3_4;

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

  bool operator==(const MoveCords &other) const;

  MoveCords() : x1(0), y1(0), x2(0), y2(0) {}
};

} // namespace Checkers

#endif