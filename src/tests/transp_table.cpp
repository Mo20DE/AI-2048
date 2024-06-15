
#include "../library/utility.hpp"
#include "../library/board.hpp"

int main() {
    
    int dim = 4;
    Board board = initializeBoard(dim);
    printBoard(board);
    // TranspositionTable tt(board.size());

    // tt.saveEntry(board, 1., 3, Left);


    return 0;
}