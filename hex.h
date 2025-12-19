#pragma once

#include <vector>

struct Move {
	int row, col;
	Move(int r = -1, int c = -1) : row(r), col(c) {};
};

enum Player { EMPTY = 0, PLAYER_X = 'X', PLAYER_O = 'O' };

class HexGame {
private:
    std::vector<std::vector<char>> board;
    int size;
    bool gameOver;

public:
    HexGame(int n) : size(n), gameOver(false) {
        board.resize(size, std::vector<char>(size, ' '));
    }

    // Методы для работы с доской
    bool makeMove(int row, int col, char player);
    bool isValidMove(int row, int col) const;
    std::vector<Move> getValidMoves() const;
    void printBoard() const;

    // Игровая логика для Hex
    bool checkWin(char player) const;
    bool hasPath(char player, int row, int col,
        std::vector<std::vector<bool>>& visited) const;

    // Геттеры
    int getSize() const { return size; }

    char getCell(int row, int col) const {
        /*if (isValidMove(row, col)) */return board[row][col];
        /*return '?';*/
    }

    // Для AI
    std::vector<std::pair<int, int>> getNeighbors(int row, int col) const;
};

int evaluate(const HexGame& game, char player);

int alphabeta(HexGame& game, int depth, int alpha, int beta, bool isMaximizing, char maximizingPlayer);

Move findBestMove(HexGame& game, int depth, char player);
