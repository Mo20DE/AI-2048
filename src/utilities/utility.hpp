#include "board.hpp"
#include <fstream>
#include <chrono>
#include "json.hpp"


using json = nlohmann::json;

struct GameData {
    Board end_board;
    int final_score;
    int node_count;
    GameData(Board b, int s, int nc) 
    : end_board(b), final_score(s), node_count(nc) {}
};

pair<vector<Board>, Eigen::VectorXi> loadData(const string filename) {

    json data;
    ifstream infile("../data/"+filename);
    if (infile.is_open()) {
        infile >> data;
        infile.close();
    }
    else cerr << "Error reading file " + filename + " !\n";

    vector<Board> end_boards;
    Eigen::VectorXi end_scores(data.size());
    int dim = data[0]["Board"].size();
    int idx = 0;

    for (auto const &entry : data) {
        auto data_b = entry["Board"];
        Board board(dim, dim);
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                board(i, j) = data_b[i][j];
            }
        }
        end_boards.push_back(board);
        end_scores(idx++) = entry["Score"];
    }
    return make_pair(end_boards, end_scores);
}

void saveData(GameData stats, const string filename) {

    int dim = stats.end_board.rows();
    json json_board = json::array();

    for (int i = 0; i < dim; i++) {
        json row = json::array();
        for (int j = 0; j < dim; j++) {
            row.push_back(stats.end_board(i, j));
        }
        json_board.push_back(row);
    }

    // create wrapper for data
    json data = json{
        {"Board", json_board},
        {"Score", stats.final_score},
        {"Node Count", stats.node_count}
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

void plotData(const string filename) {
    
    auto data = loadData(filename);

    int max_tile = 0;
    float avg_max_tile = 0.;

    int count_512 = 0;
    int count_1024 = 0;
    int count_2048 = 0;
    int count_4096 = 0;
    int count_8192 = 0;

    for (Board board : data.first) {
        int board_max_tile = board.maxCoeff();
        max_tile = max(max_tile, board_max_tile);
        avg_max_tile += board_max_tile;

        count_512 += (board_max_tile >= 512);
        count_1024 += (board_max_tile >= 1024);
        count_2048 += (board_max_tile >= 2048);
        count_4096 += (board_max_tile >= 4096);
        count_8192 += (board_max_tile >= 8192);
    }
    avg_max_tile /= data.first.size();

    cout << "\nMax Score:" << data.second.maxCoeff() << endl;
    cout << "Avg. Score:" << data.second.mean() << endl;

    cout << "\nMax Tile: " <<  max_tile << endl;
    cout << "Avg. Max Tile: " <<  avg_max_tile << endl;

    cout << "\n512 Tile [%]: " <<  count_512 * 2 << "%\n";
    cout << "1024 Tile [%]: " <<  count_1024 * 2 << "%\n";
    cout << "2048 Tile [%]: " <<  count_2048 * 2 << "%\n";
    cout << "4096 Tile [%]: " <<  count_4096 * 2 << "%\n";
    cout << "8192 Tile [%]: " <<  count_8192 * 2 << "%\n\n" << endl;
}
