#include "library/board.hpp"

using namespace std;

void test2048Game() {

    Board b = initializeBoard(4);
    printBoard(b);
    int score = 0, merged_tiles = 0;

    while (true) {
        string input;
        cin >> input;
        if (input == "q") break;

        else if (input == "r") {
            b = initializeBoard(4);
            printBoard(b);
        }
        
        else if (input == "w" || input == "a" || input == "s" || input == "d") {

            Move move = input == "w" ? Up : input == "s" ? Down : input == "a" ? Left : Right;
            Board new_b = makeMove(b, move, score, merged_tiles);

            if (boardChanged(b, new_b)) {
                b = new_b;
                insertRandomTile(b);
            }

            printBoard(b);
        }
    }
}

int main() {
    // unsigned int seed = static_cast<unsigned int>(std::time(nullptr));
    // std::srand(seed);

    test2048Game();

    // Board board = initializeBoard(4); // 4x4 board
    // printBoard(board);

    // int score = 0, merged_tiles = 0;

    // Board board2 = makeMove(board, Down, score, merged_tiles);
    // printBoard(board2);
    // TODO
    return 0;
}