#include "board.hpp"
#include "expectimax.hpp"
#include "heuristics.hpp"


ExpectiMax::ExpectiMax(int board_dim) : b_dim(board_dim), tt(board_dim) {}

Move ExpectiMax::searchBestMove(Board board, int depth) {

    assert(depth > 0);
    // tt.resetTranspositionTable();
    auto data = getLegalMoves(board);
    if (data.first.size() == 1) {
        return data.first[0];
    }

    float best_utility = -INFINITY;
    int best_move_idx = 0;

    Eigen::VectorXi scores(4);

    // int num = getNumEmptyCells(board);
    // depth = num > 3 ? depth : num > 1 ? 4 : 5; // determine depth dynamically

    // this loop represents the max node choosing the best move from root node
    for (int i = 0; i < data.first.size(); i++) {

        Board new_state = data.second[i];
        float utility = chance_node(new_state, depth);
        // scores(i) = utility;
        if (utility > best_utility) {
            best_utility = utility;
            best_move_idx = i;
        }
    }

    // cout << scores << endl;
    return data.first[best_move_idx];
}

float ExpectiMax::chance_node(Board state, int depth) const {

    Cells empty_cells = getEmptyCells(state);
    if (depth <= 0) return evaluateState(state);

    uint64_t hash = tt.computeHash(state);
    auto entry = tt.lookup(hash);
    if (entry.has_value()) {
        // we already computed utility for same board, but deeper
        // cout << "FIRST" << endl;
        if (entry->depth <= depth) { // depth <-> currdepth
            // cout << "SECOND" << endl;
            return entry->utility;
        }
    }

    float total_utility = 0.0;
    // TODO: algorithm to choose less empty cells (reduce branching factor)
    for (int i = 0; i < empty_cells.size(); i++) {

        pair<int, int> cell = empty_cells[i];
        Board new_state = state;

        new_state(cell.first, cell.second) = 2;
        total_utility += 0.9 * max_node(new_state, depth - 1);
        new_state(cell.first, cell.second) = 4;
        total_utility += 0.1 * max_node(new_state, depth - 1);
        new_state(cell.first, cell.second) = 0;
    }

    if (entry.has_value()) {
        // if (entry->depth > depth) {
        entry->utility = total_utility;
        entry->depth = depth;
        // cout << "THIRD" << endl;
        // }
    }
    else {
        TTEntryExpMax entry = {total_utility, depth};
        tt.saveEntry(hash, entry);
        // cout << "FOURTH" << endl;
    }

    return total_utility/empty_cells.size();
}

float ExpectiMax::max_node(Board state, int depth) const {

    // if (depth <= 0) return evaluateState(state);

    float best_score = -INFINITY;
    auto data = getLegalMoves(state);
    for (int i = 0; i < data.first.size(); i++) {
        Board new_state = data.second[i];
        float score = chance_node(new_state, depth - 1);
        best_score = max(score, best_score);
    }
    // tt.saveEntry(hash, best_score, depth);
    return best_score;
}



