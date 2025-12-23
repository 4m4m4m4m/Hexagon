#include "hex.h"
#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <algorithm>
#include <iomanip>

using namespace std;

bool HexGame::isValidMove(int row, int col) const {
	return row >= 0 && row < size && col >= 0 && col < size;
}

bool HexGame::makeMove(int row, int col, char player) {
    if (!isValidMove(row, col) /*|| board[row][col] != ' '*/) {
        return false;
    }
    board[row][col] = player;
    return true;
}

vector<Move> HexGame::getValidMoves() const {
    vector<Move> moves;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == ' ') {
                moves.push_back(Move(i, j));
            }
        }
    }
    return moves;
}

vector<pair<int, int>> HexGame::getNeighbors(int row, int col) const {
    vector<pair<int, int>> neighbors;
    int rowParity = 0;

    // 6 направлений для шестиугольника
    vector<pair<int, int>> dirs = {
        //{-1, -1 - rowParity},  // Верхний левый
        {-1, 1},  // Верхний правый
        {0, -1},              // Левый
        {0, 1},               // Правый
        {-1, 0},               // Верхний
        {1, 0},               // Нижний
        {1, 0},   // Нижний левый
        //{1, 1 - rowParity}    // Нижний правый
    };

    for (auto& dir : dirs) {
        int newRow = row + dir.first;
        int newCol = col + dir.second;

        if (isValidMove(newRow, newCol)) {
            neighbors.push_back({ newRow, newCol });
        }
    }

    return neighbors;
}

bool HexGame::hasPath(char player, int row, int col,
    vector<vector<bool>>& visited) const {
    visited[row][col] = true;

    // Получаем всех соседей
    auto neighbors = getNeighbors(row, col);

    for (auto& neighbor : neighbors) {
        int nr = neighbor.first;
        int nc = neighbor.second;

        // Если сосед валиден, не посещен и принадлежит тому же игроку
        if (isValidMove(nr, nc) && !visited[nr][nc] && board[nr][nc] == player) {
            // Рекурсивно проверяем путь от соседа
            if (hasPath(player, nr, nc, visited)) {
                return true;
            }
        }
    }

    // Для игрока X: если дошли до правого края, возвращаем true
    if (player == 'X' && col == size - 1) {
        return true;
    }

    // Для игрока O: если дошли до нижнего края, возвращаем true
    if (player == 'O' && row == size - 1) {
        return true;
    }

    return false;
}

bool HexGame::checkWin(char player) const {
    vector<vector<bool>> visited(size, vector<bool>(size, false));

    if (player == 'X') {
        // Проверяем от всех ячеек в первом столбце
        for (int r = 0; r < size; r++) {
            if (board[r][0] == 'X' && !visited[r][0]) {
                if (hasPath('X', r, 0, visited)) {
                    return true;
                }
            }
        }
    }
    else if (player == 'O') {
        // Проверяем от всех ячеек в первой строке
        for (int c = 0; c < size; c++) {
            if (board[0][c] == 'O' && !visited[0][c]) {
                if (hasPath('O', 0, c, visited)) {
                    return true;
                }
            }
        }
    }

    return false;
}

void HexGame::printBoard() const {
    //std::cout << "\n   ";
    // Вывод заголовков столбцов
    /*for (int col = 0; col < size; col++) {
        std::cout << std::setw(2) <<  col+1 << " ";
    }
    std::cout << "\n";*/

    for (int row = 0; row < 21; row++) {
        if (row < 11) {
            // Отступ для создания шестиугольной формы
            std::cout << std::string(11 - row, ' ');

            // Вывод номера строки
            std::cout << std::setw(2) << row  << " ";

            int rowcopy = row;
            int col = 0;
            while (rowcopy >= 0) {
                char symbol = board[rowcopy][col];
                std::cout << " " << symbol ;
                col++;
                rowcopy--;
            }
            std::cout << " " << row << std::endl;
        }
        else {
            // Отступ для создания шестиугольной формы
            std::cout << std::string(row - 11, ' ');

            int rowcopy = 10;
            int col = row - 10;
            while (rowcopy >= row - 10) {
                char symbol = board[rowcopy][col];
                std::cout << " " << symbol;
                col++;
                rowcopy--;
            }
            std::cout << " " << std::endl;
        }
    }
}

