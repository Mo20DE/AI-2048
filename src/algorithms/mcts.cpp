#include "mcts.hpp"
#include "heuristics.hpp"
#include "board.hpp"


MCTS::MCTS(int board_dim) : b_dim(board_dim), tt(board_dim) {}

Move MCTS::searchBestMove(Board board, int iterations) {

    ChanceNode* root_node = new ChanceNode(board);
    if (root_node->untried_actions.size() == 1) {
        return root_node->untried_actions.front();
    }
    // firstly expand the root node (chance node)
    int num_new_states = root_node->new_states.size();
    for (int i = 0; i < num_new_states; i++) {
        auto new_info = root_node->get_move_and_state();
        ExpectimaxNode* new_child = new ExpectimaxNode(new_info.second, root_node, new_info.first);
        root_node->children.push_back(new_child);
    }
    // run simulations
    for (int i = 0; i < iterations; i++) {
        // select best node
        ChanceNode* selected_node = select(*root_node);
        // simulate the node according to random strategy
        double reward = rollout(selected_node->state);
        // backpropagate the result to root node
        backpropagate(*selected_node, reward);
    }
    // choose best node
    double best_value = -INFINITY;
    ExpectimaxNode* best_node = nullptr;
    Eigen::VectorXd all(root_node->children.size());
    int i = 0;
    for (ExpectimaxNode* child : root_node->children) {
        all(i) = child->visits;
        int visits = child->visits > 0 ? child->visits : 1;
        float child_value = child->value / visits;
        if (child_value > best_value) {
            best_value = child_value;
            best_node = child;
        }
        i++;
    }
    Move best_move = best_node->move;
    delete root_node;
    return best_move;
}

ExpectimaxNode* MCTS::getBestChild(ChanceNode& curr_node, double c) {
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

ChanceNode* MCTS::select(ChanceNode& root_node) {
    // !! FEHLER IN SCHLEIFEN LOGIK (JEDER CHANCE NODE BAUT DIREKT NEUE EXPECTIMAX NODES)
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
            // Create a new ChanceNode if not found
            ChanceNode* new_chance_node = new ChanceNode(new_state, best_node);
            best_node->all_child_board_hashes.push_back(hash);
            best_node->children.push_back(new_chance_node);
            curr_node = static_cast<ChanceNode*>(best_node->children.back());
            return curr_node;
        }
    }
    return curr_node;
}

ChanceNode* MCTS::expand(ChanceNode& curr_node) {

    auto new_info = curr_node.get_move_and_state();
    ExpectimaxNode* new_expectimax_node = new ExpectimaxNode(new_info.second, &curr_node, new_info.first);

    // generate a new state by inserting random tile
    Board new_state2 = insertRandomTile(new_info.second);
    ChanceNode* new_chance_node = new ChanceNode(new_state2, new_expectimax_node);

    uint64_t hash = tt.computeHash(new_state2);
    new_expectimax_node->all_child_board_hashes.push_back(hash);
    new_expectimax_node->children.push_back(new_chance_node);
    curr_node.children.push_back(new_expectimax_node);

    return curr_node.children.back()->children.back();
}

double MCTS::rollout(Board state, int rollout_depth) {

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

void MCTS::backpropagate(Node& curr_node, double reward) {
    Node* node = &curr_node;
    while (node != nullptr) {
        node->visits += 1;
        node->value += reward;
        node = node->parent;
    }
}

