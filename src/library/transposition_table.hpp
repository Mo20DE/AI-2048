#pragma once

#include "globals.hpp"

// Transposition Table Entry
struct TTEntryExpMax {
    float utility;
    int depth;
};

struct TTEntryMCTS {

};

template <typename TTEntryType>
// Transposition Table
class TranspositionTable {

    public:
        TranspositionTable(int b_dim);
        std::optional<TTEntryType> lookup(uint64_t hash);
        void saveEntry(uint64_t hash, TTEntryType entry);
        // bool contains(unsigned long long hash);
        void resetTranspositionTable();
        uint64_t computeHash(Board board);

        int dim;

    private:
        unordered_map<uint64_t, TTEntryType> transp_table;
        void initializeZobristTable();
        void initializeTileMappingTable();

        vector<vector<vector<uint64_t> > > zobristTable;
        unordered_map<int, int> tileMappingTable; // log2 table
};

#include "transposition_table.hpp"

// public functions
template <typename TTEntryType>
TranspositionTable<TTEntryType>::TranspositionTable(int b_dim) : dim(b_dim) {
    initializeZobristTable();
    initializeTileMappingTable();
}

template <typename TTEntryType>
std::optional<TTEntryType> TranspositionTable<TTEntryType>::lookup(uint64_t hash) {
    auto it = transp_table.find(hash);
    if (it != transp_table.end()) {
        return it->second;
    }
    return std::nullopt;
}

template <typename TTEntryType>
void TranspositionTable<TTEntryType>::saveEntry(uint64_t hash, TTEntryType entry) {
    // TTEntryType entry = {utility, depth};
    transp_table[hash] = entry;
}

template <typename TTEntryType>
void TranspositionTable<TTEntryType>::resetTranspositionTable() {
    transp_table.clear();
}

// private functions
template <typename TTEntryType>
uint64_t TranspositionTable<TTEntryType>::computeHash(Board board) {
    uint64_t hash = 0;
    for (int row = 0; row < dim; row++) {
        for (int col = 0; col < dim; col++) {
            int tile = tileMappingTable[board(row, col)];
            hash ^= zobristTable[row][col][tile];
        }
    }
    return hash;
}

template <typename TTEntryType>
void TranspositionTable<TTEntryType>::initializeZobristTable() {

    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<uint64_t> dis(0, ULLONG_MAX);
    zobristTable.resize(dim, vector<vector<uint64_t> >(dim, vector<uint64_t>(20)));

    for (int row = 0; row < dim; row++) {
        for (int col = 0; col < dim; col++) {
            for(int tile = 0; tile < 20; tile++) {
                // 0 to 19 - tile 2 to 1.048.576
                zobristTable[row][col][tile] = dis(gen);
            }
        }
    }
}

template <typename TTEntryType>
void TranspositionTable<TTEntryType>::initializeTileMappingTable() {
    // 0 to 19 - tile 2 to 1.048.576
    for (int i = 0; i < 20; i++) {
        tileMappingTable[static_cast<int>(pow(2, i))] = i;
    }
}

