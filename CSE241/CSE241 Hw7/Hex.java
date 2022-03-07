import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;
import java.awt.Color;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseListener;
import java.awt.GridLayout;

import javax.swing.BoxLayout;
import javax.swing.ButtonGroup;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.SwingUtilities;
import javax.swing.border.LineBorder;

enum cell_state{
    PLAYER1('x'), PLAYER2('o'), EMPTY('.');
    private final char value;
    private cell_state(char value) {
        this.value = value;
    }
    public char getValue() {
        return value;
    } 
}
class past_coordinates{
    private int x, y;
    public void setx(int a){ x = a; }
    public void sety(int b){ y = b; }
    public int getx(){ return x; }
    public int gety(){ return y; }
    public past_coordinates(int a, int b){
        x = a;
        y = b;
    }
    public past_coordinates(){
        this(0, 0);
    }
}
class Cell{
    public Cell(){
        row = 0;
        column = 0;
        state = cell_state.EMPTY;
    }
    public Cell(int r, int c, cell_state s){
        row = r;
        column = c;
        state = s;
    }
    public void setRow(int a){ row = a; }
    public void setColumn(int b){ column = b; }
    public void setState(cell_state c){ state = c; }
    public int getRow(){ return row; }
    public int getColumn(){ return column; }
    public cell_state getState(){ return state; }
    private int row, column;
    private cell_state state;
}
public class Hex extends JPanel implements ActionListener{
    private int checkLine = 0, checkOrder = 0;
    private JButton[][] grid;
    private JFrame frame;
    private JLabel label;
    private JButton button;
    private JTextArea textArea;
    private void setStatic(int a){ marked_cells += a; }
    private String name;
    private int past_x, count;
    private char past_y;
    private ArrayList<Integer> p1_first_coordinates, p2_first_coordinates, connectedCells_storage;
    private static int marked_cells = 0;
    private static int connectedCells = 1;
    private int board_size, game_mode, game_status, isloaded, turn;
    private int size;
    private ArrayList<ArrayList<Cell> > hexCells;
    private ArrayList<past_coordinates> p1_past_coordinates, p2_past_coordinates, all_plays, score_coordinates;
    /*I made these functions private because client shouldn't be able to change the size and the static value outside the playGame function*/
    private void setSize(int x){
        board_size = x;
        hexCells = new ArrayList<>();
        for(int i = 0; i < board_size; i++){
            hexCells.add(i, new ArrayList<>());
        }
        for(int i = 0; i < board_size; i++){
            for(int j = 0; j < board_size; j++){
                hexCells.get(i).add(new Cell(i, j, cell_state.EMPTY));
            }
        }
    }
    private void setSize(){
        JFrame temp = new JFrame();
        JPanel temp_panel = new JPanel();
        JButton temp_button = new JButton("OK");
        JTextField temp_field = new JTextField(20);
        JLabel temp_label = new JLabel("Enter the size of hex game");
        /*StackTraceElement[] stacktrace = Thread.currentThread().getStackTrace();
        StackTraceElement st = stacktrace[2];
        String methodName = st.getMethodName();*/
        class event implements ActionListener{
            @Override
            public void actionPerformed(ActionEvent e) {
                boolean invalid = false, small = false;
                try{
                    String input = temp_field.getText();
                    size = Integer.parseInt(input);
                    if(size < 5) small = true;
                }
                catch(NumberFormatException exep){
                    invalid = true;
                }
                if(small){
                    JOptionPane.showMessageDialog(null, "Please enter an integer bigger than 5", "Size is not enough", JOptionPane.WARNING_MESSAGE);
                }
                else if(!invalid){
                    board_size = size;
                    temp.dispose();
                    checkLine = Thread.currentThread().getStackTrace()[1].getLineNumber() + 1;
                    setGameMode();
                    hexCells = new ArrayList<>();
                    for(int i = 0; i < board_size; i++){
                        hexCells.add(i, new ArrayList<>());
                    }
                    for(int i = 0; i < board_size; i++){
                        for(int j = 0; j < board_size; j++){
                            hexCells.get(i).add(new Cell(i, j, cell_state.EMPTY));
                        }
                    }
                    checkOrder++;
                }
                else{
                    JOptionPane.showMessageDialog(null, "please enter an integer", "invalid input", JOptionPane.WARNING_MESSAGE);
                }
            }
        }
        temp.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        temp.setSize(220,120);
        temp_panel.setLayout(null);
        event e = new event();
        temp_label.setBounds(20, 0, 200, 30);
        temp_field.setBounds(0, 40, 140, 30);
        temp_button.setBounds(150, 40, 50, 30);
        temp_button.addActionListener(e);
        temp_panel.add(temp_label);
        temp_panel.add(temp_field); 
        temp_panel.add(temp_button);
        temp.add(temp_panel);
        temp.setVisible(true);  
    }
    class load implements ActionListener{
        @Override
        public void actionPerformed(ActionEvent e) {

        }
    }
    private void setGameMode(){
        JFrame temp = new JFrame("Select the game mode");
        JPanel temp_panel = new JPanel();
        JRadioButton temp_button1 = new JRadioButton("player vs player");
        JRadioButton temp_button2 = new JRadioButton("player vs computer");
        JButton temp_button_ok = new JButton("OK");
        ButtonGroup group = new ButtonGroup();
        StackTraceElement[] stacktrace = Thread.currentThread().getStackTrace();
        StackTraceElement st = stacktrace[2];
        class event implements ActionListener{
            @Override
            public void actionPerformed(ActionEvent e) {
                if(temp_button1.isSelected()) game_mode = 1;
                if(temp_button1.isSelected()) game_mode = 2;
                temp.dispose();
                if(st.getLineNumber() == checkLine) setFrame();
                checkOrder++;
            }
        }
        temp.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        temp.setSize(250,120);
        temp_panel.setLayout(null);
        temp_button1.setBounds(0, 0, 250, 30);
        temp_button2.setBounds(0, 30, 250, 30);
        temp_button_ok.setBounds(100, 60, 50, 30);
        event e = new event();
        temp_button_ok.addActionListener(e);
        group.add(temp_button1);
        group.add(temp_button2);
        group.add(temp_button_ok);
        temp_panel.add(temp_button1);
        temp_panel.add(temp_button2);
        temp_panel.add(temp_button_ok);
        temp.add(temp_panel);
        temp.setVisible(true);
    }
    private void setFrame(){
        setLayout(null);
        grid = new JButton[board_size][board_size];
        for(int i = 0; i < board_size; i++){
            for(int j = 0, k; j < board_size; j++){
                k = 20*j;
                grid[i][j] = new JButton();
                grid[i][j].setBorder(new LineBorder(Color.BLACK));
                grid[i][j].setBounds((i*40) + k, j*40, 40, 40);
                grid[i][j].setOpaque(true);
                grid[i][j].addActionListener(Hex.this);
                add(grid[i][j]);
            }
        }
        setSize(board_size*60 + 100, board_size*47 + 30);
        //JPanel container = new JPanel();
        //container.setLayout(new BoxLayout(container, BoxLayout.X_AXIS));
        //container.add(this);
        //JPanel panel2 = new JPanel();
        class loadEvent implements ActionListener{
            @Override
            public void actionPerformed(ActionEvent e) {
                JFrame temp = new JFrame("Enter name of the file");
                JPanel temp_panel = new JPanel();
                JButton temp_button = new JButton("OK");
                JTextField temp_field = new JTextField(20);
                class event implements ActionListener{
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        String s = temp_field.getText();
                        try{
                            load_game(s);
                            temp.dispose();
                            frame.dispose();
                            setFrame();
                        }
                        catch(FileNotFoundException exception){
                            JOptionPane.showMessageDialog(null, "There is no such file", "invalid file", JOptionPane.ERROR_MESSAGE);
                        }
                    }
                }
                temp.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
                temp.setSize(250,90);
                temp_panel.setLayout(null);
                temp_field.setBounds(10, 10, 190, 30);
                temp_button.setBounds(200, 10, 50, 30);
                temp_button.addActionListener(new event());
                temp_panel.add(temp_field);
                temp_panel.add(temp_button);
                temp.add(temp_panel);
                temp.setVisible(true);
            }
        }
        class saveEvent implements ActionListener{
            @Override
            public void actionPerformed(ActionEvent e) {
                JFrame temp = new JFrame("Enter name of the file");
                JPanel temp_panel = new JPanel();
                JButton temp_button = new JButton("OK");
                JTextField temp_field = new JTextField(20);
                class event implements ActionListener{
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        String s = temp_field.getText();
                        save_game(s);
                        temp.dispose();
                        JOptionPane.showConfirmDialog(null, "Your game is saved to " + s);
                    }
                }
                temp.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
                temp.setSize(250,90);
                temp_panel.setLayout(null);
                temp_field.setBounds(10, 10, 190, 30);
                temp_button.setBounds(200, 10, 50, 30);
                temp_button.addActionListener(new event());
                temp_panel.add(temp_field);
                temp_panel.add(temp_button);
                temp.add(temp_panel);
                temp.setVisible(true);
            }
        }
        class undoEvent implements ActionListener{
            @Override
            public void actionPerformed(ActionEvent e) {
                // TODO Auto-generated method stub

            }
        }
        class newGameEvent implements ActionListener{

            @Override
            public void actionPerformed(ActionEvent e) {
                // TODO Auto-generated method stub

            }
            
        }
        JButton load = new JButton("LOAD");
        load.addActionListener(new loadEvent());
        load.setBounds(board_size*60 + 20, 0, 80, 40);
        JButton save = new JButton("SAVE");
        save.addActionListener(new saveEvent());
        save.setBounds(board_size*60 + 20, 40, 80, 40);
        JButton undo = new JButton("UNDO");
        undo.addActionListener(new undoEvent());
        undo.setBounds(board_size*60 + 20, 80, 80, 40);
        JButton newGame = new JButton("NEW GAME");
        newGame.addActionListener(new newGameEvent()); 
        newGame.setBounds(board_size*60 + 20, 120, 80, 40);
        add(load);
        add(save);
        add(undo);
        add(newGame);
        frame = new JFrame();
        frame.setSize(board_size*60 + 100,board_size*47 + 30);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.add(this);
        frame.setVisible(true);
    }
    public Hex(){
        game_mode = 0;
        game_status = 0;
        isloaded = 0;
        p1_first_coordinates = new ArrayList<>();
        p1_past_coordinates = new ArrayList<>();
        p2_first_coordinates = new ArrayList<>();
        p2_past_coordinates = new ArrayList<>();
        all_plays = new ArrayList<>();
        score_coordinates = new ArrayList<>();
        setSize();
    }
    /*public Hex(Hex other){
        past_x = other.past_x;
        past_y = other.past_y;
        name = other.name;
        game_mode = other.game_mode;
        game_status = other.game_status;
        isloaded = other.isloaded;
        p1_first_coordinates = new ArrayList<>(other.p1_first_coordinates);
        p1_past_coordinates = new ArrayList<>(other.p1_past_coordinates);
        p2_first_coordinates = new ArrayList<>(other.p2_first_coordinates);
        p2_past_coordinates = new ArrayList<>(other.p2_past_coordinates);
        all_plays = new ArrayList<>(other.all_plays);
        score_coordinates = new ArrayList<>(other.score_coordinates);
        turn = other.turn;
        hexCells = new ArrayList<>(other.hexCells);
        board_size = other.board_size;
        frame = (JFrame) SwingUtilities.getWindowAncestor(other.frame);
    }*/
    public Object clone() throws CloneNotSupportedException{
        Hex temp = (Hex)super.clone();
        temp.p1_first_coordinates = new ArrayList<>(this.p1_first_coordinates);
        temp.p1_past_coordinates = new ArrayList<>(this.p1_past_coordinates);
        temp.hexCells = new ArrayList<>(this.hexCells);
        temp.name = new String(this.name);
        temp.p1_past_coordinates = new ArrayList<>(this.p1_past_coordinates);
        temp.p2_past_coordinates = new ArrayList<>(this.p1_past_coordinates);
        temp.all_plays = new ArrayList<>(this.all_plays);
        temp.score_coordinates = new ArrayList<>(this.score_coordinates);
        return temp;
    }
    public Cell play(){
        int x = 0, y = 0;
        setStatic(1);
        Cell temp;
        if(check_cell_state(get_pastx(), (char)(get_pasty() + 1)) == true){
            temp = new Cell(get_pastx() - 1, (int)(get_pasty()) - 96, cell_state.PLAYER2);
            all_plays.add(new past_coordinates(get_pastx() - 1, (int)(get_pasty()) - 96));
            System.out.println("\nComputer's move: " + (char)(get_pasty() + 1) + get_pastx());
            return temp;
        }
        if(check_cell_state(get_pastx() - 1, (char)(get_pasty() + 1)) == true){
            temp = new Cell(get_pastx() - 2, (int)(get_pasty()) - 96, cell_state.PLAYER2);
            all_plays.add(new past_coordinates(get_pastx() - 2, (int)(get_pasty()) - 96));
            System.out.println("\nComputer's move: " + (char)(get_pasty() + 1) + (get_pastx() - 1));
            return temp;
        }
        if(check_cell_state(get_pastx() + 1, get_pasty()) == true){
            temp = new Cell(get_pastx(), (int)(get_pasty()) - 97, cell_state.PLAYER2);
            all_plays.add(new past_coordinates(get_pastx(), (int)(get_pasty()) - 97));
            System.out.println("\nComputer's move: " + get_pasty() + (get_pastx() + 1));
            return temp;
        }
        if(check_cell_state(get_pastx() - 1, get_pasty()) == true){
            temp = new Cell(get_pastx() - 2, (int)(get_pasty()) - 97, cell_state.PLAYER2);
            all_plays.add(new past_coordinates(get_pastx() - 2, (int)(get_pasty()) - 97));
            System.out.println("\nComputer's move: " + get_pasty() + (get_pastx() - 1));
            return temp;
        }
        if(check_cell_state(get_pastx() + 1, (char)(get_pasty() - 1)) == true){
            temp = new Cell(get_pastx(), (int)(get_pasty()) - 98, cell_state.PLAYER2);
            all_plays.add(new past_coordinates(get_pastx(), (int)(get_pasty()) - 98));
            System.out.println("\nComputer's move: " + (char)(get_pasty() - 1) + (get_pastx() + 1));
            return temp;
        }
        if(check_cell_state(get_pastx(), (char)(get_pasty() - 1)) == true){
            temp = new Cell(get_pastx() - 1, (int)(get_pasty()) - 98, cell_state.PLAYER2);
            all_plays.add(new past_coordinates(get_pastx() - 1, (int)(get_pasty()) - 98));
            System.out.println("\nComputer's move: " + (char)(get_pasty() - 1) + get_pastx());
            return temp;
        }
        else{
            for(int i = 0; i < Size(); i++){
                for(int j = 0; j < Size(); j++){
                    if(check_cell_state(i, (char)(j + 97)) == true){
                        x = i;
                        y = j;
                        all_plays.add(new past_coordinates(i, j));
                        System.out.println("\nComputer's move: " + (char)(j + 97) + (i + 1));
                    }
                }
            }
            return new Cell(x, y, cell_state.PLAYER2);
        }
    }
    /*public void play(Cell coordinates){
        hexCells.get(coordinates.getRow()).get(coordinates.getColumn()).setState(coordinates.getState());
        all_plays.add(new past_coordinates(coordinates.getRow(), coordinates.getColumn()));
        setStatic(1);
    }*/
    public void actionPerformed(ActionEvent e) {
        int i , j, k = 20;
        JButton button = (JButton)e.getSource();
        i = button.getY()/40;
        j = (button.getX() - k*i)/40;
        if(count == 0){
            count++;
            button.setBackground(Color.RED);
            hexCells.get(i).get(j).setState(cell_state.PLAYER1);
            //System.out.println(i + " " + j);
        }
        else{
            count--;
            button.setBackground(Color.BLUE);
            hexCells.get(i).get(j).setState(cell_state.PLAYER2);
            //System.out.println(i + " " + j);
        }
        button.setEnabled(false);
    }
    public void playGame(){
        Cell temp;
        String input;
        boolean file = false, invalid = false;
        int x = 0, s = 0;
        char y = 0;
        Scanner scn = new Scanner(System.in);
        for(int i = 0, main = 0, undo = 0; getGameStatus() == 0 && main == 0; i++){
            if(isloaded == 1 && i == 0){
                i = turn;
            }
            else{
                turn = i;
            }
            if(game_mode == 1){
                undo = 0;
                file = false;
                print();
                do{
                    invalid = false;
                    if(i%2 == 0){
                        System.out.printf("\nPlayer1's move(use lowercase letters): ");
                    }
                    else{
                        System.out.printf("\nPlayer2's move(use lowercase letters): ");   
                    }
                    scn = new Scanner(System.in);
                    input = scn.nextLine();
                    if(input.contains("LOAD")){//loads the given file
                        try{
                            load_game(input.substring(5, input.length()));
                        }
                        catch(FileNotFoundException exception){
                            System.out.println(exception);
                        } 
                        i--;
                        file = true;
                        break;
                    }
                    else if(input.contains("SAVE")){//saves to given file
                        save_game(input.substring(5, input.length()));
                        i--;
                        file = true;
                        break;
                    }
                    else if(input.equals("MAIN")){//returns to main menu
                        main = 1;
                        break;
                    }
                    else if(input.equals("UNDO")){
                        undo = 1;
                        if(all_plays.size() == 0){
                            System.out.println("There is no move left to undo");
                        }
                        else{
                            delete_last_element();
                        }
                        break;
                    }
                    else if((input.charAt(0) >= 'a' && input.charAt(0) <= (char)(Size() + 96)) && input.length() >= 2){//checks if the input is valid
                        if((int)(input.charAt(1)) - 48 >= 1 && (int)(input.charAt(1)) - 48 <= 9){
                            x = Integer.parseInt(input.substring(1, input.length()));
                            y = input.charAt(0);
                        }
                    }
                    else{
                        invalid = true;//if none of the above is satisfied then function marks the input as invalid
                    }
                }while(check_cell_state(x, y) == false || invalid == true);
                if(!file && main == 0 && undo == 0){
                    if(i%2 == 0){//player turn
                        temp = new Cell(x - 1, (int)(y) - 97, cell_state.PLAYER1);
                    }
                    else{
                        temp = new Cell(x - 1, (int)(y) - 97, cell_state.PLAYER2);
                    }
                    if(i%2 == 0){
                        if(isConnected(cell_state.PLAYER1) == true){
                            find_beginning(cell_state.PLAYER1);
                            for(int j = 0; j < p1_first_coordinates.size(); j++){
                                if(isWinner_P1(p1_first_coordinates.get(j), 0, 0) == true){
                                    print();
                                    System.out.println("\nPLAYER1 WINS!!!");
                                    setStatic(count_marked_cells()*-1);
                                    game_status = 1;
                                }
                                p1_past_coordinates = new ArrayList<>();
                            }
                        }
                    }
                    else{
                        if(isConnected(cell_state.PLAYER2) == true){
                            find_beginning(cell_state.PLAYER2);
                            for(int j = 0; j < p2_first_coordinates.size(); j++){
                                if(isWinner_P2(0, p2_first_coordinates.get(j), 0) == true){
                                    print();
                                    System.out.println("\nPLAYER2 WINS!!!");
                                    setStatic(count_marked_cells()*-1);
                                    game_status = 1;
                                }
                                p2_past_coordinates = new ArrayList<>();
                            }
                        }
                    }
                }
            }
            else{
                if(i%2 == 1){
                    if(!file && undo == 0){
                        print();
                        temp = play();
                        hexCells.get(temp.getRow()).get(temp.getColumn()).setState(temp.getState());
                    }
                    file = false;
                }
                else{
                    print();
                    file = false;
                    undo = 0;
                    do{
                        invalid = false;
                        System.out.printf("\nPlayer's move(use lowercase letters): ");
                        scn = new Scanner(System.in);
                        input = scn.nextLine();
                        if(input.contains("LOAD")){
                            try{
                                load_game(input.substring(5, input.length()));
                            }
                            catch(FileNotFoundException exception){
                                System.out.println(exception);
                            } 
                            i--;
                            file = true;
                            break;
                        }
                        else if(input.contains("SAVE")){
                            save_game(input.substring(5, input.length()));
                            i--;
                            file = true;
                            break;
                        }
                        else if(input.equals("MAIN")){
                            main = 1;
                            break;
                        }
                        else if(input.equals("UNDO")){
                            undo = 1;
                            if(all_plays.size() == 0){
                                System.out.println("There is no move left to undo");
                            }
                            else{
                                delete_last_element();
                                delete_last_element();
                            }
                            break;
                        }
                        else if((input.charAt(0) >= 'a' && input.charAt(0) <= (char)(Size() + 96)) && input.length() >= 2){
                            if((int)(input.charAt(1)) - 48 >= 1 && (int)(input.charAt(1)) - 48 <= 9){
                                x = Integer.parseInt(input.substring(1, input.length()));
                                y = input.charAt(0);
                            }
                        }
                        else{
                            invalid = true;
                        }
                    }while(check_cell_state(x, y) == false || invalid == true);
                    if(!file && main == 0 && undo == 0){
                        past_x = x;//sets the past coordinates for play function to use
                        past_y = y;
                    }
                }
                if(!file && main == 0 && undo == 0){
                    if(i%2 == 0){
                        if(isConnected(cell_state.PLAYER1) == true){
                            find_beginning(cell_state.PLAYER1);
                            for(int j = 0; j < p1_first_coordinates.size(); j++){
                                if(isWinner_P1(p1_first_coordinates.get(j), 0, 0) == true){
                                    print();
                                    System.out.println("\nPLAYER WINS!!!");
                                    setStatic(count_marked_cells()*-1);
                                    game_status = 1;
                                }
                                p1_past_coordinates = new ArrayList<>();
                            }
                        }
                    }
                    else{
                        if(isConnected(cell_state.PLAYER2) == true){
                            find_beginning(cell_state.PLAYER2);
                            for(int j = 0; j < p2_first_coordinates.size(); j++){
                                if(isWinner_P2(0, p2_first_coordinates.get(j), 0) == true){
                                    print();
                                    System.out.println("\nCOMPUTER WINS!!!");
                                    setStatic(count_marked_cells()*-1);
                                    game_status = 1;
                                }
                                p2_past_coordinates = new ArrayList<>();
                            }
                        }
                    }
                }
            }
        }
        scn.close();
    }
    public boolean check_cell_state(int x, char y){
        if(x - 1 >= 0 && x - 1 <= Size() - 1 && (int)(y) - 97 >= 0 && (int)(y) - 97 <= Size() - 1){
            if(hexCells.get(x - 1).get((int)(y) - 97).getState() == cell_state.EMPTY){
                return true;
            }
        }
        return false;
    }
    public int count_marked_cells(){
        int counter = 0;
        for(int i = 0; i < board_size; i++){
            for(int j = 0; j < board_size; j++){  
                if(hexCells.get(i).get(j).getState() != cell_state.EMPTY){
                    counter++;
                }
            }
        }
        return counter;
    }
    public void load_game(String filename) throws FileNotFoundException {
        int marked_cells;
        File file = new File(filename);
        try{
            Scanner sc = new Scanner(file); 
            String s;
            s = sc.nextLine();
            setSize(Integer.parseInt(s));
            s = sc.nextLine();
            game_mode = Integer.parseInt(s);
            s = sc.nextLine();
            turn = Integer.parseInt(s) - 1;
            s = sc.nextLine();
            for(int i = 0, k = 0; i < Size(); i++){
                for(int j = 0; j < Size(); j++, k++){
                    if(s.charAt(k) == 'x') hexCells.get(i).get(j).setState(cell_state.PLAYER1);
                    if(s.charAt(k) == 'o') hexCells.get(i).get(j).setState(cell_state.PLAYER2);
                    if(s.charAt(k) == '.') hexCells.get(i).get(j).setState(cell_state.EMPTY);
                }
            }
            marked_cells = count_marked_cells();
            setStatic(count_marked_cells() - marked_cells);
            sc.close();
            }
        catch(FileNotFoundException excep){
            throw excep;
        }
    }
    public void save_game(String filename){
        FileWriter file;
        try{
           file = new FileWriter(filename);
           file.write(Size() + "\n" + game_mode + "\n" + (turn%2 + 1) + "\n");
           for(int i = 0; i < Size(); i++){
                for(int j = 0; j < Size(); j++){
                    file.write(hexCells.get(i).get(j).getState().getValue());
                }
            }
           file.close();
        }
        catch(IOException e){
            e.printStackTrace();
        }
    }
    public boolean isWinner_P1(int x, int y, int rotation){
        if(hexCells.get(x).get(y).getState() != cell_state.PLAYER1){
            return false;
        }
        else if(hexCells.get(x).get(y).getState() == cell_state.PLAYER1 && y == Size() - 1){
            return true;
        }
        else{
            //function determines the coordinates of the previous node by finding the opposite rotation of current one and records these coordinates so that function doesn't check the same coordinates over and over again and this prevents the function from getting into an infinite loop
            if(rotation == 1){
                p1_past_coordinates.add(new past_coordinates(x + 1, y));
            }
            if(rotation == 2){
                p1_past_coordinates.add(new past_coordinates(x + 1, y - 1));
            }
            if(rotation == 3){
                p1_past_coordinates.add(new past_coordinates(x, y - 1));
            }
            if(rotation == 4){
                p1_past_coordinates.add(new past_coordinates(x - 1, y));
            }
            if(rotation == 5){
                p1_past_coordinates.add(new past_coordinates(x - 1, y + 1));
            }
            if(rotation == 6){
                p1_past_coordinates.add(new past_coordinates(x, y + 1));
            }
            //rotations starts from up and goes on clockwise
            //if isWinner_P1 is true it uppercases the current node so that only the winning path is uppercased 
            if(x - 1 >= 0 && check_coordinates(x - 1, y, cell_state.PLAYER1) == true){
                rotation = 1;
                if(isWinner_P1(x - 1, y, 1) == true){
                    return true;
                }
            }
            if(x - 1 >= 0 && y + 1 <= Size() - 1 && check_coordinates(x - 1, y + 1, cell_state.PLAYER1) == true){
                rotation = 2;
                if(isWinner_P1(x - 1, y + 1, 2) == true){
                    return true;
                }
            }
            if(y + 1 <= Size() - 1 && check_coordinates(x, y + 1, cell_state.PLAYER1) == true){
                rotation = 3;
                if(isWinner_P1(x, y + 1, 3) == true){
                    return true;
                }
            }
            if(x + 1 <= Size() - 1 && check_coordinates(x + 1, y, cell_state.PLAYER1) == true){
                rotation = 4;
                if(isWinner_P1(x + 1, y, 4) == true){
                    return true;
                }
            }
            if(x + 1 <= Size() - 1 && y - 1 >= 0 && check_coordinates(x + 1, y - 1, cell_state.PLAYER1) == true){
                rotation = 5;
                if(isWinner_P1(x + 1, y - 1, 5) == true){
                    return true;
                }
            }
            if(y - 1 >= 0 && check_coordinates(x, y - 1, cell_state.PLAYER1) == true){
                rotation = 6;
                if(isWinner_P1(x, y - 1, 6) == true){
                    return true;
                }
            }
            return false;
        }
    }
    public boolean isWinner_P2(int x, int y, int rotation){
        if(hexCells.get(x).get(y).getState() != cell_state.PLAYER2){
            return false;
        }
        else if(hexCells.get(x).get(y).getState() == cell_state.PLAYER2 && x == Size() - 1){
            return true;
        }
        else{
            //function determines the coordinates of the previous node by finding the opposite rotation of current one and records these coordinates so that function doesn't check the same coordinates over and over again and this prevents the function from getting into an infinite loop
            if(rotation == 1){
                p2_past_coordinates.add(new past_coordinates(x, y - 1));
            }
            if(rotation == 2){
                p2_past_coordinates.add(new past_coordinates(x - 1, y));
            }
            if(rotation == 3){
                p2_past_coordinates.add(new past_coordinates(x - 1, y + 1));
            }
            if(rotation == 4){
                p2_past_coordinates.add(new past_coordinates(x, y + 1));
            }
            if(rotation == 5){
                p2_past_coordinates.add(new past_coordinates(x + 1, y));
            }
            if(rotation == 6){
                p2_past_coordinates.add(new past_coordinates(x + 1, y - 1));
            }
            //rotations starts from right and goes on clockwise
            //if isWinner_P2 is true it uppercases the current node so that only the winning path is uppercased 
            if(y + 1 <= Size() - 1 && check_coordinates(x, y + 1, cell_state.PLAYER2) == true){
                rotation = 1;
                if(isWinner_P2(x, y + 1, 1) == true){
                    return true;
                }
            }
            if(x + 1 <= Size() - 1 && check_coordinates(x + 1, y, cell_state.PLAYER2) == true){
                rotation = 2;
                if(isWinner_P2(x + 1, y, 2) == true){
                    return true;
                }
            }
            if(x + 1 <= Size() - 1 && y - 1 >= 0 && check_coordinates(x + 1, y - 1, cell_state.PLAYER2) == true){
                rotation = 3;
                if(isWinner_P2(x + 1, y - 1, 3) == true){
                    return true;
                }
            }
            if(y - 1 >= 0 && check_coordinates(x, y - 1, cell_state.PLAYER2) == true){
                rotation = 4;
                if(isWinner_P2(x, y - 1, 4) == true){
                    return true;
                }
            }
            if(x - 1 >= 0 && check_coordinates(x - 1, y, cell_state.PLAYER2) == true){
                rotation = 5;
                if(isWinner_P2(x - 1, y, 5) == true){
                    return true;
                }
            }
            if(x - 1 >= 0 && y + 1 <= Size() - 1 && check_coordinates(x - 1, y + 1, cell_state.PLAYER2) == true){
                rotation = 6;
                if(isWinner_P2(x - 1, y + 1, 6) == true){
                    return true;
                }
            }
            return false;
        }
    }
    public boolean check_coordinates(int x, int y, cell_state player){
        if(player == cell_state.PLAYER1){
            for(int i = 0; i < p1_past_coordinates.size(); i++){
                if(p1_past_coordinates.get(i).getx() == x && p1_past_coordinates.get(i).gety() == y){
                    return false;
                }
            }
        }
        else{
            for(int i = 0; i < p2_past_coordinates.size(); i++){
                if(p2_past_coordinates.get(i).getx() == x && p2_past_coordinates.get(i).gety() == y){
                    return false;
                }
            }
        }
        return true;
    }
    public boolean isConnected(cell_state player){
        int check1 = 0, check2 = 0;
        if(player == cell_state.PLAYER1){
            for(int i = 0; i < Size() && check1 == 0; i++){
                if(hexCells.get(i).get(0).getState() == player){
                    check1++;
                }
            }
            for(int i = 0; i < Size() && check2 == 0; i++){
                if(hexCells.get(i).get(Size() - 1).getState() == player){
                    check2++;
                }
            }
        }
        else{
            for(int i = 0; i < Size() && check1 == 0; i++){
                if(hexCells.get(0).get(i).getState() == player){
                    check1++;
                }
            }
            for(int i = 0; i < Size() && check2 == 0; i++){
                if(hexCells.get(Size() - 1).get(i).getState() == player){
                    check2++;
                }
            }
        }
        if(check1 == 1 && check2 == 1){
            return true;
        } 
        else{
            return false;
        }
    }
    public void find_beginning(cell_state player){
        if(player == cell_state.PLAYER1){
            for(int i = 0; i < Size(); i++){
                if(hexCells.get(i).get(0).getState() == player){
                    p1_first_coordinates.add(i);
                }
            }
        }
        else{
            for(int i = 0; i < Size(); i++){
                if(hexCells.get(0).get(i).getState() == player){
                    p2_first_coordinates.add(i);
                }
            }
        }
    }
    public void delete_last_element(){
        int temp_size = all_plays.size() - 1;
        ArrayList<past_coordinates> temp = new ArrayList<>();
        for(int i = 0; i < temp_size; i++){
            temp.add(new past_coordinates(all_plays.get(i).getx(), all_plays.get(i).gety()));
        }
        all_plays = new ArrayList<>();
        for(int i = 0; i < temp_size; i++){
            all_plays.add(new past_coordinates(temp.get(i).getx(), temp.get(i).gety()));
        }
    }
    public int score_of_player(cell_state player){
        int number_of_coordinates, max;
        find_occurances(player);
        number_of_coordinates = score_coordinates.size();
        if(number_of_coordinates == 0){
            return 0;
        }
        if(number_of_coordinates == 1){
            return 1;
        }
        connectedCells_storage = new ArrayList<>();
        for(int i = 0; i < number_of_coordinates; i++){
            for(int j = i + 1; j < number_of_coordinates; j++){
                if(player == cell_state.PLAYER1){
                    connected_cells_p1(score_coordinates.get(i).getx(), score_coordinates.get(i).gety(), 0, score_coordinates.get(j).getx(), score_coordinates.get(j).gety());
                }
                else{
                    connected_cells_p2(score_coordinates.get(i).getx(), score_coordinates.get(i).gety(), 0, score_coordinates.get(j).getx(), score_coordinates.get(j).gety());
                }
                connectedCells_storage.add(connectedCells);
                connectedCells = 1;
                if(player == cell_state.PLAYER1){
                    p1_past_coordinates = new ArrayList<>();
                }
                else{
                    p2_past_coordinates = new ArrayList<>();
                }
            }
        }
        max = connectedCells_storage.get(0);
        for(int i = 0; i < number_of_coordinates*(number_of_coordinates - 1)/2; i++){
            if(max < connectedCells_storage.get(i)){
                max = connectedCells_storage.get(i);
            }
        }
        return max;
    }
    public boolean connected_cells_p1(int x, int y, int rotation, int dest_x, int dest_y){
        if(x == dest_x && y == dest_y){
            return true;
        }
        else if(hexCells.get(x).get(y).getState() != cell_state.PLAYER1){
            return false;
        }
        else{
            if(rotation == 1){
                p1_past_coordinates.add(new past_coordinates(x + 1, y));
            }
            if(rotation == 2){
                p1_past_coordinates.add(new past_coordinates(x + 1, y - 1));
            }
            if(rotation == 3){
                p1_past_coordinates.add(new past_coordinates(x, y - 1));
            }
            if(rotation == 4){
                p1_past_coordinates.add(new past_coordinates(x - 1, y));
            }
            if(rotation == 5){
                p1_past_coordinates.add(new past_coordinates(x - 1, y + 1));
            }
            if(rotation == 6){
                p1_past_coordinates.add(new past_coordinates(x, y + 1));
            }
            if(x - 1 >= 0 && check_coordinates(x - 1, y, cell_state.PLAYER1) == true){
                rotation = 1;
                if(connected_cells_p1(x - 1, y, 1, dest_x, dest_y) == true){
                    return true;
                }
            }
            if(x - 1 >= 0 && y + 1 <= Size() - 1 && check_coordinates(x - 1, y + 1, cell_state.PLAYER1) == true){
                rotation = 2;
                if(connected_cells_p1(x - 1, y + 1, 2, dest_x, dest_y) == true){
                    connectedCells++;
                    return true;
                }
            }
            if(y + 1 <= Size() - 1 && check_coordinates(x, y + 1, cell_state.PLAYER1) == true){
                rotation = 3;
                if(connected_cells_p1(x, y + 1, 3, dest_x, dest_y) == true){
                    connectedCells++;
                    return true;
                }
            }
            if(x + 1 <= Size() - 1 && check_coordinates(x + 1, y, cell_state.PLAYER1) == true){
                rotation = 4;
                if(connected_cells_p1(x + 1, y, 4, dest_x, dest_y) == true){
                    return true;
                }
            }
            if(x + 1 <= Size() - 1 && y - 1 >= 0 && check_coordinates(x + 1, y - 1, cell_state.PLAYER1) == true){
                rotation = 5;
                if(connected_cells_p1(x + 1, y - 1, 5, dest_x, dest_y) == true){
                    return true;
                }
            }
            if(y - 1 >= 0 && check_coordinates(x, y - 1, cell_state.PLAYER1) == true){
                rotation = 6;
                if(connected_cells_p1(x, y - 1, 6, dest_x, dest_y) == true){
                    return true;
                }
            }
            return false;
        }
    }
    public boolean connected_cells_p2(int x, int y, int rotation, int dest_x, int dest_y){
        if(x == dest_x && y == dest_y){
            return true;
        }
        else if(hexCells.get(x).get(y).getState() != cell_state.PLAYER2){
            return false;
        }
        else{
            if(rotation == 1){
                p2_past_coordinates.add(new past_coordinates(x, y - 1));
            }
            if(rotation == 2){
                p2_past_coordinates.add(new past_coordinates(x - 1, y));
            }
            if(rotation == 3){
                p2_past_coordinates.add(new past_coordinates(x - 1, y + 1));
            }
            if(rotation == 4){
                p2_past_coordinates.add(new past_coordinates(x, y + 1));
            }
            if(rotation == 5){
                p2_past_coordinates.add(new past_coordinates(x + 1, y));
            }
            if(rotation == 6){
                p2_past_coordinates.add(new past_coordinates(x + 1, y - 1));
            }
            if(y + 1 <= Size() - 1 && check_coordinates(x, y + 1, cell_state.PLAYER2) == true){
                rotation = 1;
                if(connected_cells_p2(x, y + 1, 1, dest_x, dest_y) == true){
                    return true;
                }
            }
            if(x + 1 <= Size() - 1 && check_coordinates(x + 1, y, cell_state.PLAYER2) == true){
                rotation = 2;
                if(connected_cells_p2(x + 1, y, 2, dest_x, dest_y) == true){
                    connectedCells++;
                    return true;
                }
            }
            if(x + 1 <= Size() - 1 && y - 1 >= 0 && check_coordinates(x + 1, y - 1, cell_state.PLAYER2) == true){
                rotation = 3;
                if(connected_cells_p2(x + 1, y - 1, 3, dest_x, dest_y) == true){
                    connectedCells++;
                    return true;
                }
            }
            if(y - 1 >= 0 && check_coordinates(x, y - 1, cell_state.PLAYER2) == true){
                rotation = 4;
                if(connected_cells_p2(x, y - 1, 4, dest_x, dest_y) == true){
                    return true;
                }
            }
            if(x - 1 >= 0 && check_coordinates(x - 1, y, cell_state.PLAYER2) == true){
                rotation = 5;
                if(connected_cells_p2(x - 1, y, 5, dest_x, dest_y) == true){
                    return true;
                }
            }
            if(x - 1 >= 0 && y + 1 <= Size() - 1 && check_coordinates(x - 1, y + 1, cell_state.PLAYER2) == true){
                rotation = 6;
                if(connected_cells_p2(x - 1, y + 1, 6, dest_x, dest_y) == true){
                    return true;
                }
            }
            return false;
        }
    }
    public void find_occurances( cell_state player){
        for(int i = 0; i < Size(); i++){
            for(int j = 0; j < Size(); j++){
                if(hexCells.get(i).get(j).getState() == player){
                    score_coordinates.add(new past_coordinates(i, j));
                }
            }
        }
    }
    public boolean equals(Object other){
        if(other == null) return false;
        if(other.getClass() != this.getClass()) return false;
        Hex temp = (Hex)other;
        if(this.board_size != temp.board_size) return false;
        for(int i = 0; i < this.board_size; i++){
            for(int j = 0; j < this.board_size; j++){
                if(hexCells.get(i).get(j).getState() != temp.hexCells.get(i).get(j).getState()) return false;
            }
        }
        return true;
    }
    public void print(){
        System.out.printf("\n  ");
        for(int i = 97, k = 0; k < Size(); k++){
            System.out.printf("%c ", (char)(i++));
        }
        System.out.println();
        for(int i = 0; i < Size(); i++){
            System.out.printf("%d", i + 1);
            if(i >= 9){//to aline the rows
                for(int k = 1; k < i + 1; k++){
                    System.out.printf(" ");
                }
            }
            else{
                for(int k = 0; k < i + 1; k++){
                    System.out.printf(" ");
                }
            }
            for(int j = 0; j < Size(); j++){
                System.out.printf("%c ", hexCells.get(i).get(j).getState().getValue());
            }
            System.out.println();
        }
        if(game_mode == 1){
            System.out.printf("Score of player1: %d(maximum score is %d)\n", score_of_player(cell_state.PLAYER1), Size());
            score_coordinates = new ArrayList<>();
            System.out.printf("Score of player2: %d(maximum score is %d)\n", score_of_player(cell_state.PLAYER2), Size());
            score_coordinates = new ArrayList<>();
        }
        else{
            System.out.printf("Score of player: %d(maximum score is %d)\n", score_of_player(cell_state.PLAYER1), Size());
            score_coordinates = new ArrayList<>();
            System.out.printf("Score of bot: %d(maximum score is %d)\n", score_of_player(cell_state.PLAYER2), Size());
            score_coordinates = new ArrayList<>();
        }
    }
    public void setName(String n){ name = n; }
    public void setIsloaded(int var){ isloaded = var; }
    public static int getStatic(){ return marked_cells; }
    public int Size() { return board_size; }
    public int get_pastx() { return past_x; }
    public char get_pasty() { return past_y; }
    public int getGameStatus() { return game_status; }
    public int getGameMode() { return game_mode; }
    public String getName() { return name; }
}