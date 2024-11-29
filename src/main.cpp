#include "board.hpp"
#include "expectimax.hpp"
#include "mctse.hpp"
#include "mcts.hpp"
#include "utilities/utility.hpp"


void runSimulation(int algo, int deepness) {

    static const string MOVES[4] = {"LEFT", "RIGHT", "UP", "DOWN"};
    int GAME_MODE = algo; // 0 - Expectimax, 1 - MCTSE, 2 - MCTS
    int dim = 4; // Board Dimension
    
    // initialize algorithm classes
    ExpectiMax expmax(dim);
    MCTSE mctse(dim);
    MCTS mcts(dim);

    Board board = initializeBoard(dim);
    cout << "\nInitial Board:\n";
    printBoard(board);

    int game_score = 0;
    double iter_time = 0;
    int iter_moves = 0;

    while (!isGameEnd(board)) {

        Move best_move;
        auto start = chrono::high_resolution_clock::now();
        if (GAME_MODE == 0) best_move = expmax.searchBestMove(board, deepness);
        else if (GAME_MODE == 1) best_move = mctse.searchBestMove(board, deepness);
        else best_move = mcts.searchBestMove(board, deepness);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> runtime = end - start;

        BoardData new_state = executeMove(board, best_move);
        game_score += new_state.score;

        if (boardChanged(board, new_state.board)) {
            board = insertRandomTile(new_state.board);
        }

        iter_time += runtime.count();
        ++iter_moves;

        cout << "\n------------------------------------\n\n";
        cout << "Move: " << MOVES[best_move] << "\n";
        cout << "Game Score: " << game_score << "\n";
        printBoard(board);
    }
    int node_count = algo == 0 ? expmax.node_count : algo == 1 ? mctse.node_count : mcts.node_count;

    cout << "\nFinal Score: " << game_score << "\n";
    cout << "Total " << (algo == 0 ? "recursive calls: " : "nodes generated: ") << node_count << "\n";
    cout << "Avg. Time per Move (s): " << iter_time / iter_moves << "\n\n";

    string algorithms[] = {"Expectimax", "MCTSE", "MCTS"};
    string num = to_string(deepness);
    string s = algo == 0 ? "depth " + num : num +" iterations";
    cout << "This simulation used " + algorithms[algo] + " with " + s << "\n";
    cout << endl;
}

int main(int argc, char* argv[]) {

    if (argc != 3) {
        cerr << "Only provide <Algorithm> <Depth | Iterations> as arguments.";
        return 1;
    }

    int algo = stoi(argv[1]);
    int deepness = stoi(argv[2]);
    runSimulation(algo, deepness);

    return 0;
}
