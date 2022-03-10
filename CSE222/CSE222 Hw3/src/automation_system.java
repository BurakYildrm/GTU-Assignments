import java.io.File;
import java.io.FileNotFoundException;
import java.util.*;

public class automation_system implements requiredFields {
    private KWArraylist<administrator> administrators;
    private KWArraylist<branchEmployee> branchEmployees;
    private KWArraylist<customer> customers;
    private KWLinkedList<branch> branches;
    private int[][][] onlineWishList;
    private storeHouse StoreHouse;
    private request stocksChangeRequest;
    private final IDgenerator IDgeneratorC;
    private boolean test = false;

    public KWArraylist<administrator> getAdministrators(){
        return administrators;
    }

    public KWLinkedList<branch> getBranches(){
        return branches;
    }

    public KWArraylist<customer> getCustomers() {
        return customers;
    }

    public KWArraylist<branchEmployee> getBranchEmployees() {
        return branchEmployees;
    }

    public storeHouse getStoreHouse(){
        return StoreHouse;
    }

    public void setTest(boolean val){
        test = val;
    }

    public boolean getTest() {
        return test;
    }

    /**
     * @return stockChangeRequest
     */
    public request getStocksChangeRequest() {
        return stocksChangeRequest;
    }

    /**
     * @param stocksChangeRequest request object that current request will be set to
     */
    public void setStocksChangeRequest(request stocksChangeRequest) {
        this.stocksChangeRequest = stocksChangeRequest;
    }

    /**
     * @return online wishlist
     */
    public final int[][][] getOnlineWishList() {
        return onlineWishList;
    }

    /**
     * initializes the system
     */
    public automation_system() {
        user[] temp;
        branches = new KWLinkedList<>();
        administrators = new KWArraylist<>();
        branchEmployees = new KWArraylist<>();
        customers = new KWArraylist<>();
        onlineWishList = new int[modelNums.length][0][0];
        IDgeneratorC = new customerIDgenerator();
        stocksChangeRequest = new request();

        for(int i = 0; i < modelNums.length; i++){
            onlineWishList[i] = new int[modelNums[i]][colorNums[i]];
        }

        readBranchInfo(1, 0);
        readBranchInfo(1, 1);
        readBranchInfo(1, 2);
        readBranchInfo(1, 3);
        readBranchInfo(2, 1);

        temp = readUserInfo(0, "default_administrators.txt");
        for (user value : temp) {
            administrators.add((administrator) value);
        }

        temp = readUserInfo(1, "default_branch_employees.txt");
        for (user value : temp) {
            branchEmployees.add((branchEmployee) value);
        }
    }

    /**
     * returns the index of chosen branch in branches array
     * @param branchName name of the chosen branch
     * @return index of chosen branch in branches array
     */
    public int getBranchIndex(String branchName){
        int index = 0;
        for(branch i : branches){
            if(i.getName().equals(branchName)){
                break;
            }
            index++;
        }
        return index;
    }

    /**
     * returns the index of chosen product in productNames array
     * @param productName name of the chosen product
     * @return index of chosen product in productNames array
     */
    public int getNameIndex(String productName){
        if(productName.equals("-1"))
            return -1;
        int index;
        for(index = 0; index < productNames.length && !productName.equals(productNames[index]); index++);
        return index;
    }

    /**
     * checks if the given wishlist is empty
     * @param wishList wishlist which will be checked
     * @return true if the given wishlist is empty
     */
    public boolean isWishListEmpty(int[][][] wishList){
        for(int i = 0; i < modelNums.length; i++) {
            for (int j = 0; j < modelNums[i]; j++) {
                for (int k = 0; k < colorNums[i]; k++) {
                    if(wishList[i][j][k] != 0){
                        return false;
                    }
                }
            }
        }
        return true;
    }

    /**
     * empties the given wishlist
     * @param wishList wishlist which will be emptied
     */
    public void emptyWishlist(int[][][] wishList){
        for(int i = 0; i < modelNums.length; i++) {
            for (int j = 0; j < modelNums[i]; j++) {
                for (int k = 0; k < colorNums[i]; k++) {
                    wishList[i][j][k] = 0;
                }
            }
        }
    }

