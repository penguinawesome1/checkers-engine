#ifndef MOVES_H
#define MOVES_H
#include "board.h"
#include "pieces.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class Moves {
private:
  std::vector<Checkers::MoveData> moveHistory;

  void decodeMoveSquares(const Checkers::MoveCords &move,
                         Checkers::Bitboard &startSquare,
                         Checkers::Bitboard &endSquare) const {
    const int BOARD_SIZE = 8;
    const int ROW_SIZE = 4;

    startSquare = 1L << (move.x1 + (BOARD_SIZE - 1 - move.y1) * ROW_SIZE);
    endSquare = 1L << (move.x2 + (BOARD_SIZE - 1 - move.y2) * ROW_SIZE);
  }

  Checkers::PieceName getBoardName(const Checkers::Pieces &pieces,
                                   const Checkers::Bitboard &board) const {
    using namespace Checkers;

    if (pieces.whitePieces & board) {
      return PieceName::WhitePieces;
    } else if (pieces.blackPieces & board) {
      return PieceName::BlackPieces;
    } else if (pieces.whiteKings & board) {
      return PieceName::WhiteKings;
    } else if (pieces.blackKings & board) {
      return PieceName::BlackKings;
    } else {
      return PieceName::None;
    }
  }

  Checkers::Bitboard &getBoard(Checkers::Pieces &pieces,
                               Checkers::PieceName name) const {
    using namespace Checkers;

    switch (name) {
    default:
    case PieceName::WhitePieces:
      return pieces.whitePieces;
    case PieceName::WhiteKings:
      return pieces.whiteKings;
    case PieceName::BlackPieces:
      return pieces.blackPieces;
    }
  }

  void tryToDeleteCapture(Checkers::Pieces &pieces,
                          Checkers::MoveData &moveData,
                          const Checkers::Bitboard captureSquare) const {
    Checkers::PieceName pieceName = getBoardName(pieces, captureSquare);
    Checkers::Bitboard &pieceBoard = getBoard(pieces, pieceName);

    moveData.name1 = pieceName;
    moveData.board1 = pieceBoard;

    pieceBoard ^= captureSquare;
  }

  void tryToMove(Checkers::Pieces &pieces, Checkers::MoveData &moveData,
                 const Checkers::Bitboard &startSquare,
                 const Checkers::Bitboard &endSquare) const {
    Checkers::PieceName pieceName = getBoardName(pieces, startSquare);
    Checkers::Bitboard &pieceBoard = getBoard(pieces, pieceName);

    moveData.name2 = pieceName;
    moveData.board2 = pieceBoard;

    pieceBoard ^= startSquare | endSquare;
  }

  void tryToPromote(Checkers::Pieces &pieces, Checkers::MoveData &moveData,
                    const Checkers::Bitboard endSquare) const {
    using namespace Checkers;

    PieceName pieceName = getBoardName(pieces, endSquare);
    // std::cout << "kjfsdjkl: " << pieceName;
    if (endSquare & ROW_1 && pieceName == PieceName::WhitePieces) {
      moveData.name3 = PieceName::WhiteKings;
      moveData.board3 = pieces.whiteKings;

      pieces.whitePieces ^= endSquare;
      pieces.whiteKings ^= endSquare;
    } else if (endSquare & ROW_8 && pieceName == PieceName::BlackPieces) {
      moveData.name3 = PieceName::BlackKings;
      moveData.board3 = pieces.blackKings;

      pieces.blackPieces ^= endSquare;
      pieces.blackKings ^= endSquare;
    }
  }

  void addMoves(std::vector<Checkers::MoveCords> &allMoves,
                const Checkers::Bitboard selectedBoard,
                const Checkers::Bitboard occupied, const int shift,
                const int rowOffset, const int colOffset,
                const Checkers::Bitboard colMask,
                const Checkers::Bitboard rowMask) const {
    Checkers::Bitboard moveBoard =
        (shift > 0 ? selectedBoard >> shift : selectedBoard << -shift) &
        ~occupied & colMask & rowMask;

    while (moveBoard) {
      int index = __builtin_ctzll(moveBoard);
      int col = index % 4;
      int row = index / 4;
      Checkers::MoveCords move;
      move.x1 = row + rowOffset;
      move.y1 = 7 - (col + colOffset);
      move.x2 = row;
      move.y2 = 7 - col;
      allMoves.push_back(move);
      moveBoard &= moveBoard - 1;
    }
  }

