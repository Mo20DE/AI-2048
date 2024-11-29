#include "board.hpp"
#include "expectimax.hpp"
#include "heuristics.hpp"


ExpectiMax::ExpectiMax(int board_dim) : b_dim(board_dim), tt(board_dim), node_count(0) {}

Move ExpectiMax::searchBestMove(Board board, int depth) {

    assert(depth >= 1 && depth <= 10);
    auto data = getLegalMoves(board);
    if (data.first.size() == 1) {
        return data.first[0];
    }

    float best_utility = -INFINITY;
    int best_move_idx = 0;

    for (int i = 0; i < data.first.size(); i++) {
        Board new_state = data.second[i];
        float utility = chance_node(new_state, depth);
        if (utility > best_utility) {
            best_utility = utility;
            best_move_idx = i;
        }
    }

    return data.first[best_move_idx];
}

float ExpectiMax::chance_node(Board state, int depth) const {
    
    node_count++;
    if (depth <= 0) return evaluateState(state);

    uint64_t hash = tt.computeHash(state);
    auto entry = tt.lookup(hash);
    if (entry.has_value()) {
        // already computed utility for same board, but deeper
        if (entry->depth <= depth) {
            return entry->utility;
        }
    }

    float total_utility = 0.0;
    Cells empty_cells = getEmptyCells(state);
    for (int i = 0; i < empty_cells.size(); i++) {

        pair<int, int> cell = empty_cells[i];

        Board new_state_2 = state;
        new_state_2(cell.first, cell.second) = 2;
        total_utility += 0.9 * max_node(new_state_2, depth - 1);

        Board new_state_4 = state;
        new_state_4(cell.first, cell.second) = 4;
        total_utility += 0.1 * max_node(new_state_4, depth - 1);
    }

    total_utility /= empty_cells.size();
    tt.saveEntry(hash, TTEntryExpMax(total_utility, depth));

    return total_utility;
}

float ExpectiMax::max_node(Board state, int depth) const {

    node_count++;
    if (isGameEnd(state)) return -10000;
    if (depth <= 0) return evaluateState(state);

    float best_score = -INFINITY;
    auto data = getLegalMoves(state);
    for (int i = 0; i < data.first.size(); i++) {
        Board new_state = data.second[i];
        float score = chance_node(new_state, depth - 1);
        best_score = max(score, best_score);
    }

    return best_score;
}
