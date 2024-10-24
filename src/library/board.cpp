#include "board.hpp"

void printBoard(Board b) {
    cout << "\n" << b << "\n";
}

bool boardChanged(Board old_b, Board curr_b) {
    return old_b != curr_b;
}

bool isGameEnd(Board board) {

    int n = board.rows();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {

            if (board(i, j) == 0) return false;
            if (j < n-1) {
                if (board(i, j) == board(i, j+1)) return false;
            }
            if (i < n-1) {
                if (board(i, j) == board(i+1, j)) return false;
            }
            // if (board(i, j) == board(i, j+1) || b_T(i, j) == b_T(i, j+1)) {
            //     return false;
            // }
        }
    }
    return true;
}

Cells getEmptyCells(Board board) {

    int n = board.rows();
    Cells empty_cells;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board(i, j) == 0) {
                empty_cells.push_back(make_pair(i, j));
            }
        }
    }
    return empty_cells;
}

pair<vector<Move>, vector<Board> > getLegalMoves(Board board) {
    vector<Move> legal_moves;
    vector<Board> new_states;
    for (int i = 0; i < 4; i++) {
        Move move = static_cast<Move>(i);
        Board new_state = executeMove(board, move).board;
        if (boardChanged(board, new_state)) {
            legal_moves.push_back(move);
            new_states.push_back(new_state);
        }
    }
    return make_pair(legal_moves, new_states);
}

// function for board initialization
Board initializeBoard(int dim) {
    assert(dim >= 3 && dim <= 6);
    Board board(dim, dim);
    return insertRandomTile(insertRandomTile(board));;
}

Board insertRandomTile(Board board) {

    Cells empty_cells = getEmptyCells(board);
    // select random cell for new tile
    uniform_int_distribution<int> dis(0, empty_cells.size()-1);
    int emptyIdx = dis(rng);
    pair<int, int> pos = empty_cells[emptyIdx];
    // select random tile 
    uniform_real_distribution<float> dis1(0, 1);
    int randTile = dis1(rng) < 0.9 ? 2 : 4; // choose random tile (2 or 4)
    board(pos.first, pos.second) = randTile;

    return board;
}

Board transformBoard(Board board, Move dir, bool back = false) {
    if (dir == Left) return board;
    else if (dir == Right) {
        return board.rowwise().reverse();
    }
    else if (dir == Up) {
        return board.transpose();
    }
    // Down
    if (back) return board.rowwise().reverse().transpose();
    return board.colwise().reverse().transpose();
}

// move logic
BoardData executeMove(Board board, Move dir) {

    // transform matrix
    Board b = transformBoard(board, dir);;
    int n = b.rows();

    int score = 0;
    int merges = 0;

    for (int i = 0; i < n; i++) {

        Eigen::VectorXi merge_buffer(n);
        for (int j = 1; j < n; j++) {
            
            int tile = b(i, j);
            if (tile == 0) continue;

            for (int k = 1; k < j + 1; k++) {

                int off = j - k;
                if (b(i, off) == 0) { // cell is empty
                    b(i, off) = tile;
                    b(i, off + 1) = 0;
                }
                else if (b(i, off) == tile && merge_buffer(off) != 1){ // two identical tiles
                    merge_buffer(off) = 1;
                    b(i, off) *= 2;
                    b(i, off + 1) = 0;

                    score += b(i, off);
                    merges += 1;
                    break;
                }
                else break; // tile can't move
            }
        }
    }
    BoardData bd(transformBoard(b, dir, true), score, merges);
    return bd;
}
