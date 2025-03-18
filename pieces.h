#ifndef PIECES_H
#define PIECES_H

#include <cstdint>

typedef uint64_t Bitboard;

struct Pieces {
  Bitboard whitePieces;
  Bitboard blackPieces;
  Bitboard whiteKings;
  Bitboard blackKings;
};

#endif