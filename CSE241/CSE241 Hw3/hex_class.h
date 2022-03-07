#ifndef hex_class
#define hex_class
#include <vector>
enum cell_state{Player1 = 'x', Player2 = 'o', Empty = '.', Bot = 'o'};
class Hex{
    private:
        std::string name;
        int past_x;
        char past_y;
        std::vector<int> p1_first_coordinates, p2_first_coordinates;
        class past_coordinates{
            private:
                int x, y;
            public:
                void setx(const int& a);
                void sety(const int& b);
                int getx() const;
                int gety() const;
        };
        static int marked_cells;
        int board_size, game_mode, game_status, isloaded, turn;
        class Cell{
            public:
                Cell();
                void setRow(const int& a);
                void setColumn(const int& b);
                void setState(const cell_state& c);
                int getRow() const;
                char getColumn() const;
                char getState() const;
            private:
                int row, column;
                char state;
    };
    public:
        Hex(const int& size = 5);
        Hex(const int& size, const int& gm);
        void playGame();
        void play();
        void play(const int& x, const int& y, const cell_state& player);
        void print_board() const;
        void setSize(const int& s);
        void setGameMode(const int& mode);
        int getGameMode() const;
        int getSize() const;
        bool check_cell_state(const int& x, const char& y) const;
        void setStatic(const int& a);
        static int getStatic(){
            return marked_cells;
        }
        int count_marked_cells() const;
        void load_game(int& player_turn, std::string filename);
        void save_game(const int& player_turn, std::string filename) const;
        bool isWinner_P1(const cell_state& player, const int& x, const int& y, int rotation);
        bool isWinner_P2(const cell_state& player, const int& x, const int& y, int rotation);
        bool check_coordinates(const int& x, const int& y, int player);
        int occupied_indexs(std::vector<past_coordinates> vec);
        int occupied_indexs(std::vector<int> vec);
        std::vector<std::vector<Cell> >& setBoard();
        const std::vector<std::vector<Cell> > getBoard() const;
        std::vector<past_coordinates>& setPastCoordinates(int player);
        const std::vector<past_coordinates> getPastCoordinates(int player);
        std::vector<int>& setFirstCoordinates(int player);
        const std::vector<int> getFirstCoordinates(int player);
        void initializeFirstCoordinates();
        void initializePastCoordinates();
        void resetPastCoordinates(int player);
        bool isConnected(cell_state player);
        void find_beginning(cell_state player);
        void set_pastx(const int& x);
        void set_pasty(const char& y);
        int get_pastx() const;
        char get_pasty() const;
        void setGameStatus(int gs);
        int getGameStatus() const;
        bool compareGames(const Hex& other) const;
        void setName(const std::string& n);
        std::string getName() const;
        void setIsloaded(const int& a);
        void setTurn(const int& a);
        int getIsloaded() const;
        int getTurn() const;
    private:
        std::vector<std::vector<Cell> >hexCells;
        std::vector<past_coordinates> p1_past_coordinates, p2_past_coordinates;
};
#endif