    /**
     * prints given wishlist
     * @param wishList wishlist which will be printed
     */
    public void seeWishList(int[][][] wishList){
        for(int i = 0; i < modelNums.length; i++){
            for(int j = 0; j < modelNums[i]; j++){
                for(int k = 0; k < colorNums[i]; k++){
                    if(wishList[i][j][k] != 0){
                        System.out.println("furniture = " + productNames[i] + " model = Model" +
                                (j + 1) + " color = Color" + (k + 1) + " need = " +
                                wishList[i][j][k]);
                    }
                }
            }
        }
    }

    /**
     * returns the branch employee whose ID is the given ID
     * @param ID ID of the wanted branch employee
     * @return branch employee whose ID is the given ID
     */
    public branchEmployee findBranchEmployee(String ID){
        for (branchEmployee i : branchEmployees) {
            if (i.getID().equals(ID)) {
                return i;
            }
        }
        throw new NoSuchElementException();
    }

    /**
     * make sures that user entered an integer
     * @param scanner scanner which will be used to get user input
     * @return integer that user entered
     */
    public int getValidInput(Scanner scanner){
        int input = -1;
        int check;
        do {
            check = 0;
            try {
                input = scanner.nextInt();
            } catch (InputMismatchException e) {
                System.out.println("Please enter an integer");
                scanner.nextLine();
                check = 1;
            }
        } while (check == 1);
        return input;
    }

    /**
     * make sures that user entered an integer between bottomBound and topBound
     * @param scanner scanner which will be used to get user input
     * @param bottomBound min value that user can enter
     * @param topBound max value that user can enter
     * @return integer that meets the given requirements
     */
    public int getValidInput(Scanner scanner, int bottomBound, int topBound){
        int input = -1;
        int check;
        do {
            check = 0;
            try {
                input = scanner.nextInt();
                if(input < bottomBound || input > topBound){
                    System.out.println("Please enter an integer between " + bottomBound + " and " + topBound);
                }
            }
            catch (InputMismatchException e){
                System.out.println("Please enter an integer between " + bottomBound + " and " + topBound);
                scanner.nextLine();
                check = 1;
            }
        } while (input < bottomBound || input > topBound || check == 1);
        return input;
    }

    /**
     * takes all the necessary info to add new customer and sends them to newCustomer method
     */
    public void register(){
        String name;
        String surname;
        String email;
        String password;
        Scanner scanner = new Scanner(System.in);
        System.out.println("NEW ACCOUNT");
        System.out.println("Enter your name: ");
        name = scanner.next();
        System.out.println("Enter your surname: ");
        surname = scanner.next();
        do {
            System.out.println("Enter your email: ");
            email = scanner.next();
        } while (!email.contains("@") || !email.contains(".com") || !uniqueEmail(email));
        System.out.println("Enter your password: ");
        password = scanner.next();
        newCustomer(name, surname, email, password);
    }

    /**
     * checks if the given email is used by another customer
     * @param email email which will be checked
     * @return true if email is not used by another customer
     */
    public boolean uniqueEmail(String email){
        for (customer i : customers) {
            if (i.getEmail().equals(email)) {
                System.out.println("A customer is already using this email");
                return false;
            }
        }
        return true;
    }

    /**
     * reduces the given amount of given product from store house's stocks
     * @param name name index of the product
     * @param model model number of the product
     * @param color color number of the product
     * @param num amount of chosen product which will be reduced from store house's stocks
     * @return true if the requested amount is greater than the stocks in store house
     */
    public boolean reduceFromStock(int name, int model, int color, int num){
        int initialStock = StoreHouse.getStockInfo(name, model, color);
        if(num > initialStock){
            StoreHouse.changeStock(name, model, color, 0);
            System.out.println("Due to stock issues right now we can only send you "
                    + initialStock + " of your ordered furniture but we will send the rest after resupplying the furniture");
            addToWishList(name, model, color, num - initialStock);
            return true;
        }
        else {
            StoreHouse.changeStock(name, model, color, initialStock - num);
        }
        return false;
    }

