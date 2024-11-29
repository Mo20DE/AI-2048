#pragma once

#include "globals.hpp"


struct BoardData {
    Board board;
    int score;
    int merges;
    BoardData(Board b, int s, int m) 
    : board(b), score(s), merges(m) {}
};

void printBoard(Board b);
bool boardChanged(Board old_b, Board curr_b);
bool isGameEnd(Board board);
Cells getEmptyCells(Board board);
pair<vector<Move>, vector<Board> > getLegalMoves(Board board);

Board initializeBoard(int dim);
Board insertRandomTile(Board board);
Board transformBoard(Board board, Move dir, bool back);
BoardData executeMove(Board board, Move dir);
