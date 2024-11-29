#pragma once

#include "transposition_table.hpp"
#include "node.hpp"


class MCTSE {

    public:
        MCTSE(int board_dim);
        Move searchBestMove(Board board, int iterations = 1000, int rollout_depth = 8);

        int b_dim;
        mutable TranspositionTable<TTEntryExpMax> tt;
        mutable int node_count;

    private:
        ExpectimaxNode* getBestChild(ChanceNode& curr_node, double c = sqrt(2));
        ChanceNode* select(ChanceNode& root_node); // according to (modified) UCT
        ChanceNode* expand(ChanceNode& curr_node);
        double rollout(Board state, int rollout_depth);
        void backpropagate(Node& curr_node, double reward);
};