    /**
     * adds the certain amount of chosen product to online wishlist to resupply later
     * @param name name index of the product
     * @param model model number of the product
     * @param color color number of the product
     * @param num amount of chosen product needed
     */
    public void addToWishList(int name, int model, int color, int num){
        onlineWishList[name][model][color] += num;
    }

    /**
     * adds new customer to customers array
     * @param name name of the new customer
     * @param surname surname of the new customer
     * @param email email of the new customer
     * @param password password of the new customer
     */
    public void newCustomer(String name, String surname, String email, String password){
        customers.add(new customer(name, surname, email, password, IDgeneratorC.generateID(this)));
        System.out.println("Your ID is " + customers.get(customers.size() - 1).getID());
    }

    /**
     * returns the customer whose email and password matches the given email and password
     * @param email email of wanted customer
     * @param password password of wanted customer
     * @return customer who meets the requirements
     * @throws NoSuchElementException if there is no customer who meets the requirements
     */
    public customer findCustomer(String email, String password){
        for (customer i : customers) {
            if (i.getEmail().equals(email) && i.getPassword().equals(password)) {
                return i;
            }
        }
        throw new NoSuchElementException();
    }

    /**
     * returns the customer whose email and ID matches the given email and ID
     * @param email email of wanted customer
     * @param ID ID of wanted customer
     * @return customer who meets the requirements
     * @throws NoSuchElementException if there is no customer who meets the requirements
     */
    public customer findCustomerForSale(String email, String ID){
        for (customer i : customers) {
            if (i.getEmail().equals(email) && i.getID().equals(ID)) {
                return i;
            }
        }
        throw new NoSuchElementException();
    }

    /**
     * returns the administrator whose ID and password matches the given ID and password
     * @param ID ID of wanted administrator
     * @param password password of wanted administrator
     * @return administrator who meets the requirements
     * @throws NoSuchElementException if there is no administrator who meets the requirements
     */
    public administrator findAdministrator(String ID, String password){
        for (administrator i : administrators) {
            if (i.getID().equals(ID) && i.getPassword().equals(password)) {
                return i;
            }
        }
        throw new NoSuchElementException();
    }

    /**
     * returns the branch employee whose ID and password matches the given ID and password
     * @param ID ID of wanted branch employee
     * @param password password of wanted branch employee
     * @return branch employee who meets the requirements
     * @throws NoSuchElementException if there is no branch employee who meets the requirements
     */
    public branchEmployee findBranchEmployee(String ID, String password){
        for (branchEmployee i : branchEmployees) {
            if (i.getID().equals(ID) && i.getPassword().equals(password)) {
                return i;
            }
        }
        throw new NoSuchElementException();
    }

    /**
     * reads the default info about administrators and branch employees
     * @param mode decides whether the info will be written xxghhxghgxhxhxhgxhgxxgh
     * @param filepath filepath of the wanted file
     */
    public user[] readUserInfo(int mode, String filepath){
        user[] users = null;
        try {
            if(mode == 1)
                users = new user[DEFAULT_BRANCH_NUM*EMPLOYEE_PER_BRANCH];
            else
                users = new user[DEFAULT_ADMIN_NUM];
            String[] values;
            String s;
            File file = new File(filepath);
            Scanner scanner = new Scanner(file);
            for(int i = 0; scanner.hasNextLine(); i++){
                if(i >= users.length) break;
                s = scanner.nextLine();
                values = s.split("\\s");
                if(mode == 1){
                    if (i < 5) {
                        users[i] = new branchEmployee(values[0], values[1], values[2], values[3], branches.get(0));
                    }
                    else if (i < 10) {
                        users[i] = new branchEmployee(values[0], values[1], values[2], values[3], branches.get(1));
                    }
                    else if (i < 15) {
                        users[i] = new branchEmployee(values[0], values[1], values[2], values[3], branches.get(2));
                    }
                    else {
                        users[i] = new branchEmployee(values[0], values[1], values[2], values[3], branches.get(3));
                    }
                }
                else{
                    users[i] = new administrator(values[0], values[1], values[2], values[3]);
                }
            }
        }
        catch (FileNotFoundException e){
            e.printStackTrace();
            System.out.println("Source is not found");
        }
        return users;
    }

