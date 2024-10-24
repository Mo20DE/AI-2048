#pragma once

#include "transposition_table.hpp"
#include "node.hpp"


class MCTS {

    public:
        MCTS(int board_dim);
        Move searchBestMove(Board board, int iterations = 1500);

        int b_dim;
        mutable TranspositionTable<TTEntryExpMax> tt;

    private:
        ExpectimaxNode* getBestChild(ChanceNode& curr_node, double c = sqrt(2));
        ChanceNode* select(ChanceNode& root_node); // according to (modified) UCT
        ChanceNode* expand(ChanceNode& curr_node);
        double rollout(Board state, int rollout_depth = 6);
        void backpropagate(Node& curr_node, double reward);
};
