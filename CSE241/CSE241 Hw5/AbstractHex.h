#ifndef ABSTRACTHEX
#define ABSTRACTHEX

#include <vector>
#include <stdexcept>
#include <string>

namespace Abstract{
    enum cell_state{Player1 = 'x', Player2 = 'o', Empty = '.', Bot = 'o'};
    class Cell{
        public:
            Cell();
            Cell(const int& r, const int& c, const cell_state& s);
            void setRow(const int& a){ row = a; }
            void setColumn(const int& b){ column = b; }
            void setState(const cell_state& c){ state = c; }
            int getRow() const{ return row; }
            int getColumn() const{ return column; }
            char getState() const{ return state; }
        private:
            int row, column;
            char state;
    };
    class past_coordinates{
        private:
            int x, y;
        public:
            void setx(const int& a){ x = a; }
            void sety(const int& b){ y = b; }
            int getx() const{ return x; }
            int gety() const{ return y; }
    };
    class AbstractHex{
        public:
            virtual void print() = 0;
            virtual void readFromFile(int& player_turn, const std::string& filename) = 0;
            virtual void writeToFile(const int& player_turn, const std::string& filename) const = 0;
            virtual void reset() = 0;
            virtual void setSize(const int& size) = 0;
            virtual void play() = 0;
            virtual void play(const Cell& coordinates) = 0;
            virtual void playGame() = 0;
            bool isEnd();
            virtual Cell operator()(const int& x, const int& y) = 0;
            bool operator==(AbstractHex& other);
            Cell lastMove();
            int numberOfMoves(){ return moveNum; }
            virtual bool isWinner_P1(const int& x, const int& y, int rotation) = 0;
            virtual bool isWinner_P2(const int& x, const int& y, int rotation) = 0;
            virtual bool check_cell_state(const int& x, const char& y) const = 0;
            bool check_coordinates(const int& x, const int& y, const cell_state& player);
            virtual bool isConnected(const cell_state& player) = 0;
            virtual void find_beginning(const cell_state& player) = 0;
            void initializePastCoordinates();
            void resetPastCoordinates(const cell_state& player);
            void setName(const std::string& n){ name = n; }
            void setIsloaded(const int& var){ isloaded = var; }
            int getSize() const{ return boardSize; }
            int get_pastx() const{ return past_x; }
            char get_pasty() const{ return past_y; }
            int getGameStatus() const{ return gameStatus; }
            int getGameMode() const{ return gameMode; }
            std::string getName() const{ return name; }
            int occupied_indexs(std::vector<past_coordinates> vec, const int& size);
            AbstractHex();
            AbstractHex(const int& s);
            AbstractHex(const int& s, const int& gm);
        protected:
            std::vector<past_coordinates> p1_past_coordinates;
            std::vector<past_coordinates> p2_past_coordinates;
            std::vector<int> p1_first_coordinates;
            std::vector<int> p2_first_coordinates;
            Cell last_move;
            int gameMode, boardSize, past_x, gameStatus, isloaded, turn, moveNum;
            char past_y;
            std::string name;
    };
}
namespace Exceptions{
    class OutOfSize : public std::runtime_error{
        public:
            OutOfSize() : std::runtime_error("Index arguments are bigger than the size of container"){}
    };
    class NoLastMove : public std::runtime_error{
        public:
            NoLastMove() : std::runtime_error("There is no last move"){}
    };
}
#endif