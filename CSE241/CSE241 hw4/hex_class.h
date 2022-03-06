#ifndef hex_class
#define hex_class
enum cell_state{Player1 = 'x', Player2 = 'o', Empty = '.', Bot = 'o'};
class Hex{
    private:
        /*I made these functions private because client shouldn't be able to change the size and the static value outside the playGame function*/
        void setSize(const int& s);
        void setStatic(const int& a){ marked_cells += a; }
        std::string name;
        int past_x;
        char past_y;
        int *p1_first_coordinates, *p2_first_coordinates, *connectedCells_storage;
        class past_coordinates{
            private:
                int x, y;
            public:
                void setx(const int& a){ x = a; }
                void sety(const int& b){ y = b; }
                int getx() const{ return x; }
                int gety() const{ return y; }
        };
        static int marked_cells, connectedCells;
        int board_size, game_mode, game_status, isloaded, turn;
    public:
        class Cell{
            public:
                Cell();
                Cell(const int& r, const int& c, const cell_state& s);
                void setRow(const int& a){ row = a; }
                void setColumn(const int& b){ column = b; }
                void setState(const cell_state& c){ state = c; }
                int getRow() const{ return row; }
                char getColumn() const{ return column; }
                char getState() const{ return state; }
            private:
                int row, column;
                char state;
        };
        /*----The Big Three----*/
        Hex();
        explicit Hex(const int& size);
        ~Hex();
        Hex(const Hex& other);
        /*---Other Functions---*/
        Cell play();
        void play(const Cell& coordinates);
        void playGame();
        bool check_cell_state(const int& x, const char& y) const;
        int count_marked_cells() const;
        void load_game(int& player_turn, std::string filename);
        void save_game(const int& player_turn, std::string filename) const;
        bool isWinner_P1(const int& x, const int& y, int rotation);
        bool isWinner_P2(const int& x, const int& y, int rotation);
        bool check_coordinates(const int& x, const int& y, const cell_state& player);
        int occupied_indexs(const past_coordinates *vec, const int& size);
        int occupied_indexs(const int *vec, const int& size);
        void initializeFirstCoordinates();
        void initializePastCoordinates();
        void resetPastCoordinates(int player);
        bool isConnected(cell_state player);
        void find_beginning(cell_state player);
        void delete_last_element();
        int score_of_player(cell_state player);
        bool connected_cells_p1(const int& x, const int& y, int rotation, const int& dest_x, const int& dest_y);
        bool connected_cells_p2(const int& x, const int& y, int rotation, const int& dest_x, const int& dest_y);
        void find_occurances(const cell_state& player);
        void reset_score_coordinates();
        /*---Setters---*/
        void setName(const std::string& n){ name = n; }
        void setIsloaded(const int& var){ isloaded = var; }
        /*---Getters---*/
        static int getStatic(){ return marked_cells; }
        int getSize() const{ return board_size; }
        int get_pastx() const{ return past_x; }
        char get_pasty() const{ return past_y; }
        int getGameStatus() const{ return game_status; }
        int getGameMode() const{ return game_mode; }
        std::string getName() const{ return name; }
        /*---Overloaded Operators---*/
        Hex& operator--();
        Hex operator--(int);
        Hex& operator=(const Hex& other);
        friend bool operator==(const Hex& obj1, const Hex& obj2);
        friend std::ostream& operator<<(std::ostream& outputStream, Hex& game);
        friend std::ofstream& operator<<(std::ofstream& outputStream, const Hex& game);
        friend std::ifstream& operator>>(std::ifstream& outputStream, Hex& game);
    private:
        Cell **hexCells;
        past_coordinates *p1_past_coordinates, *p2_past_coordinates, *all_plays, *score_coordinates;
};
#endif