#pragma once

#include "globals.hpp"


struct Node {

    Board state;
    Node* parent;
    int visits; // if we got two nodes: vis > 0 & vis = 0 then choose vis = 0 -> the UCB is infinite
    double value;
    bool is_fully_expanded;
    
    Node(Board b, Node* par = nullptr);
    virtual ~Node();
};

struct ChanceNode;

struct ExpectimaxNode : Node {

    vector<ChanceNode*> children;
    Move move; // the move which lead to this board
    // TranspositionTable1 transposition_table;
    vector<uint64_t> all_child_board_hashes;

    ExpectimaxNode(Board b, Node* par, Move mov);
    ~ExpectimaxNode();
};

struct ChanceNode : Node {

    vector<ExpectimaxNode*> children;
    vector<Move> untried_actions;
    vector<Board> new_states;

    ChanceNode(Board b, ExpectimaxNode* par = nullptr);
    ~ChanceNode();
    pair<Move, Board> get_move_and_state();
};
