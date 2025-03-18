#ifndef MOVES_H
#define MOVES_H
#include "board.h"
#include "pieces.h"
#include <iostream>
#include <string>
#include <vector>

typedef uint64_t Bitboard;

class Moves {
private:
  // save row masks for moves
  const Bitboard RANK_1 = -72057594037927936L;
  const Bitboard RANK_4 = 1095216660480L;
  const Bitboard RANK_5 = 4278190080L;
  const Bitboard RANK_8 = 255L;

  // save column masks for moves
  const Bitboard FILE_A = 72340172838076673L;
  const Bitboard FILE_AB = 217020518514230019L;
  const Bitboard FILE_H = -9187201950435737472L;
  const Bitboard FILE_GH = -4557430888798830400L;

  enum class PieceType {
    WhitePieces,
    BlackPieces,
    WhiteKings,
    BlackKings,
    None
  };
  struct SelectedBoards {
    Bitboard startSquare;
    Bitboard endSquare;
  };
  struct BoardState {
    PieceType name;
    Bitboard board;
  };
  struct MoveData {
    BoardState boardState1;
    BoardState boardState2;
    BoardState boardState3;
  };
  std::vector<MoveData> moveHistory;

public:
  Moves(){};

  BoardState &getSelectedBoardState(Pieces &pieces, const Bitboard &board) {
    if (pieces.whitePieces & board) {
      return {PieceType::WhitePieces, pieces.whitePieces};
    } else if (pieces.blackPieces & board) {
      return {PieceType::BlackPieces, pieces.blackPieces};
    } else if (pieces.whiteKings & board) {
      return {PieceType::WhiteKings, pieces.whiteKings};
    } else if (pieces.blackKings & board) {
      return {PieceType::BlackKings, pieces.blackKings};
    } else {
      return {PieceType::None, 0};
    }
  }

  SelectedBoards extractSelectedBoards(const std::string &move) {
    const int x1 = move[1] - '0';
    const int y1 = move[2] - '0';
    const int x2 = move[3] - '0';
    const int y2 = move[4] - '0';
    const Bitboard startSquare = 1L << (x1 + (7 - y1) * 8);
    const Bitboard endSquare = 1L << (x2 + (7 - y2) * 8);
    return {startSquare, endSquare};
  }

  void tryToDeleteCapture(Pieces &pieces, MoveData &moveData,
                          const Bitboard captureSquare) {
    BoardState &boardState = getSelectedBoardState(pieces, captureSquare);
    moveData.boardState2 = boardState;
    boardState.board ^= captureSquare;
  }

  void tryToMove(Pieces &pieces, MoveData &moveData, const Bitboard startSquare,
                 const Bitboard endSquare) {
    BoardState &boardState = getSelectedBoardState(pieces, startSquare);
    moveData.boardState1 = boardState;
    boardState.board ^= startSquare | endSquare;
  }

  void tryToPromote(Pieces &pieces, MoveData &moveData,
                    const Bitboard endSquare) {
    const Bitboard whitePromoteMask = 0x55;
    const Bitboard blackPromoteMask = 0xAA00000000000000;
    if (pieces.whitePieces & whitePromoteMask) {
      BoardState boardState = {PieceType::WhitePieces, pieces.whitePieces};
      pieces.whitePieces ^= endSquare;
      pieces.whiteKings ^= endSquare;
    } else if (pieces.blackPieces & blackPromoteMask) {
      BoardState boardState = {PieceType::BlackPieces, pieces.blackPieces};
      pieces.blackPieces ^= endSquare;
      pieces.blackKings ^= endSquare;
    }
  }

  void doMove(const std::string move, Pieces &pieces, const bool isWhiteTurn) {
    MoveData moveData;
    const SelectedBoards selectedBoards = extractSelectedBoards(move);
    const Bitboard captureSquare = isWhiteTurn
                                       ? selectedBoards.startSquare << 8
                                       : selectedBoards.startSquare >> 8;

    tryToDeleteCapture(pieces, moveData, captureSquare);
    tryToMove(pieces, moveData, selectedBoards.startSquare,
              selectedBoards.endSquare);
    tryToPromote(pieces, moveData, selectedBoards.endSquare);

    moveHistory.push_back(moveData);
  }

