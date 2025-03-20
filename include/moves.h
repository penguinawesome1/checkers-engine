#ifndef MOVES_H
#define MOVES_H

#include "pieces.h"
#include <vector>

class Moves {
private:
  std::vector<Checkers::MoveData> moveHistory;

  void decodeMoveSquares(const Checkers::MoveCords &move,
                         Checkers::Bitboard &startSquare,
                         Checkers::Bitboard &endSquare) const;
  Checkers::PieceName getBoardName(const Checkers::Pieces &pieces,
                                   const Checkers::Bitboard &board) const;
  Checkers::Bitboard &getBoard(Checkers::Pieces &pieces,
                               Checkers::PieceName name) const;
  void tryToDeleteCapture(Checkers::Pieces &pieces,
                          Checkers::MoveData &moveData,
                          const Checkers::Bitboard captureSquare) const;
  void tryToMove(Checkers::Pieces &pieces, Checkers::MoveData &moveData,
                 const Checkers::Bitboard &startSquare,
                 const Checkers::Bitboard &endSquare) const;
  void tryToPromote(Checkers::Pieces &pieces, Checkers::MoveData &moveData,
                    const Checkers::Bitboard endSquare) const;
  void addMoves(std::vector<Checkers::MoveCords> &allMoves,
                const Checkers::Bitboard selectedBoard,
                const Checkers::Bitboard occupied, const int shift,
                const int rowOffset, const int colOffset,
                const Checkers::Bitboard colMask,
                const Checkers::Bitboard rowMask) const;

public:
  void doMove(const Checkers::MoveCords move, Checkers::Pieces &pieces,
              const bool isWhiteTurn);
  void undoMove(Checkers::Pieces &pieces);
  std::vector<Checkers::MoveCords> possibleMoves(const Checkers::Pieces &pieces,
                                                 const bool isWhiteTurn) const;
};

#endif