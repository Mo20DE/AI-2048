#include "board.hpp"
#include "node.hpp"

    
Node::Node(Board b, Node* par)
    : state(b), parent(par), visits(0), value(0.), is_fully_expanded(false) {}
    
Node::~Node() {};


ExpectimaxNode::ExpectimaxNode(Board b, Node* par, Move mov) : 
    Node(b, par), move(mov) {}
    
ExpectimaxNode::~ExpectimaxNode() {
    for (ChanceNode* child : children) {
            delete child;
    }
}

ChanceNode::ChanceNode(Board b, ExpectimaxNode* par) : Node(b, par) {
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

ChanceNode::~ChanceNode() {
    for (ExpectimaxNode* child : children) {
        delete child;
    }
}

pair<Move, Board> ChanceNode::get_move_and_state() {
    uniform_int_distribution<int> dis(0, untried_actions.size()-1);
    int idx = dis(rng);
    Move action = untried_actions[idx];
    untried_actions.erase(untried_actions.begin()+idx);
    Board new_state = new_states[idx];
    new_states.erase(new_states.begin()+idx);
    if (untried_actions.empty()) is_fully_expanded = true;
    return make_pair(action, new_state);
}