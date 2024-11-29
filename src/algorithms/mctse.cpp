#include "mctse.hpp"
#include "board.hpp"


MCTSE::MCTSE(int board_dim) : b_dim(board_dim), tt(board_dim), node_count(0) {}

Move MCTSE::searchBestMove(Board board, int iterations, int rollout_depth) {

    assert(iterations >= 100 && iterations <= 4000);
    ChanceNode* root_node = new ChanceNode(board);
    if (root_node->untried_actions.size() == 1) {
        return root_node->untried_actions[0];
    }
    // run simulations
    for (int i = 0; i < iterations; i++) {
        // select best node
        ChanceNode* selected_node = select(*root_node);
        // simulate the node according to random strategy
        double reward = rollout(selected_node->state, rollout_depth);
        // backpropagate the result to root node
        backpropagate(*selected_node, reward);
    }
    // choose best node
    double best_value = -INFINITY;
    ExpectimaxNode* best_node = nullptr;
    for (ExpectimaxNode* child : root_node->children) {
        int visits = child->visits > 0 ? child->visits : 1;
        float child_value = child->value / visits;
        if (child_value > best_value) {
            best_value = child_value;
            best_node = child;
        }
    }
    Move best_move = best_node->move;
    delete root_node;
    return best_move;
}

ExpectimaxNode* MCTSE::getBestChild(ChanceNode& curr_node, double c) {
    
    double best_uct = -INFINITY;
    ExpectimaxNode* best_child = nullptr;
    // take the best child according to UCT
    for (ExpectimaxNode* child : curr_node.children) {
        if (child->visits == 0) return child;
        double exploitation = child->value / child->visits;
        double exploration = c * sqrt(log(child->parent->visits) / child->visits);
        double uct_score = exploitation + exploration + 1e-6;
        if (uct_score > best_uct) {
            best_uct = uct_score;
            best_child = static_cast<ExpectimaxNode*>(child);
        }
    }
    return best_child;
} 

ChanceNode* MCTSE::select(ChanceNode& root_node) {

    ChanceNode* curr_node = &root_node; // has min. 2 child nodes
    while (!isGameEnd(curr_node->state)) {
        if (!curr_node->is_fully_expanded) {
            return expand(*curr_node);
        }
        ExpectimaxNode* best_node = getBestChild(*curr_node);
        Board new_state = insertRandomTile(best_node->state);
        uint64_t hash = tt.computeHash(new_state);
        // Check if new state exists in child hashes
        auto it = std::find(best_node->all_child_board_hashes.begin(), best_node->all_child_board_hashes.end(), hash);
        if (it != best_node->all_child_board_hashes.end()) {
            int index = std::distance(best_node->all_child_board_hashes.begin(), it);
            curr_node = static_cast<ChanceNode*>(best_node->children[index]);
        } else {
            node_count++;
            // Create a new chance node if not found
            ChanceNode* new_chance_node = new ChanceNode(new_state, best_node);
            best_node->all_child_board_hashes.push_back(hash);
            best_node->children.push_back(new_chance_node);
            curr_node = new_chance_node;
        }
    }
    return curr_node;
}

ChanceNode* MCTSE::expand(ChanceNode& curr_node) {

    node_count += 2;
    auto new_info = curr_node.get_move_and_state();
    ExpectimaxNode* new_expectimax_node = new ExpectimaxNode(new_info.second, &curr_node, new_info.first);

    // generate a new state by inserting random tile
    Board new_state2 = insertRandomTile(new_info.second);
    ChanceNode* new_chance_node = new ChanceNode(new_state2, new_expectimax_node);

    uint64_t hash = tt.computeHash(new_state2);
    new_expectimax_node->all_child_board_hashes.push_back(hash);
    new_expectimax_node->children.push_back(new_chance_node);
    curr_node.children.push_back(new_expectimax_node);

    return new_chance_node;
}

double MCTSE::rollout(Board state, int rollout_depth) {

    Board curr_state = state;
    int curr_depth = 0;
    while (!isGameEnd(curr_state) && curr_depth < rollout_depth) {
        auto move_info = getLegalMoves(curr_state);
        uniform_int_distribution<int> dis(0, move_info.first.size()-1);
        curr_state = insertRandomTile(move_info.second[dis(rng)]);
        curr_depth++;
    }
    return getEmptyCells(curr_state).size();
}

void MCTSE::backpropagate(Node& curr_node, double reward) {

    Node* node = &curr_node;
    while (node != nullptr) {
        node->visits += 1;
        node->value += reward;
        node = node->parent;
    }
}
