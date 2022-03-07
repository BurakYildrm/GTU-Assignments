#ifndef HEXVECTOR
#define HEXVECTOR

#include "AbstractHex.h"

using namespace Abstract;

namespace HexVec{
    class HexVector : public AbstractHex{
        public:
            void print();
            void readFromFile(int& player_turn, const std::string& filename);
            void writeToFile(const int& player_turn, const std::string& filename) const;
            void reset();
            void setSize(const int& size);
            void play(const Cell& coordinates);
            void play();
            void playGame();
            Cell operator()(const int& x, const int& y);
            bool isWinner_P1(const int& x, const int& y, int rotation);
            bool isWinner_P2(const int& x, const int& y, int rotation);
            bool check_cell_state(const int& x, const char& y) const;
            bool isConnected(const cell_state& player);
            void find_beginning(const cell_state& player);
            HexVector();
            HexVector(const int& s);
            HexVector(const int& s, const int& gm);
        private:
            std::vector<std::vector<Cell> > hexCells;
    };
}
#endif