    /**
     * reads the default info about branches and store house
     * @param mode determines if the read info will be stored in branches array or in StoreHouse variable
     * @param docNum number of the branch
     */
    public void readBranchInfo(int mode, int docNum){
        File file;
        Scanner scanner;
        String line;
        String[] values;
        int[][] officeChairStock = new int[furniture.OFFICE_CHAIR_MODEL_NUM][furniture.OFFICE_CHAIR_COLOR_NUM];
        int[][] officeDeskStock = new int[furniture.OFFICE_DESK_MODEL_NUM][furniture.OFFICE_DESK_COLOR_NUM];
        int[][] meetingTableStock = new int[furniture.MEETING_TABLE_MODEL_NUM][furniture.MEETING_TABLE_COLOR_NUM];
        int[][] bookCaseStock = new int[furniture.BOOK_CASE_MODEL_NUM][furniture.BOOK_CASE_COLOR_NUM];
        int[][] officeCabinetStock = new int[furniture.OFFICE_CABINET_MODEL_NUM][furniture.OFFICE_CABINET_COLOR_NUM];

        try {
            if (mode == 1) {
                file = new File("default_branch" + (docNum + 1) + ".txt");
            }
            else {
                file = new File("default_store_house.txt");
            }
            scanner = new Scanner(file);
            for(int j = 0; j < modelNums.length; j++){
                scanner.nextLine();
                for(int k = 0; k < modelNums[j]; k++){
                    line = scanner.nextLine();
                    values = line.split("\\s");
                    for(int l = 0; l < values.length; l++) {
                        if (j == 0) {
                            officeChairStock[k][l] = Integer.parseInt(values[l]);
                        } else if (j == 1) {
                            officeDeskStock[k][l] = Integer.parseInt(values[l]);
                        } else if (j == 2) {
                            meetingTableStock[k][l] = Integer.parseInt(values[l]);
                        } else if (j == 3) {
                            bookCaseStock[k][l] = Integer.parseInt(values[l]);
                        } else {
                            officeCabinetStock[k][l] = Integer.parseInt(values[l]);
                        }
                    }
                }
            }
            if (mode == 1) {
                branches.add(new branch("branch" + (docNum + 1), officeChairStock, officeDeskStock, meetingTableStock, bookCaseStock, officeCabinetStock));
            }
            else {
                StoreHouse = new storeHouse(officeChairStock, officeDeskStock, meetingTableStock, bookCaseStock, officeCabinetStock);
            }
            scanner.close();
        }
        catch (Exception e){
            e.printStackTrace();
            System.out.println("Source is not found");
        }
    }

