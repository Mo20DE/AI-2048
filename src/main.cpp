#include "board.hpp"
#include "expectimax.hpp"
#include "mcts.hpp"
#include <fstream>

#include "json.hpp"
using json = nlohmann::json;

struct Statistics {
    Board end_board;
    int final_score;
    Statistics(Board b, int s) 
    : end_board(b), final_score(s) {}
};


void test2048Game() {

    array<string, 4> move_directions = {"LEFT", "RIGHT", "UP", "DOWN"};
    Board board = initializeBoard(4);
    printBoard(board);

    ExpectiMax expectimax(4);
    MCTS mcts(4);
    
    int depth = 2;
    int algo = 0; // 0 - expectimax | 1 - mcts

    cout << endl << "CHOOSE MODE:" << endl;
    cout << endl << "    1. AUTO - 'a'" << endl;
    cout << endl << "    2. PLAY - 'p'" << endl;

    cout << endl << "Mode: ";
    string mode;
    // cin >> mode;
    mode = "a"; // AUTO-RUN

    while (mode != "a" && mode != "p") {
        cout << "Please choose correct mode." << endl;
        cout << endl << "Mode: ";
        cin >> mode;
    }

    while (true) {

        // if (input == "q") break;
        if (isGameEnd(board)) {
            cout << endl << "Game Over" << endl;
            break;
        }
        cout << endl << "Move: ";
        if (mode == "a") {
            Move best_move;
            if (algo == 0) best_move = expectimax.searchBestMove(board, depth);
            else best_move = mcts.searchBestMove(board);
            cout << move_directions[best_move] << endl;
            Board new_board = executeMove(board, best_move).board;
            if (boardChanged(board, new_board)) {
                board = insertRandomTile(new_board);
            }
            printBoard(board);
        }
        else {
            
            string input;
            cin >> input;

            if (input == "r") {
                cout << "New Board Initialized:" << endl;
                board = initializeBoard(4);
                printBoard(board);
            }
        
            else if (input == "w" || input == "a" || input == "s" || input == "d") {

                Move move = input == "w" ? Up : input == "s" ? Down : input == "a" ? Left : Right;
                Board new_board = executeMove(board, move).board;

                if (boardChanged(board, new_board)) {
                    board = insertRandomTile(new_board);
                    printBoard(board);
                }
                else cout << endl << "Board didn't alter." << endl;

            }
        }
    }
}

void testTranspositionTable() {
    
    int dim = 4;
    Board board = initializeBoard(dim);
    printBoard(board);
    // TranspositionTable tt(dim);

    // uint64_t hash = tt.computeHash(board);
    // cout << hash << endl;
    
    // tt.saveEntry(hash, 2.4f, 3);
    // auto entry = tt.lookup(hash);

    // cout << entry.has_value() << endl;

    // cout << entry->utility << endl;
    // cout << entry->depth << endl;

    // cout << tt.computeHash(board);
    // tt.saveEntry(hash, 1., 3);
    // TTEntry entry = tt.lookup(hash);
}

void plotExpectimaxData() {

}

void plotMCTSData() {
    
}

void saveStatistics(Statistics stats, const string filename) {

    // auto i = stats.end_board.data();
    // std::vector<int> board_vector(i, i + stats.end_board.size());
    int dim = stats.end_board.rows();

    json json_board = json::array();
    for (int i = 0; i < dim; i++) {
        json row = json::array();
        for (int j = 0; j < dim; j++) {
            row.push_back(stats.end_board(i, j));
        }
        json_board.push_back(row);
    }

    // creat wrapper for data
    json data = json{
        {"Board", json_board},
        {"Score", stats.final_score}
    };

    // read data 
    json json_data;
    ifstream infile("../data/"+filename);
    if (infile.is_open()) {
        infile >> json_data;
        infile.close();
    }
    else json_data = json::array();

    // append the new data
    json_data.push_back(data);

    ofstream file("../data/"+filename, ios::trunc);
    if (file.is_open()) {
        file << json_data.dump(4);
        file.close();
        cout << "Data was successfully saved to " + filename + " !\n";
    }
    else cerr << "Error opening file " + filename + " for writing data!";
}

void runSimulation() {

    static const string MOVES[4] = {"LEFT", "RIGHT", "UP", "DOWN"};

    // SIMULATION PARAMETERS
    int TOTAL_SIMULATIONS = 50;

    int GAME_MODE = 1; // 0 - Expectimax, 1 - MCTS
    int dim = 4; // Board Dimension
    int depth = 2; // Tree Depth

    // initialize algorithm classes
    ExpectiMax expmax(dim);
    MCTS mcts(dim);


    for (int i = 0; i < TOTAL_SIMULATIONS; i++) {

        Board board = initializeBoard(dim);
        printBoard(board);
        int game_score = 0;

        while (!isGameEnd(board)) {

            Move best_move;
            if (GAME_MODE == 0) best_move = expmax.searchBestMove(board, depth);
            else best_move = mcts.searchBestMove(board);

            BoardData new_state = executeMove(board, best_move);
            game_score += new_state.score;

            if (boardChanged(board, new_state.board)) {
                board = insertRandomTile(new_state.board);
            }

            cout << endl << "---------------------------- \n\n";
            cout << "Move: " << MOVES[best_move] << "\n";
            cout << "Game Score: " << game_score << "\n";
            printBoard(board);
        }

        expmax.tt.resetTranspositionTable();

        saveStatistics(Statistics(board, game_score), "expectimax_data.json");
    }

    // current best: 41.9892 38.0214 84.8506
}

void testParseData() {
    Statistics data(initializeBoard(4), 20);
    saveStatistics(data, "mcts_data.json");
}

int main() {
    runSimulation();
    // testParseData();
    return 0;
}
