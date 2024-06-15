
#include "global.hpp"

// Transposition Table Entry
struct TTEntry {
    float value; // utility
    int depth;
    Move bestmove;
};

// Transposition Table
class TranspositionTable {

    public:
        int dim;
        unordered_map<unsigned long long, TTEntry> transp_table;

        TranspositionTable(int d) : dim(d) {
            initializeZobristTable();
            initializeTileMappingTable();
        }

        TTEntry lookup(Board board) {
            unsigned long long hash = computeHash(board);
            return transp_table[hash];
        }

        void saveEntry(Board board, float value, int depth, Move bestmove) {
            unsigned long long hash = computeHash(board);
            cout << hash << endl;
            TTEntry table_entry = {value, depth, bestmove};
            transp_table[hash] = table_entry;
        }

        void resetTranspositionTable() {
            transp_table.clear();
        }

    private:

        vector<vector<vector<unsigned long long> > > zobristTable;
        unordered_map<int, int> tileTable; // log2 table

        unsigned long long computeHash(Board board) {
            unsigned long long hash = 0;
            for (int row = 0; row < dim; row++) {
                for (int col = 0; col < dim; col++) {
                    int tile = tileTable[board(row, col)];
                    hash ^= zobristTable[row][col][tile];
                }
            }
            return hash;
        }

        void initializeZobristTable() {

            uniform_real_distribution<unsigned long long> dis1;
            zobristTable.resize(dim, vector<vector<unsigned long long> >(dim, vector<unsigned long long>(20)));

            for (int row = 0; row < dim; row++) {
                for (int col = 0; col < dim; col++) {
                    for(int tile = 0; tile < 20; tile++) {
                        // 0 to 19 - tile 2 to 1.048.576
                        zobristTable[row][col][tile] = dis1(rng);
                        cout << zobristTable[row][col][tile] << endl;
                    }
                }
            }
        }

        void initializeTileMappingTable() {
            // 0 to 19 - tile 2 to 1.048.576
            for (int i = 0; i < 20; i++) {
                tileTable[static_cast<int>(pow(2, i))] = i;
            }
        }
};