int evaluate(const HexGame& game, char player) {
    int score = 0;
    int size = game.getSize();

    // Проверка победы
    if (game.checkWin(player)) {
        return 10000;
    }
    if (game.checkWin((player == 'X') ? 'O' : 'X')) {
        return -10000;
    }

    // Эвристика: центральные ячейки важнее
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (game.getCell(i, j) == player) {
                // Ближе к центру - больше очков
                int distanceFromCenter = abs(size / 2 - i) + abs(size / 2 - j);
                score += (size - distanceFromCenter) * 10;

                // Ячейки на краю важны для победы
                if (player == 'X' && j == 0) score += 50;
                if (player == 'X' && j == size - 1) score += 50;
                if (player == 'O' && i == 0) score += 50;
                if (player == 'O' && i == size - 1) score += 50;
            }
            else if (game.getCell(i, j) != ' ') {
                // Противник
                int distanceFromCenter = abs(size / 2 - i) + abs(size / 2 - j);
                score -= (size - distanceFromCenter) * 10;

                if (player != 'X' && j == 0) score -= 50;
                if (player != 'X' && j == size - 1) score -= 50;
                if (player != 'O' && i == 0) score -= 50;
                if (player != 'O' && i == size - 1) score -= 50;
            }
        }
    }

    return score;
}

int alphabeta(HexGame& game, int depth, int alpha, int beta,
    bool isMaximizing, char maximizingPlayer) {
    HexGame gameCopy = game;
    if (depth == 0 || gameCopy.checkWin('X') || gameCopy.checkWin('O')) {
        return evaluate(gameCopy, maximizingPlayer);
    }

    auto validMoves = gameCopy.getValidMoves();

    if (isMaximizing) {
        int maxEval = INT_MIN;

        for (const auto& move : validMoves) {
            // Сохраняем старое состояние
            char oldCellState = gameCopy.getCell(move.row, move.col);

            // Делаем ход
            gameCopy.makeMove(move.row, move.col, maximizingPlayer);

            // Рекурсивный вызов
            int eval = alphabeta(gameCopy, depth - 1, alpha, beta, false, maximizingPlayer);

            // Отменяем ход
            gameCopy.makeMove(move.row, move.col, oldCellState);

            maxEval = max(maxEval, eval);
            alpha = max(alpha, eval);

            if (beta <= alpha) {
                break; // Альфа-бета отсечение
            }
        }
        return maxEval;
    }
    else {
        int minEval = INT_MAX;
        char minimizingPlayer = 'X';

        for (const auto& move : validMoves) {
            // Сохраняем старое состояние
            char oldCellState = gameCopy.getCell(move.row, move.col);

            // Делаем ход
            gameCopy.makeMove(move.row, move.col, minimizingPlayer);

            // Рекурсивный вызов
            int eval = alphabeta(gameCopy, depth - 1, alpha, beta, true, maximizingPlayer);

            // Отменяем ход
            gameCopy.makeMove(move.row, move.col, oldCellState);

            minEval = min(minEval, eval);
            beta = min(beta, eval);

            if (beta <= alpha) {
                break; // Альфа-бета отсечение
            }
        }
        return minEval;
    }
}

// Найти лучший ход для Hex
Move findBestMove(HexGame& game, int depth, char player) {
    int bestValue = INT_MIN;
    Move bestMove(-1, -1);

    auto validMoves = game.getValidMoves();

    for (const auto& move : validMoves) {
        // Сохраняем старое состояние
        char oldCell = game.getCell(move.row, move.col);

        // Пробуем ход
        game.makeMove(move.row, move.col, player);

        // Вычисляем оценку
        int moveValue = alphabeta(game, depth - 1, INT_MIN, INT_MAX, false, player);

        // Отменяем ход
        game.makeMove(move.row, move.col, oldCell);

        // Если этот ход лучше
        if (moveValue > bestValue) {
            bestMove = move;
            bestValue = moveValue;
        }
    }

    return bestMove;
}