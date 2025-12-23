#include "hex.h"
#include <iostream>

int main() {
    HexGame game(11); // Доска 11x11
    int size = 11;

    // Человек vs AI
    char human = 'X';
    char ai = 'O';

    while (true) {
        // Ход человека
        game.printBoard();
        int row, col;
        std::cout << "Your move (row col): ";
        std::cin >> row >> col;

        if (row >= 0 && row < size && col >= 0 && col < size) {
            game.makeMove(row, col, human);
        }
        else {
            std::cout << "Imposible move!" << std::endl;
            continue;
        }

        if (game.checkWin(human)) {
            std::cout << "You won!" << std::endl;
            break;
        }

        // Ход AI
        std::cout << "Move AI..." << std::endl;
        Move aiMove = findBestMove(game, 3, ai);

        if (aiMove.row != -1) {
            game.makeMove(aiMove.row, aiMove.col, ai);
            std::cout << "AI moved: " << aiMove.row << " " << aiMove.col << std::endl;
        }

        if (game.checkWin(ai)) {
            game.printBoard();
            std::cout << "AI win!" << std::endl;
            break;
        }
    }

    return 0;
}