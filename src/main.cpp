#include "../include/board.h"
#include "../include/moves.h"

int main() {
  Moves moves;
  Board board(moves);
  board.startGame();

  return 0;
}