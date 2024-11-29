#pragma once

#include "transposition_table.hpp"


class ExpectiMax {

    public:
        ExpectiMax(int board_dim);
        Move searchBestMove(Board board, int depth = 3);

        int b_dim;
        mutable TranspositionTable<TTEntryExpMax> tt; 
        mutable int node_count;
    
    protected:
        float chance_node(Board state, int depth) const;
        float max_node(Board state, int depth) const;
};
