#include "mcts.hpp"


MCTSNode::MCTSNode(Board b, MCTSNode* par) : state(b), parent(par) {
    auto new_data = getLegalMoves(state);
    if (new_data.first.empty()) {
        is_fully_expanded = true;
    } 
    else {
        for (int i = 0; i < new_data.first.size(); i++) {
            untried_actions.push_back(new_data.first[i]);
            new_states.push_back(new_data.second[i]);
        }
    }
}

MCTSNode::~MCTSNode() {
    for (MCTSNode* child : children) {
        delete child;
    }
};

pair<Move, Board> MCTSNode::get_move_and_state() {
    uniform_int_distribution<int> dis(0, untried_actions.size()-1);
    int idx = dis(rng);
    Move action = untried_actions[idx];
    untried_actions.erase(untried_actions.begin()+idx);
    Board new_state = new_states[idx];
    new_states.erase(new_states.begin()+idx);
    if (untried_actions.empty()) is_fully_expanded = true;
    return make_pair(action, new_state);
}


MCTS::MCTS(int board_dim) : b_dim(board_dim), node_count(0) {}

Move MCTS::searchBestMove(Board board, int iterations, int rollout_depth) {

    assert(iterations >= 100 && iterations <= 4000);
    MCTSNode* root_node = new MCTSNode(board);
    if (root_node->untried_actions.size() == 1) {
        return root_node->untried_actions.front();
    }
    // run simulations
    for (int i = 0; i < iterations; i++) {
        // select best node
        MCTSNode* selected_node = select(*root_node);
        // simulate the node according to random strategy
        double reward = rollout(selected_node->state, rollout_depth);
        // backpropagate the result to root node
        backpropagate(*selected_node, reward);
    }
    // choose best node
    double best_value = -INFINITY;
    MCTSNode* best_node = nullptr;
    for (MCTSNode* child : root_node->children) {
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

MCTSNode* MCTS::getBestChild(MCTSNode& curr_node, double c) {

    double best_uct = -INFINITY;
    MCTSNode* best_child = nullptr;
    // take the best child according to UCT
    for (MCTSNode* child : curr_node.children) {
        if (child->visits == 0) return child;
        double exploitation = child->value / child->visits;
        double exploration = c * sqrt(log(child->parent->visits) / child->visits);
        double uct_score = exploitation + exploration + 1e-6;
        if (uct_score > best_uct) {
            best_uct = uct_score;
            best_child = child;
        }
    }
    return best_child;
} 

MCTSNode* MCTS::select(MCTSNode& root_node) {

    MCTSNode* curr_node = &root_node;
    while (!isGameEnd(curr_node->state)) {
        if (!curr_node->is_fully_expanded) {
            return expand(*curr_node);
        }
        curr_node = getBestChild(*curr_node);
    }
    return curr_node;
}

MCTSNode* MCTS::expand(MCTSNode& curr_node) {

    node_count += 1;
    auto new_info = curr_node.get_move_and_state();
    Board new_state = insertRandomTile(new_info.second);
    MCTSNode* new_node = new MCTSNode(new_state, &curr_node);
    new_node->move = new_info.first;
    // generate a new state by inserting a random tile
    curr_node.children.push_back(new_node);
    return new_node;
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

void MCTS::backpropagate(MCTSNode& curr_node, double reward) {
    
    MCTSNode* node = &curr_node;
    while (node != nullptr) {
        node->visits += 1;
        node->value += reward;
        node = node->parent;
    }
}
