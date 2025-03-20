#ifndef INPUT_H
#define INPUT_H
#include "pieces.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

namespace input {

Checkers::MoveCords
getPlayerMove(const std::vector<Checkers::MoveCords> possibleMoves) {
  std::string moveString;
  Checkers::MoveCords move;
  std::vector<Checkers::MoveCords>::const_iterator it;

  while (true) { // loop until valid input
    std::cout << "Choose your move (xyxy) ";
    std::cin >> moveString;

    if (moveString.length() != 4) {
      std::cout << "Invalid input. Please enter four digits." << std::endl;
      continue;
    }

    bool validDigits = true;
    for (char c : moveString) {
      if (!isdigit(c)) {
        validDigits = false;
        break;
      }
    }

    if (!validDigits) {
      std::cout << "Invalid input. Please enter only digits." << std::endl;
      continue;
    }

    move.x1 = moveString[0] - '0';
    move.y1 = moveString[1] - '0';
    move.x2 = moveString[2] - '0';
    move.y2 = moveString[3] - '0';

    it = std::find(possibleMoves.begin(), possibleMoves.end(), move);

    if (it != possibleMoves.end()) {
      return move;
    } else {
      std::cout << "Invalid input. Not in the list of possible moves."
                << std::endl;
    }
  }
}

} // namespace input

#endif