public:
  void doMove(const Checkers::MoveCords move, Checkers::Pieces &pieces,
              const bool isWhiteTurn) {
    using namespace Checkers;

    MoveData moveData;
    Bitboard startSquare, endSquare;
    decodeMoveSquares(move, startSquare, endSquare);
    // const Bitboard captureSquare =
    //     isWhiteTurn ? startSquare << 8 : startSquare >> 8;

    // tryToDeleteCapture(pieces, moveData, captureSquare);
    tryToMove(pieces, moveData, startSquare, endSquare);
    // tryToPromote(pieces, moveData, endSquare);

    moveHistory.push_back(moveData);
  }

  void undoMove(Checkers::Pieces &pieces) {
    using namespace Checkers;

    const MoveData moveData = moveHistory.back();
    const std::vector<PieceName> names = {moveData.name1, moveData.name2,
                                          moveData.name3};
    const std::vector<Bitboard> boards = {moveData.board1, moveData.board2,
                                          moveData.board3};
    moveHistory.pop_back();

    for (int i = 0; i < 3; ++i) {
      switch (names[i]) {
      case (PieceName::WhitePieces):
        pieces.whitePieces = boards[i];
        break;
      case (PieceName::BlackPieces):
        pieces.blackPieces = boards[i];
        break;
      case (PieceName::WhiteKings):
        pieces.whiteKings = boards[i];
        break;
      case (PieceName::BlackKings):
        pieces.blackKings = boards[i];
        break;
      }
    }
  }

  std::vector<Checkers::MoveCords> possibleMoves(const Checkers::Pieces &pieces,
                                                 const bool isWhiteTurn) const {
    using namespace Checkers;

    const int LEFT_FORWARD = 9;
    const int RIGHT_FORWARD = 7;
    const int LEFT_BACK = -9;
    const int RIGHT_BACK = -7;

    const Bitboard occupied = pieces.whitePieces | pieces.whiteKings |
                              pieces.blackPieces | pieces.blackKings;
    std::vector<MoveCords> allMoves;

    if (isWhiteTurn) {
      // Left Forward Capture (Pawns and Kings)
      addMoves(allMoves, pieces.whitePieces | pieces.whiteKings, occupied,
               LEFT_FORWARD, 1, 1, ~COL_4, ~ROW_1);
      // Right Forward Capture (Pawns and Kings)
      addMoves(allMoves, pieces.whitePieces | pieces.whiteKings, occupied,
               RIGHT_FORWARD, 1, 1, ~COL_1, ~ROW_1);
      // Left Back Capture (Kings)
      addMoves(allMoves, pieces.whiteKings, occupied, LEFT_BACK, 1, -1, ~COL_4,
               ~ROW_8);
      // Right Back Capture (Kings)
      addMoves(allMoves, pieces.whiteKings, occupied, RIGHT_BACK, -1, -1,
               ~COL_1, ~ROW_8);
    } else {
      // Left Forward Capture (Kings)
      addMoves(allMoves, pieces.blackPieces | pieces.blackKings, occupied,
               LEFT_FORWARD, 1, 1, ~COL_4, ~ROW_1);
      // Right Forward Capture (Kings)
      addMoves(allMoves, pieces.blackPieces | pieces.blackKings, occupied,
               RIGHT_FORWARD, 1, 1, ~COL_1, ~ROW_1);
      // Left Back Capture (Pawns and Kings)
      addMoves(allMoves, pieces.blackKings, occupied, LEFT_BACK, 1, -1, ~COL_4,
               ~ROW_8);
      // Right Back Capture (Pawns and Kings)
      addMoves(allMoves, pieces.blackKings, occupied, RIGHT_BACK, -1, -1,
               ~COL_1, ~ROW_8);
    }

    return allMoves;
  }
};

#endif