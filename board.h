#ifndef BOARD_H
#define BOARD_H
#include "moves.h"
#include "pieces.h"
#include <iostream>
#include <limits>

class Board {
private:
  Pieces pieces;
  Moves moves;
  bool isWhiteTurn;
  std::string bestMove;

  void displayBoard() {
    std::cout << "\n     0    1    2    3    4    5    6    7\n"
              << "  |----|----|----|----|----|----|----|----|\n";
    for (int i = 0; i < 64; ++i) {
      if (i % 8 == 0) {
        std::cout << (7 - i / 8) << " ";
      }
      if (pieces.whitePieces >> i & 1) {
        std::cout << "| w  ";
      } else if (pieces.blackPieces >> i & 1) {
        std::cout << "| b  ";
      } else if (pieces.whiteKings >> i & 1) {
        std::cout << "| W  ";
      } else if (pieces.blackKings >> i & 1) {
        std::cout << "| B  ";
      } else {
        std::cout << "|    ";
      }
      if (i % 8 == 7) {
        std::cout << "| " << (7 - i / 8)
                  << "\n  |----|----|----|----|----|----|----|----|\n";
      }
    }
    std::cout << "     0    1    2    3    4    5    6    7" << std::endl;
  }

  std::string getPlayerMove() {
    const std::string myMoves = isWhiteTurn ? moves.possibleMovesWhite(pieces)
                                            : moves.possibleMovesBlack(pieces);
    std::cout << myMoves << std::endl;
    std::string move;
    do {
      std::cout << "Choose your move (xyxy) ";
      std::cin >> move;
    } while (move.length() != 4 || myMoves.find(move) == std::string::npos);

    return " " + move;
  }

  float minimax(int depth, float alpha, float beta, bool isRootNode) {
    if (depth == 0 || gameOver()) {
      return evaluate(depth);
    }

    if (isWhiteTurn) {
      const std::string myMoves = moves.possibleMovesWhite(pieces);
      float maxScore = std::numeric_limits<float>::lowest();
      for (size_t i = 0; i < myMoves.length(); i += 5) {
        const std::string move = myMoves.substr(i, 5);
        moves.doMove(move, pieces);
        bool tempTurn = isWhiteTurn;
        isWhiteTurn = !isWhiteTurn;
        float score = minimax(depth - 1, alpha, beta, false);
        isWhiteTurn = tempTurn;
        moves.undoMove(pieces);

        if (isRootNode && score > maxScore) {
          bestMove = move;
        }
        maxScore = std::max(score, maxScore);
        alpha = std::max(alpha, maxScore);
        if (beta <= alpha) {
          return maxScore;
        }
      }
      return maxScore;
    } else {
      const std::string myMoves = moves.possibleMovesBlack(pieces);
      float minScore = std::numeric_limits<float>::max();
      for (size_t i = 0; i < myMoves.length(); i += 5) {
        const std::string move = myMoves.substr(i, 5);
        moves.doMove(move, pieces);
        bool tempTurn = isWhiteTurn;
        isWhiteTurn = !isWhiteTurn;
        float score = minimax(depth - 1, alpha, beta, false);
        isWhiteTurn = tempTurn;
        moves.undoMove(pieces);

        if (isRootNode && score < minScore) {
          bestMove = move;
        }
        minScore = std::min(score, minScore);
        beta = std::min(beta, minScore);
        if (beta <= alpha) {
          return minScore;
        }
      }
      return minScore;
    }
  }

  float evaluate(const int depth) {
    if (gameOver()) {
      return (1000 + depth) * (isWhiteTurn ? -1 : 1);
    }
    return materialScore();
  }

  bool gameOver() {
    return (isWhiteTurn && noWhiteMoves()) || (!isWhiteTurn && noBlackMoves());
  }

  bool noWhiteMoves() { return moves.possibleMovesWhite(pieces).size() == 0; }

  bool noBlackMoves() { return moves.possibleMovesBlack(pieces).size() == 0; }

  int materialScore() {
    return __builtin_popcountll(pieces.whitePieces) +
           2 * __builtin_popcountll(pieces.whiteKings) -
           __builtin_popcountll(pieces.blackPieces) -
           2 * __builtin_popcountll(pieces.blackKings);
  }

public:
  Board(const Moves &moves)
      : pieces{0x55AA550000000000, 0xAA55AA, 0, 0}, moves(moves), bestMove(""),
        isWhiteTurn(true) {}

  void startGame() {
    while (!gameOver()) {
      const std::string move = getPlayerMove();
      moves.doMove(move, pieces);
      displayBoard();
    }

    const std::string winner = isWhiteTurn ? "Black" : "White";
    std::cout << winner << " Wins!" << std::endl;
  }
};

#endif