  void undoMove(Pieces &pieces) {
    // const MoveData moveData = moveHistory.back();
    // moveHistory.pop_back();

    // for (int i = 0; i < 3; ++i) {
    //   switch (moveData[i].name) {
    //   case (PieceType::WhitePieces):
    //     pieces.whitePieces = moveData[i].board;
    //     break;
    //   case (PieceType::BlackPieces):
    //     pieces.blackPieces = moveData[i].board;
    //     break;
    //   case (PieceType::WhiteKings):
    //     pieces.whiteKings = moveData[i].board;
    //     break;
    //   case (PieceType::BlackKings):
    //     pieces.blackKings = moveData[i].board;
    //     break;
    //   }
    // }
  }

  std::string generateMoves(Pieces &pieces, Bitboard pieceBoard,
                            Bitboard occupied, int shift, int rowOffset,
                            int colOffset, Bitboard fileMask,
                            Bitboard rankMask) {
    std::string moves = "";
    Bitboard moveBoard = pieceBoard;
    moveBoard = (shift > 0 ? moveBoard >> shift : moveBoard << -shift) &
                ~occupied & fileMask & rankMask;

    while (moveBoard) {
      int index = __builtin_ctzll(moveBoard);
      int row = index % 8;
      int col = index / 8;
      moves += " " + std::to_string(row + rowOffset) +
               std::to_string(7 - (col + colOffset)) + std::to_string(row) +
               std::to_string(7 - col);
      moveBoard &= moveBoard - 1;
    }
    return moves;
  }

  std::string possibleMovesWhite(Pieces &pieces) {
    const Bitboard occupiedWhite = pieces.whitePieces | pieces.whiteKings;
    std::string allMoves = "";

    // Left Forward Capture (Pawns and Kings)
    allMoves += generateMoves(pieces, pieces.whitePieces | pieces.whiteKings,
                              occupiedWhite, 9, 1, 1, ~FILE_H, ~RANK_8);
    // Right Forward Capture (Pawns and Kings)
    allMoves += generateMoves(pieces, pieces.whitePieces | pieces.whiteKings,
                              occupiedWhite, 7, 1, 1, ~FILE_A, ~RANK_8);
    // Left Back Capture (Kings)
    allMoves += generateMoves(pieces, pieces.whiteKings, occupiedWhite, -9, 1,
                              -1, ~FILE_H, ~RANK_1);
    // Right Back Capture (Kings)
    allMoves += generateMoves(pieces, pieces.whiteKings, occupiedWhite, -7, -1,
                              -1, ~FILE_A, ~RANK_1);

    return allMoves;
  }

  std::string possibleMovesBlack(Pieces &pieces) {
    const Bitboard occupiedBlack = pieces.blackPieces | pieces.blackKings;
    std::string allMoves = "";

    // Left Forward Capture (Kings)
    allMoves += generateMoves(pieces, pieces.blackPieces | pieces.blackKings,
                              occupiedBlack, 9, 1, 1, ~FILE_H, ~RANK_8);
    // Right Forward Capture (Kings)
    allMoves += generateMoves(pieces, pieces.blackPieces | pieces.blackKings,
                              occupiedBlack, 7, 1, 1, ~FILE_A, ~RANK_8);
    // Left Back Capture (Pawns and Kings)
    allMoves += generateMoves(pieces, pieces.blackKings, occupiedBlack, -9, 1,
                              -1, ~FILE_H, ~RANK_1);
    // Right Back Capture (Pawns and Kings)
    allMoves += generateMoves(pieces, pieces.blackKings, occupiedBlack, -7, -1,
                              -1, ~FILE_A, ~RANK_1);

    return allMoves;
  }
};

#endif