    public void runSystem(){
        Scanner scanner = new Scanner(System.in);
        int choiceMain = 0;
        while (choiceMain != 4) {
            System.out.println("WELCOME\nPlease choose a user type to login\n1)Customer\n2)Branch Employee\n3)Administrator\n4)Close the system");
            choiceMain = getValidInput(scanner, 1, 4);
            if (choiceMain == 1) {
                customer userC = null;
                int choice;
                int check;
                int onlineBranch;
                int modelNum;
                int colorNum;
                int number;
                long phone = 0;
                int furnitureName;
                String password;
                String email = "";
                String address;
                System.out.println("Options\n1)Signup\n2)Login");
                choice = getValidInput(scanner, 1, 2);
                if(choice == 2){
                    do {
                        check = 0;
                        System.out.println("LOGIN\nEnter your email(enter -1 to exit): ");
                        email = scanner.next();
                        if(email.equals(String.valueOf(-1))){
                            System.out.println("returning to main menu");
                            break;
                        }
                        else {
                            try {
                                System.out.println("Enter your password: ");
                                password = scanner.next();
                                userC = findCustomer(email, password);
                            } catch (NoSuchElementException e) {
                                System.out.println("Email or password does not match");
                                check = 1;
                            }
                        }
                    } while (check == 1);
                }
                else {
                    register();
                    userC = customers.get(customers.size() - 1);
                }
                if(!email.equals(String.valueOf(-1))) {
                    choice = -1;
                    while (choice != 5) {
                        System.out.println("CUSTOMER MENU");
                        System.out.println("1)Search Product\n2)List Of Products\n3)Shop Online\n4)View Previous Orders\n5)Return To Main Menu");
                        choice = getValidInput(scanner, 1, 5);
                        if (choice == 1) {
                            System.out.println("1)Online stock\n2)Branch stock");
                            onlineBranch = getValidInput(scanner, 1, 2);
                            userC.listOfProducts();
                            scanner.nextLine();
                            System.out.println("Enter the furniture name(use lowercase letters && enter -1 to exit): ");
                            furnitureName = getNameIndex(scanner.nextLine());
                            if (furnitureName == -1){
                                System.out.println("returning to customer menu");
                            } else {
                                while (furnitureName == productNames.length){
                                    System.out.println("Enter a valid furniture name");
                                    furnitureName = getNameIndex(scanner.nextLine());
                                }
                                System.out.println("Enter the model num: ");
                                modelNum = getValidInput(scanner, 1, modelNums[furnitureName]);
                                System.out.println("Enter the color num: ");
                                colorNum = getValidInput(scanner, 1, colorNums[furnitureName]);
                                userC.searchProducts(furnitureName, modelNum - 1, colorNum - 1, onlineBranch, this);
                            }
                        } else if (choice == 2) {
                            userC.listOfProducts();
                        } else if (choice == 3) {
                            userC.listOfProducts();
                            scanner.nextLine();
                            System.out.println("Enter the furniture name(use lowercase letters && enter -1 to exit): ");
                            furnitureName = getNameIndex(scanner.nextLine());
                            if (furnitureName == -1) {
                                System.out.println("returning to customer menu");
                            } else {
                                while (furnitureName == productNames.length){
                                    System.out.println("Enter a valid furniture name");
                                    furnitureName = getNameIndex(scanner.nextLine());
                                }
                                System.out.println("Enter the model num: ");
                                modelNum = getValidInput(scanner, 1, modelNums[furnitureName]);
                                System.out.println("Enter the color num: ");
                                colorNum = getValidInput(scanner, 1, modelNums[furnitureName]);
                                System.out.println("How many of the given product do you want: ");
                                number = getValidInput(scanner);
                                System.out.println("Enter your address: ");
                                address = scanner.next();
                                System.out.println("Enter your phone number(only digits with no whitespaces): ");
                                do {
                                    check = 0;
                                    try {
                                        phone = Long.parseLong(scanner.next());
                                    } catch (NumberFormatException e) {
                                        System.out.println("Please enter an phone number");
                                        check = 1;
                                    }
                                } while (check == 1);
                                userC.shopOnline(furnitureName, modelNum, colorNum, number, address, phone, this);
                                System.out.println("Shopping is done");
                            }
                        } else if (choice == 4) {
                            userC.viewPreviousOrders();
                        } else {
                            System.out.println("Logging out\n");
                        }
                    }
                }
            }
            else if (choiceMain == 2) {
                int stockChoice;
                int check;
                int choice;
                int modelNum;
                int colorNum;
                int number;
                int furnitureName;
                String ID;
                String password;
                branchEmployee userB = null;
                customer Customer = null;
                do {
                    check = 0;
                    System.out.println("LOGIN\nEnter your ID(enter -1 to exit): ");
                    ID = scanner.next();
                    if(ID.equals(String.valueOf(-1))){
                        System.out.println("returning to main menu");
                        break;
                    }
                    else {
                        try {
                            System.out.println("Enter your password: ");
                            password = scanner.next();
                            userB = findBranchEmployee(ID, password);
                        } catch (NoSuchElementException e) {
                            System.out.println("ID or password does not match");
                            check = 1;
                        }
                    }
                } while (check == 1);
                if(!ID.equals(String.valueOf(-1))) {
                    choice = -1;
                    while (choice != 7) {
                        System.out.println("BRANCH EMPLOYEE MENU");
                        System.out.println("1)Inquire Stock\n2)Resupply Wishlist\n3)Sale\n4)Request Permission\n5)Add Products\n" +
                                "6)See The Wishlist\n7)Return To Main Menu");
                        choice = getValidInput(scanner, 1, 7);
                        if (choice == 1) {
                            System.out.println("1)See All Of The Stocks\n2)See The Stock Info Of A Specific Furniture");
                            stockChoice = getValidInput(scanner, 1, 2);
                            if (stockChoice == 1) {
                                userB.stocksOfAllProducts();
                            } else {
                                scanner.nextLine();
                                System.out.println("Enter the furniture name(use lowercase letters && enter -1 to exit): ");
                                furnitureName = getNameIndex(scanner.nextLine());
                                if (furnitureName == -1) {
                                    System.out.println("returning to branch employee menu");
                                } else {
                                    while (furnitureName == productNames.length){
                                        System.out.println("Enter a valid furniture name");
                                        furnitureName = getNameIndex(scanner.nextLine());
                                    }
                                    System.out.println("Enter the model num: ");
                                    modelNum = getValidInput(scanner, 1, modelNums[furnitureName]);
                                    System.out.println("Enter the color num: ");
                                    colorNum = getValidInput(scanner, 1, colorNums[furnitureName]);
                                    System.out.println("Stock of chosen furniture in this branch is " +
                                            userB.inquireStock(furnitureName, modelNum - 1, colorNum - 1));
                                }
                            }
                        } else if (choice == 2) {
                            if (isWishListEmpty(userB.getBranch().getBranchWishList())) {
                                System.out.println("Wishlist is empty");
                            } else {
                                if (stocksChangeRequest.getEmployeeID() == null) {
                                    System.out.println("You have to get administrator's permission");
                                } else if (getStocksChangeRequest().getEmployeeID().equals(userB.getID()) && getStocksChangeRequest().getPermission().equals(request.Permission.GRANTED)) {
                                    getStocksChangeRequest().setPermission(request.Permission.NO_REQUEST);
                                    emptyWishlist(userB.getBranch().getBranchWishList());
                                    for (customer i : customers) {
                                        for (int j = 0; j < i.getPreviousOrdersNum(); j++) {
                                            if (!(i.getPreviousOrder(j) instanceof online) && i.getPreviousOrder(j).getWillBeSupplied()) {
                                                i.getPreviousOrder(j).setWillBeSupplied(false);
                                            }
                                        }
                                    }
                                    System.out.println("Wishlist resupplied and supply-waiting branch orders of customers are delivered");
                                } else {
                                    System.out.println("You have to get administrator's permission");
                                }
                            }
                        } else if (choice == 3) {
                            System.out.println("1)New Costumer\n2)Already Existing Costumer");
                            choice = getValidInput(scanner, 1, 2);
                            if (choice == 2) {
                                do {
                                    try {
                                        check = 0;
                                        System.out.println("Enter customer's email: ");
                                        String email = scanner.next();
                                        System.out.println("Enter customer's ID: ");
                                        ID = scanner.next();
                                        Customer = findCustomerForSale(email, ID);
                                    } catch (NoSuchElementException e) {
                                        System.out.println("Email or password does not match");
                                        check = 1;
                                    }
                                } while (check == 1);
                            } else {
                                register();
                                Customer = customers.get(customers.size() - 1);
                            }
                            scanner.nextLine();
                            System.out.println("Enter the furniture name(use lowercase letters): ");
                            furnitureName = getNameIndex(scanner.nextLine());
                            while (furnitureName == productNames.length){
                                System.out.println("Enter a valid furniture name");
                                furnitureName = getNameIndex(scanner.nextLine());
                            }
                            System.out.println("Enter the model num: ");
                            modelNum = getValidInput(scanner, 1, modelNums[furnitureName]);
                            System.out.println("Enter the color num: ");
                            colorNum = getValidInput(scanner, 1, colorNums[furnitureName]);
                            System.out.println("How many of the given product do you want: ");
                            number = getValidInput(scanner);
                            userB.sale(Customer, furnitureName, modelNum - 1, colorNum - 1, number, this);
                            System.out.println("sale is done");
                        } else if (choice == 4) {
                            if (getStocksChangeRequest() == null) {
                                System.out.println("Request sent");
                                setStocksChangeRequest(new request(userB.getID()));
                            } else if (getStocksChangeRequest().getPermission().equals(request.Permission.PARKED) ||
                                    getStocksChangeRequest().getPermission().equals(request.Permission.GRANTED)) {
                                System.out.println("Only one request can be made at a time");
                            } else {
                                System.out.println("Request sent");
                                setStocksChangeRequest(new request(userB.getID()));
                            }
                        } else if (choice == 5) {
                            if (getStocksChangeRequest() == null) {
                                System.out.println("You have to get administrator's permission");
                            } else if (getStocksChangeRequest().getEmployeeID().equals(userB.getID()) && getStocksChangeRequest().getPermission().equals(request.Permission.GRANTED)) {
                                scanner.nextLine();
                                System.out.println("Enter the furniture name(use lowercase letters && enter -1 to exit): ");
                                furnitureName = getNameIndex(scanner.nextLine());
                                if (furnitureName == -1) {
                                    System.out.println("returning to branch employee menu");
                                } else {
                                    while (furnitureName == productNames.length){
                                        System.out.println("Enter a valid furniture name");
                                        furnitureName = getNameIndex(scanner.nextLine());
                                    }
                                    System.out.println("Enter the model num: ");
                                    modelNum = getValidInput(scanner, 1, modelNums[furnitureName]);
                                    System.out.println("Enter the color num: ");
                                    colorNum = getValidInput(scanner, 1, colorNums[furnitureName]);
                                    System.out.println("Enter the number of products you want to add: ");
                                    number = getValidInput(scanner, 1, 20);
                                    userB.addProduct(furnitureName, modelNum - 1, colorNum - 1, number);
                                    System.out.println("Furniture added");
                                    getStocksChangeRequest().setPermission(request.Permission.NO_REQUEST);
                                }
                            } else {
                                System.out.println("You have to get administrator's permission");
                            }
                        } else if (choice == 6) {
                            if (isWishListEmpty(userB.getBranch().getBranchWishList())) {
                                System.out.println("Wishlist is empty");
                            } else {
                                seeWishList(userB.getBranch().getBranchWishList());
                            }
                        } else {
                            System.out.println("Logging out\n");
                        }
                    }
                }
            }
            else if (choiceMain == 3) {
                String ID;
                String password;
                String name;
                String surname;
                String branchName;
                int check;
                int choice;
                int number;
                int permissionOption;
                administrator userA = null;
                do {
                    check = 0;
                    System.out.println("LOGIN\nEnter your ID: ");
                    ID = scanner.next();
                    if(ID.equals(String.valueOf(-1))){
                        System.out.println("returning to main menu");
                        break;
                    }
                    else {
                        try {
                            System.out.println("Enter your password: ");
                            password = scanner.next();
                            userA = findAdministrator(ID, password);
                        } catch (NoSuchElementException e) {
                            System.out.println("ID or password does not match");
                            check = 1;
                        }
                    }
                } while (check == 1);
                if(!ID.equals(String.valueOf(-1))) {
                    choice = -1;
                    while (choice != 12) {
                        System.out.println("ADMINISTRATOR MENU\n1)Add Branch\n2)Remove Branch\n3)Add Branch Employee\n" +
                                "4)Remove Branch Employee\n5)Query Stocks\n6)Resupply Online WishList\n7)View Requests\n" +
                                "8)List Of Branch Employees\n9)Stocks Of All Products\n10)See The Wishlist\n11)List Of Branches\n" +
                                "12)Return To Main Menu");
                        choice = getValidInput(scanner, 1, 12);
                        if (choice == 1) {
                            System.out.println("Enter the name of the branch you want to add(enter -1 to exit): ");
                            name = scanner.next();
                            if (name.equals(String.valueOf(-1))) {
                                System.out.println("returning to administrator menu");
                            } else {
                                System.out.println("Enter the number of employees that will work in this branch: ");
                                number = getValidInput(scanner, 1, MAX_EMPLOYEE_NUM);
                                userA.addBranch(name, number, this);
                                System.out.println(name + " added as branch");
                            }
                        } else if (choice == 2) {
                            System.out.println("Enter the name of the branch you want to remove(enter -1 to exit): ");
                            branchName = scanner.next();
                            if (branchName.equals(String.valueOf(-1))) {
                                System.out.println("returning to administrator menu");
                            } else {
                                try {
                                    userA.removeBranch(branchName, this);
                                    System.out.println(branchName + " removed from branches");
                                } catch (NoSuchElementException e) {
                                    System.out.println("This branch does not exist");
                                }
                            }
                        } else if (choice == 3) {
                            do {
                                System.out.println("Enter the name of the branch that new branch employee will work(enter -1 to exit): ");
                                branchName = scanner.next();
                                if(branchName.equals(String.valueOf(-1))){
                                    break;
                                }
                            } while (getBranchIndex(branchName) >= branches.size());
                            if (branchName.equals(String.valueOf(-1))) {
                                System.out.println("returning to administrator menu");
                            } else {
                                System.out.println("Enter the name of employee: ");
                                name = scanner.next();
                                System.out.println("Enter the surname of employee: ");
                                surname = scanner.next();
                                System.out.println("Enter the password of employee: ");
                                password = scanner.next();
                                userA.addBranchEmployee(name, surname, password, branches.get(getBranchIndex(branchName)), this);
                                System.out.println(name + " " + surname + " added as branch employee");
                            }
                        } else if (choice == 4) {
                            userA.listOfBranchEmployees(this);
                            System.out.println("Enter the ID of branch employee you want to remove(enter -1 to exit): ");
                            try {
                                ID = scanner.next();
                                if (ID.equals(String.valueOf(-1))) {
                                    System.out.println("returning to administrator menu");
                                } else {
                                    if(stocksChangeRequest.getEmployeeID().equals(ID)){
                                        stocksChangeRequest.setPermission(request.Permission.NO_REQUEST);
                                    }
                                    userA.removeBranchEmployee(ID, this);
                                    System.out.println("Employee removed");
                                }
                            } catch (NoSuchElementException e) {
                                System.out.println("This employee does not exist");
                            }
                        } else if (choice == 5) {
                            if (!userA.queryStocks(this)) {
                                System.out.println("All of the stocks are enough");
                            }
                        } else if (choice == 6) {
                            if (isWishListEmpty(getOnlineWishList())) {
                                System.out.println("Online wishlist is empty");
                            } else {
                                userA.resupplyOnlineWishList(this);
                                System.out.println("Online wishlist resupplied and supply-waiting online orders of the customers are delivered");
                            }
                        } else if (choice == 7) {
                            if (getStocksChangeRequest() == null) {
                                System.out.println("There is no request to view");
                            } else if (getStocksChangeRequest().getPermission().equals(request.Permission.NO_REQUEST)) {
                                System.out.println("There is no request to view");
                            } else {
                                System.out.println(findBranchEmployee(getStocksChangeRequest().getEmployeeID()).getName() + " " +
                                        findBranchEmployee(getStocksChangeRequest().getEmployeeID()).getSurname() + " wants to change the stocks in " +
                                        findBranchEmployee(getStocksChangeRequest().getEmployeeID()).getBranch().getName());
                                System.out.println("Do you allow this?\n1)Yes\n2)No");
                                permissionOption = getValidInput(scanner, 1, 2);
                                if (permissionOption == 1) {
                                    userA.givePermission(getStocksChangeRequest());
                                } else {
                                    userA.denyPermission(getStocksChangeRequest());
                                }
                            }
                        } else if (choice == 8) {
                            userA.listOfBranchEmployees(this);
                        } else if (choice == 9) {
                            userA.stocksOfAllProducts(this);
                        } else if (choice == 10) {
                            if (isWishListEmpty(getOnlineWishList())) {
                                System.out.println("Wishlist is empty");
                            } else {
                                seeWishList(getOnlineWishList());
                            }
                        }
                        else if(choice == 11){
                            userA.listOfBranches(this);
                        }
                        else {
                            System.out.println("Logging out\n");
                        }
                    }
                }
            }
            else {
                System.out.println("GOODBYE");
            }
        }
    }
}