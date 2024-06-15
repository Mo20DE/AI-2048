
#include "global.hpp"

class Algorithm {
    
    public:
        Algorithm(Board b) : board(b) {}
        // define heuristics
    
    private:
        Board board; // initial board state
};


class Expectimax : public Algorithm {
    public:
        Expectimax(Board b) : Algorithm(b) {
            // initialize heuristics for expectimax
            
        }
};