#pragma once

#include "board.hpp"


struct MCTSNode {

    Board state;
    MCTSNode* parent;
    int visits;
    double value;
    bool is_fully_expanded;

    vector<MCTSNode*> children;
    vector<Move> untried_actions;
    vector<Board> new_states;
    Move move;
    
    MCTSNode(Board b, MCTSNode* par = nullptr);
    virtual ~MCTSNode();
    pair<Move, Board> get_move_and_state();
};

class MCTS {

    public:
        MCTS(int board_dim);
        Move searchBestMove(Board board, int iterations = 1000, int rollout_depth = 8);

        int b_dim;
        mutable int node_count;

    private:
        MCTSNode* getBestChild(MCTSNode& curr_node, double c = sqrt(2));
        MCTSNode* select(MCTSNode& root_node); // according to (modified) UCT
        MCTSNode* expand(MCTSNode& curr_node);
        double rollout(Board state, int rollout_depth);
        void backpropagate(MCTSNode& curr_node, double reward);
};
