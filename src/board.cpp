#include "board.h"

Board::Board(const Moves &moves)
    : pieces{0xFFF, 0xFFF00000, 0, 0}, moves(moves), bestMove(),
      isWhiteTurn(false) {}

void Board::displayBoard() {
  for (int i = 0; i < 32; i++) {
    if (i % 4 == 0)
      std::cout << "\n";
    if ((pieces.whitePieces >> i) & 1) {
      std::cout << " w ";
    } else if ((pieces.blackPieces >> i) & 1) {
      std::cout << " b ";
    } else if ((pieces.whiteKings >> i) & 1) {
      std::cout << " W ";
    } else if ((pieces.blackKings >> i) & 1) {
      std::cout << " B ";
    } else {
      std::cout << "   ";
    }
  }
}

float Board::minimax(int depth, float alpha, float beta, bool isRootNode) {
  using Checkers::MoveCords;

  if (depth == 0 || gameOver()) {
    return evaluate(depth);
  }

  const std::vector<MoveCords> myMoves =
      moves.possibleMoves(pieces, isWhiteTurn);

  if (isWhiteTurn) {
    float maxScore = std::numeric_limits<float>::lowest();
    for (const MoveCords &move : myMoves) {
      moves.doMove(move, pieces, isWhiteTurn);
      isWhiteTurn = !isWhiteTurn;
      float score = minimax(depth - 1, alpha, beta, false);
      isWhiteTurn = !isWhiteTurn;
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
    float minScore = std::numeric_limits<float>::max();
    for (const MoveCords &move : myMoves) {
      moves.doMove(move, pieces, isWhiteTurn);
      isWhiteTurn = !isWhiteTurn;
      float score = minimax(depth - 1, alpha, beta, false);
      isWhiteTurn = !isWhiteTurn;
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

float Board::evaluate(const int depth) {
  if (gameOver()) {
    return (1000 + depth) * (isWhiteTurn ? -1 : 1);
  }
  return materialScore();
}

bool Board::gameOver() {
  return (isWhiteTurn && noWhiteMoves()) || (!isWhiteTurn && noBlackMoves());
}

bool Board::noWhiteMoves() {
  return moves.possibleMoves(pieces, isWhiteTurn).size() == 0;
}

bool Board::noBlackMoves() {
  return moves.possibleMoves(pieces, isWhiteTurn).size() == 0;
}

int Board::materialScore() {
  return __builtin_popcountll(pieces.whitePieces) +
         2 * __builtin_popcountll(pieces.whiteKings) -
         __builtin_popcountll(pieces.blackPieces) -
         2 * __builtin_popcountll(pieces.blackKings);
}

void Board::startGame() {
  displayBoard();
  while (!gameOver()) {
    const std::vector<Checkers::MoveCords> possibleMoves =
        moves.possibleMoves(pieces, isWhiteTurn);
    const Checkers::MoveCords move = input::getPlayerMove(possibleMoves);
    moves.doMove(move, pieces, isWhiteTurn);
    displayBoard();
  }

  const std::string winner = isWhiteTurn ? "Black" : "White";
  std::cout << winner << " Wins!" << std::endl;
}