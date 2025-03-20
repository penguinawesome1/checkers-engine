#ifndef BOARD_H
#define BOARD_H

#include "input.h"
#include "moves.h"
#include "pieces.h"
#include <iostream>
#include <limits>
#include <vector>

class Board {
private:
  Checkers::Pieces pieces;
  Moves moves;
  bool isWhiteTurn;
  Checkers::MoveCords bestMove;

  void displayBoard();
  float minimax(int depth, float alpha, float beta, bool isRootNode);
  float evaluate(const int depth);
  bool gameOver();
  bool noWhiteMoves();
  bool noBlackMoves();
  int materialScore();

public:
  Board(const Moves &moves);
  void startGame();
};

#endif