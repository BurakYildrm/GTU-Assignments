#ifndef hex
#define hex
enum cell_state{Player1 = 'x', Player2 = 'o', Empty = '.'};
bool isEmpty(char board[12][12], int x, char y);
bool isFull(char board[12][12], int board_size);
bool isConnected(char board[12][12], cell_state player, int board_size);
bool isWinner_P1(char board[12][12], int board_size, cell_state player, int x, int y, int rotation, int past_coordinates_x[73], int past_coordinates_y[73]);
bool isWinner_P2(char board[12][12], int board_size, cell_state player, int x, int y, int rotation, int past_coordinates_x[73], int past_coordinates_y[73]);
void print_board(char board[12][12], int board_size);
void find_beginning(char board[12][12], int board_size, cell_state player, int coordinates[13]);
int sizeofarray(int coordinates[]);
bool check_coordinates(int past_coordinates_x[73], int past_coordinates_y[73], int x, int y);
int block_player(char board[12][12], int x, int y);
void find_cell(char board[12][12], int board_size, int y, int* cell_x, int* cell_y);
void load_game(char board[12][12], int& board_size, int& game_mode, int& player_turn, std::string filename = "file.txt");
void save_game(const char board[12][12], const int& board_size, const int& game_mode, const int& player_turn, std::string filename = "file.txt");
#endif