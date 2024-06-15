
#include "global.hpp"

void printBoard(Board b) {
    cout << endl << b << endl;
}

vector<pair<int, int> > get_free_cells(Board board) {
    int n = board.rows();
    vector<pair<int, int> > free_cells;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board(i, j) == 0) {
                free_cells.push_back(make_pair(i, j));
            }
        }
    }
    return free_cells;
}

void insertRandomTile(Board& board) {

    vector<pair<int, int> > free_cells = get_free_cells(board);

    // select random cell for new tile
    uniform_int_distribution<int> dis(0, free_cells.size()-1);
    int freeIdx = dis(rng);
    pair<int, int> pos = free_cells[freeIdx];

    // select random tile 
    // uniform_int_distribution<int> dis(0, 9);

    uniform_real_distribution<float> dis1(0, 1);
    float randTile = dis1(rng) < 0.9 ? 2 : 4; // choose random tile (2 or 4)
    cout << dis1(rng) << endl;
    board(pos.first, pos.second) = randTile;
}

// function for board initialization
Board initializeBoard(int dim) {
    Board board(dim, dim);
    insertRandomTile(board);
    insertRandomTile(board);
    return board;
}

bool boardChanged(Board old_b, Board curr_b) {
    return old_b != curr_b;
}

Board transformBoard(Board board, Move dir, bool back = false) {
    if (dir == Left) return board;
    else if (dir == Right) {
        return board.rowwise().reverse();
    }
    else if (dir == Up) {
        return board.transpose();
    }
    if (back) return board.rowwise().reverse().transpose();
    return board.colwise().reverse().transpose(); // Down
}

// move logic
Board makeMove(Board board, Move dir, int& score, int& merged_tiles) {

    // transform matrix
    Board b = transformBoard(board, dir);;
    int n = b.rows();

    for (int i = 0; i < n; i++) {

        for (int j = 1; j < n; j++) {
            
            int tile = b(i, j);
            if (tile == 0) continue;

            for (int k = 1; k < j + 1; k++) {

                int off = j - k;
                if (b(i, off) == 0) { // cell is empty
                    b(i, off) = tile;
                    b(i, off + 1) = 0;
                }
                else if (b(i, off) == tile){ // two identical tiles
                    b(i, off) *= 2;
                    b(i, off + 1) = 0;
                    break;
                }
                else break; // tile can't move
            }
        }
    }
    return transformBoard(b, dir, true);
}
