#pragma once

#include "transposition_table.hpp"

class ExpectiMax {

    public:
        ExpectiMax(int board_dim);
        Move searchBestMove(Board board, int depth);

        int b_dim;
        mutable TranspositionTable<TTEntryExpMax> tt; // initial transposition table
    
    protected:
        float chance_node(Board state, int depth) const;
        float max_node(Board state, int depth) const;
};
