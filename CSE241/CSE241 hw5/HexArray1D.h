#ifndef HEXARRAY1D
#define HEXARRAY1D

#include "AbstractHex.h"

using namespace Abstract;

namespace HexArr{
    class HexArray1D : public AbstractHex{
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
            ~HexArray1D();
            HexArray1D();
            HexArray1D(const int& s);
            HexArray1D(const int& s, const int& gm);
            HexArray1D(const HexArray1D& other);
            HexArray1D& operator=(const HexArray1D& other);
        private:
            Cell *hexCells;
    };
}
#endif