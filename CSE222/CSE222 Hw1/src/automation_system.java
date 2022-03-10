import java.io.File;
import java.io.FileNotFoundException;
import java.util.*;

public class automation_system implements constants{
    private administrator[] administrators;
    private branchEmployee[] branchEmployees;
    private customer[] customers;
    private branch[] branches;
    private String[] productNames = {"office chair", "office desk", "meeting table", "bookcase", "office cabinet"};
    private final int[] modelNums = {furniture.OFFICE_CHAIR_MODEL_NUM, furniture.OFFICE_DESK_MODEL_NUM, furniture.MEETING_TABLE_MODEL_NUM,
                                furniture.BOOK_CASE_MODEL_NUM, furniture.OFFICE_CABINET_MODEL_NUM};
    private final int[] colorNums = {furniture.OFFICE_CHAIR_COLOR_NUM, furniture.OFFICE_DESK_COLOR_NUM, furniture.MEETING_TABLE_COLOR_NUM,
                                furniture.BOOK_CASE_COLOR_NUM, furniture.OFFICE_CABINET_COLOR_NUM};
    private int[][][] onlineWishList;
    private storeHouse StoreHouse;
    private request stocksChangeRequest = null;
    private branchEmployeeIDgenerator IDgeneratorE;
    private customerIDgenerator IDgeneratorC;

    /**
     * @return customers array
     */
    public customer[] getCustomers() {
        return customers;
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
     * @return branches array
     */
    public branch[] getBranches() {
        return branches;
    }

    /**
     * @return online wishlist
     */
    public int[][][] getOnlineWishList() {
        return onlineWishList;
    }

    /**
     * initializes the system
     */
    public automation_system() {
        branches = new branch[DEFAULT_BRANCH_NUM];
        readBranchInfo(1, DEFAULT_BRANCH_NUM);
        readBranchInfo(2, 1);
        administrators = new administrator[DEFAULT_ADMIN_NUM];
        readUserInfo(administrators, "default_administrators.txt");
        branchEmployees = new branchEmployee[EMPLOYEE_PER_BRANCH*DEFAULT_BRANCH_NUM];
        readUserInfo(branchEmployees, "default_branch_employees.txt");
        customers = new customer[DEFAULT_CUSTOMER_NUM];
        onlineWishList = new int[modelNums.length][0][0];
        for(int i = 0; i < modelNums.length; i++){
            onlineWishList[i] = new int[modelNums[i]][colorNums[i]];
        }
        IDgeneratorC = new customerIDgenerator();
        IDgeneratorE = new branchEmployeeIDgenerator();
    }


    /**
     * class that represents one customer
     */
    public class customer extends user{
        private String email;
        private order[] previousOrders;

        /**
         * @return email of customer
         */
        public String getEmail(){
            return email;
        }

        /**
         * Returns the specified previous order
         * @param index integer that specifies which previous order will be returned
         * @return order from previousOrders array specified by the index
         */
        public order getPreviousOrders(int index) {
            return previousOrders[index];
        }


        /**
         * @return number of previous orders
         */
        public int getPreviousOrdersNum() {
            return previousOrders.length;
        }

        /**
         * constructs a customer with given parameters
         * @param name name of the customer
         * @param surname surname of the customer
         * @param email email of the customer
         * @param password password of the customer
         */
        public customer(String name, String surname, String email, String password) {
            super(name, surname, IDgeneratorC.generateID(), password);
            this.email = email;
            previousOrders = new order[0];
        }

        /**
         * Adds new order to previousOrders array
         * @param product name of the product
         * @param productModel model number of the product
         * @param productColor color number of the product
         * @param num demanded amount of the product
         * @param willBeSupplied specifies if the requested amount is more than the stocks
         */
        public void addNewOrder(String product, int productModel, int productColor, int num, boolean willBeSupplied){
            order[] temp = new order[previousOrders.length + 1];
            System.arraycopy(previousOrders, 0, temp, 0, previousOrders.length);
            temp[temp.length - 1] = new order(product, productModel, productColor, num, willBeSupplied);
            previousOrders = new order[temp.length];
            System.arraycopy(temp, 0, previousOrders, 0, temp.length);
        }

        /**
         * Adds new online order to previousOrders array
         * @param product name of the product
         * @param productModel model number of the product
         * @param productColor color number of the product
         * @param num demanded amount of the product
         * @param phoneNum phone number of the customer
         * @param address address of the customer
         * @param willBeSupplied specifies if the requested amount is more than the stocks
         */
        public void addNewOrder(String product, int productModel, int productColor, int num, long phoneNum, String address, boolean willBeSupplied){
            order[] temp = new order[previousOrders.length + 1];
            System.arraycopy(previousOrders, 0, temp, 0, previousOrders.length);
            temp[temp.length - 1] = new online(product, productModel, productColor, num, phoneNum, address, willBeSupplied);
            previousOrders = new order[temp.length];
            System.arraycopy(temp, 0, previousOrders, 0, temp.length);
        }

        /**
         * Searches chosen product across all branches or online stocks
         * @param productName name of the product
         * @param productModel model number of the product
         * @param productColor color number of the product
         * @param mode determines if the search will be done across branches or online stocks
         */
        public void searchProducts(String productName, int productModel, int productColor, int mode){
            int name;
            for (name = 0; name < productNames.length && !productName.equals(productNames[name]); name++) ;
            if(mode == 2) {
                for (int i = 0, stock; i < branches.length; i++) {
                    stock = branches[i].getStockInfo(name, productModel, productColor);
                    System.out.println(branches[i].name + " has " + stock + " of chosen product in its stocks");
                }
            }
            else{
                System.out.println("Online stock of chosen product is " + StoreHouse.getStockInfo(name, productModel, productColor));
            }
        }

        /**
         * prints all the general information about all products
         */
        public void listOfProducts(){
            System.out.println("All of our products");
            for(int i = 0; i < productNames.length; i++){
                System.out.println((i + 1) + "| " + productNames[i] + "(" + modelNums[i] +" models, " + colorNums[i] + " color per model)");
            }
        }

        /**
         * Shops online
         * @param productName name of the product
         * @param productModel model number of the product
         * @param productColor color number of the product
         * @param num demanded amount of the product
         * @param address address of the customer
         * @param phoneNo phone number of the customer
         */
        public void shopOnline(String productName, int productModel, int productColor, int num, String address, long phoneNo){
            boolean result;
            int name = getNameIndex(productName);
            if(name == productNames.length || productModel > modelNums[name] || productColor > colorNums[name]){
                throw new ArrayIndexOutOfBoundsException();
            }
            result = reduceFromStock(name, productModel - 1, productColor - 1, num);
            addNewOrder(productName, productModel, productColor, num, phoneNo, address, result);
        }

        /**
         * prints all the previous orders
         */
        public void viewPreviousOrders(){
            System.out.println("Previous Orders(new to old)");
            if(previousOrders.length == 0){
                System.out.println("There is no previous order to view");
            }
            for (int i = previousOrders.length - 1; i >= 0; i--){
                System.out.println(previousOrders[i]);
            }
        }
    }

    /**
     * class that represents one branch employee
     */
    public class branchEmployee extends user{
        private branch Branch;

        /**
         * constructs a new branch employee with given parameters
         * @param name name of the branch employee
         * @param surname surname of the branch employee
         * @param ID ID of the branch employee
         * @param password password of the branch employee
         * @param Branch branch that branch employee will work
         */
        public branchEmployee(String name, String surname, String ID, String password, branch Branch) {
            super(name, surname, ID, password);
            this.Branch = Branch;
        }

        /**
         * returns the stock of chosen product that is in employee's branch
         * @param name name of the product
         * @param model model number of the product
         * @param color color number of the product
         * @return stock of chosen product in employee's branch
         */
        public int inquireStock(int name, int model, int color){
            return Branch.getStockInfo(name, model, color);
        }

        /**
         * adds the certain amount of chosen product to wishlist to resupply later
         * @param name name of the product
         * @param model model number of the product
         * @param color color number of the product
         * @param num amount of chosen product needed
         */
        public void addToWishList(int name, int model, int color, int num){
            Branch.branchWishList[name][model][color] += num;
        }

        /**
         * prints all the stock info about all the product which are in employee's branch
         */
        public void stocksOfAllProducts(){
            for(int i = 0; i < modelNums.length; i++){
                System.out.println(productNames[i]);
                for(int j = 0; j < modelNums[i]; j++){
                    for(int k = 0; k < colorNums[i]; k++){
                        System.out.println("Model" + (j + 1) + " Color" + (k + 1) + " Stock = " +inquireStock(i, j, k));
                    }
                }
            }
        }

        /**
         * sells the chosen product to the customer
         * @param Customer customer that employee will sell the chosen product to
         * @param productName name of the product
         * @param productModel model number of the product
         * @param productColor color number of the product
         * @param num amount of chosen product which will be sold
         */
        public void sale(customer Customer, String productName, int productModel, int productColor, int num){
            int name = getNameIndex(productName);
            int initialStock = Branch.getStockInfo(name, productModel, productColor);
            if(num > inquireStock(name, productModel, productColor)){
                Branch.changeStock(name, productModel, productColor, 0);
                addToWishList(name, productModel, productColor, num - initialStock);
                Customer.addNewOrder(productName, productModel + 1, productColor + 1, num, true);
                System.out.println("Due to stock issues right now we can only provide "
                        + initialStock + " of the ordered furniture to the customer but we will send the rest after resupplying the furniture");
            }
            else{
                Branch.changeStock(name, productModel, productColor, initialStock - num);
                Customer.addNewOrder(productName, productModel + 1, productColor + 1, num, false);
            }
        }

        /**
         * adds certain amounts of chosen product to branch's stocks
         * @param name name of the product
         * @param model model number of the product
         * @param color color number of the product
         * @param num amount of chosen product which will be added
         */
        public void addProduct(String name, int model, int color, int num){
            int index = getNameIndex(name);
            int initialStock = Branch.getStockInfo(index, model, color);
            Branch.changeStock(index, model, color, initialStock + num);
        }

        /**
         * @return branch of employee
         */
        public final branch getBranch() {
            return Branch;
        }

        @Override
        public String toString() {
            return "name = " + getName() +
                    ", surname = " + getSurname() +
                    ", ID = " + getID() +
                    ", branch = " + Branch.getName();
        }
    }

    /**
     * class that represents one administrator
     */
    public class administrator extends user{

        /**
         * constructs a new administrator with given parameters
         * @param name name of the administrator
         * @param surname surname of the administrator
         * @param ID ID of the administrator
         * @param password password of the administrator
         */
        public administrator(String name, String surname, String ID, String password) {
            super(name, surname, ID, password);
        }

        /**
         * prints a list of all branches
         */
        public void listOfBranches(){
            for(int i = 0; i < branches.length; i++){
                System.out.println(branches[i].getName());
            }
        }

        /**
         * adds new branch
         * @param name name of the branch which will be added
         * @param employeeNum number of employees which will work in this branch
         */
        public void addBranch(String name, int employeeNum){
            String employeeName;
            String employeeSurname;
            String employeePassword;
            Scanner scanner = new Scanner(System.in);
            branch[] temp = new branch[branches.length + 1];
            int[][][] stocks = new int[modelNums.length][0][0];
            for(int i = 0; i < modelNums.length; i++){
                stocks[i] = new int[modelNums[i]][colorNums[i]];
            }
            for(int i = 0; i < modelNums.length; i++){
                for(int j = 0; j < modelNums[i]; j++){
                    for(int k = 0; k < colorNums[i]; k++){
                        stocks[i][j][k] = DEFAULT_STOCK_NUM;
                    }
                }
            }
            System.arraycopy(branches, 0, temp, 0, branches.length);
            temp[temp.length - 1] = new branch(name, stocks[0], stocks[1], stocks[2], stocks[3], stocks[4]);
            branches = new branch[temp.length];
            System.arraycopy(temp, 0, branches, 0, temp.length);
            for(int i = 0; i < employeeNum; i++){
                System.out.println("Enter the name of employee: ");
                employeeName = scanner.next();
                System.out.println("Enter the surname of employee: ");
                employeeSurname = scanner.next();
                System.out.println("Enter the password of employee: ");
                employeePassword = scanner.next();
                addBranchEmployee(employeeName, employeeSurname, employeePassword, branches[branches.length - 1]);
            }
        }

        /**
         * removes the chosen branch and its employees
         * @param name name of the branch which will be removed
         */
        public void removeBranch(String name){
            int branch = getBranchIndex(name);
            int initialLength = branchEmployees.length;
            branch[] temp = new branch[branches.length - 1];
            branchEmployee[] tempEmployees = new branchEmployee[initialLength];
            System.arraycopy(branchEmployees, 0, tempEmployees, 0, initialLength);
            for(int i = 0; i < initialLength; i++){
                if(tempEmployees[i].getBranch() == branches[branch]){
                    removeBranchEmployee(tempEmployees[i].getID());
                }
            }
            for(int i = 0; i < temp.length; i++){
                if(i >= branch){
                    temp[i] = branches[i + 1];
                }
                else {
                    temp[i] = branches[i];
                }
            }
            branches = new branch[temp.length];
            System.arraycopy(temp, 0, branches, 0, temp.length);
        }

        /**
         * adds a new branch employee to the chosen branch
         * @param name name of the branch employee which will be added
         * @param surname surname of the branch employee which will be added
         * @param password password of the branch employee which will be added
         * @param Branch branch of the branch employee which will be added
         */
        public void addBranchEmployee(String name, String surname, String password, branch Branch){
            branchEmployee[] temp = new branchEmployee[branchEmployees.length + 1];
            System.arraycopy(branchEmployees, 0, temp, 0, branchEmployees.length);
            temp[temp.length - 1] = new branchEmployee(name, surname, IDgeneratorE.generateID(), password, Branch);
            branchEmployees = new branchEmployee[temp.length];
            System.arraycopy(temp, 0, branchEmployees, 0, temp.length);
            System.out.println("ID of the employee: " + branchEmployees[branchEmployees.length - 1].getID());
        }

        /**
         * removes the chosen branch employee
         * @param ID ID of the branch employee which will be removed
         */
        public void removeBranchEmployee(String ID){
            branchEmployee[] temp = new branchEmployee[branchEmployees.length - 1];
            int employee;
            for(employee = 0; employee < branchEmployees.length && !branchEmployees[employee].getID().equals(ID); employee++);
            if(employee >= branchEmployees.length) throw new ArrayIndexOutOfBoundsException();
            for(int i = 0; i < temp.length; i++){
                if(i >= employee){
                    temp[i] = branchEmployees[i + 1];
                }
                else {
                    temp[i] = branchEmployees[i];
                }
            }
            branchEmployees = new branchEmployee[temp.length];
            System.arraycopy(temp, 0, branchEmployees, 0, temp.length);
        }

        /**
         * resupplies all the needed products in online wishlist and changes the status of supply-waiting online orders to completed
         */
        public void resupplyOnlineWishList(){
            for(int i = 0; i < modelNums.length; i++) {
                for (int j = 0; j < modelNums[i]; j++) {
                    for (int k = 0; k < colorNums[i]; k++) {
                        onlineWishList[i][j][k] = 0;
                    }
                }
            }
            for(int i = 0; i < customers.length; i++){
               for(int j = 0; j < customers[i].getPreviousOrdersNum(); j++){
                   if(customers[i].getPreviousOrders(j) instanceof online && customers[i].getPreviousOrders(j).getWillBeSupplied()){
                       customers[i].getPreviousOrders(j).setWillBeSupplied(false);
                   }
               }
            }
        }

        /**
         * gives permission to the branch employee who is the one makes the request to make changes in stocks
         * @param Request request which will be granted
         */
        public void givePermission(request Request){
            Request.setPermission(request.Permission.GRANTED);
        }

        /**
         * denies the request so that the branch employee who made this request can't make changes in stocks
         * @param Request request which will be denied
         */
        public void denyPermission(request Request){
            Request.setPermission(request.Permission.DENIED);
        }

        /**
         * resupplies products if they are lower than a certain amount in stocks
         * @return true if at least one product is resupplied
         */
        public boolean queryStocks(){
            int initialStock;
            int counter = 0;
            for(int i = 0; i < modelNums.length; i++){
                for(int j = 0; j < modelNums[i]; j++){
                    for(int k = 0; k < colorNums[i]; k++){
                        initialStock = StoreHouse.getStockInfo(i, j, k);
                        if(initialStock < 50){
                            System.out.println(productNames[i] + " Model" + (j + 1) + " Color" + (k + 1) +
                                    " had a stock less than 50 and resupplied to " + (initialStock + 50));
                            StoreHouse.changeStock(i, j, k, initialStock + 50);
                            counter++;
                        }
                    }
                }
            }
            if(counter > 0){
                return true;
            }
            return false;
        }

        /**
         * prints all the stock info about all the products in store house
         */
        public void stocksOfAllProducts(){
            for(int i = 0; i < modelNums.length; i++){
                System.out.println(productNames[i]);
                for(int j = 0; j < modelNums[i]; j++){
                    for(int k = 0; k < colorNums[i]; k++){
                        System.out.println("Model" + (j + 1) + " Color" + (k + 1) +
                                " Stock = " + StoreHouse.getStockInfo(i, j, k));
                    }
                }
            }
        }

        /**
         * print a list of all branch employees
         */
        public void listOfBranchEmployees(){
            for(int i = 0; i < branchEmployees.length; i++){
                System.out.println(branchEmployees[i]);
            }
        }
    }

    /**
     * class that represents one store house
     */
    public class storeHouse{
        protected furniture[] allProducts;
        /**
         * constructs a new store house with given parameters
         * @param officeChair stock of office chairs
         * @param officeDesk stock of office desks
         * @param meetingTable stock of meeting tables
         * @param bookCase stock of bookcases
         * @param officeCabinet stock of office cabinets
         */
        public storeHouse(int[][] officeChair, int[][] officeDesk, int[][] meetingTable,
                          int[][] bookCase, int[][] officeCabinet){
            allProducts = new furniture[5];
            allProducts[0] = new office_chair(officeChair);
            allProducts[1] = new office_desk(officeDesk);
            allProducts[2] = new meeting_table(meetingTable);
            allProducts[3] = new book_case(bookCase);
            allProducts[4] = new office_cabinet(officeCabinet);
        }

        /**
         * returns the stock info about chosen product in store house
         * @param productName name index of the product
         * @param productModel model number of the product
         * @param productColor color number of the product
         * @return stock info about chosen product
         */
        public int getStockInfo(int productName, int productModel, int productColor){
            return allProducts[productName].stockInfo(productModel, productColor);
        }

        /**
         * changes the stock of chosen product in store house
         * @param productName name index of the product
         * @param productModel model number of the product
         * @param productColor color number of the product
         * @param num new stock of the chosen product
         */
        public void changeStock(int productName, int productModel, int productColor, int num){
            allProducts[productName].changeStock(productModel, productColor, num);
        }
    }

    /**
     * class that represent one branch
     */
    public class branch extends storeHouse{
        private String name;
        private int[][][] branchWishList;

        /**
         * constructs a new branch with given parameters
         * @param name name of the branch
         * @param officeChair stock of office chairs
         * @param officeDesk stock of office desks
         * @param meetingTable stock of meeting tables
         * @param bookCase stock of bookcases
         * @param officeCabinet stock of office cabinets
         */
        public branch(String name, int[][] officeChair, int[][] officeDesk, int[][] meetingTable,
                      int[][] bookCase, int[][] officeCabinet) {
            super(officeChair, officeDesk, meetingTable, bookCase, officeCabinet);
            this.name = name;
            branchWishList = new int[modelNums.length][0][0];
            for(int i = 0; i < modelNums.length; i++){
                branchWishList[i] = new int[modelNums[i]][colorNums[i]];
            }
        }

        /**
         * @return wishlist of the branch
         */
        public int[][][] getBranchWishList() {
            return branchWishList;
        }

        /**
         * @return name of the branch
         */
        public String getName() {
            return name;
        }
    }

    /**
     * returns the index of chosen branch in branches array
     * @param branchName name of the chosen branch
     * @return index of chosen branch in branches array
     */
    public int getBranchIndex(String branchName){
        int index;
        for(index = 0; index < branches.length && !branchName.equals(branches[index].getName()); index++);
        return index;
    }

    /**
     * returns the index of chosen product in productNames array
     * @param productName name of the chosen product
     * @return index of chosen product in productNames array
     */
    public int getNameIndex(String productName){
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
        branchEmployee target = null;
        for(int i = 0; i < branchEmployees.length; i++){
            if(branchEmployees[i].getID().equals(ID)){
                target = branchEmployees[i];
                break;
            }
        }
        return target;
    }

    /**
     * make sures that user entered an integer
     * @param scanner scanner which will be used to get user input
     * @return integer that user entered
     */
    public int getValidInput(Scanner scanner){
        int input = -1;
        int check = -1;
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
        int check = -1;
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
        for(int i = 0; i < customers.length; i++){
            if(customers[i].getEmail().equals(email)){
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
        customer[] temp = new customer[customers.length + 1];
        System.arraycopy(customers, 0, temp, 0, customers.length);
        temp[temp.length - 1] = new customer(name, surname, email, password);
        customers = new customer[temp.length];
        System.arraycopy(temp, 0, customers, 0, temp.length);
        System.out.println("Your ID is " + customers[customers.length - 1].getID());
    }

    /**
     * generates IDs for customers
     */
    public class customerIDgenerator extends IDgenerator {
        @Override
        public String generateID(){
            String id = "67";
            for(int i = 2 + String.valueOf(customers.length + 1).length(); i < 5; i++){
                id = new String(id + "0");
            }
            id = new String(id + (customers.length + 1));
            return id;
        }
    }

    /**
     * generates IDs for employees
     */
    public class branchEmployeeIDgenerator extends IDgenerator {
        @Override
        public String generateID(){
            int num = Integer.parseInt(branchEmployees[branchEmployees.length - 1].getID());
            return String.valueOf(num + 1);
        }
    }

    /**
     * returns the customer whose email and password matches the given email and password
     * @param email email of wanted customer
     * @param password password of wanted customer
     * @return customer who meets the requirements
     * @throws NoSuchElementException if there is no customer who meets the requirements
     */
    public customer findCustomer(String email, String password){
        for(int i = 0; i < customers.length; i++){
            if(customers[i].getEmail().equals(email) && customers[i].getPassword().equals(password)){
                return customers[i];
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
        for(int i = 0; i < customers.length; i++){
            if(customers[i].getEmail().equals(email) && customers[i].getID().equals(ID)){
                return customers[i];
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
        for(int i = 0; i < administrators.length; i++){
            if(administrators[i].getID().equals(ID) && administrators[i].getPassword().equals(password)){
                return administrators[i];
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
        for(int i = 0; i < branchEmployees.length; i++){
            if(branchEmployees[i].getID().equals(ID) && branchEmployees[i].getPassword().equals(password)){
                return branchEmployees[i];
            }
        }
        throw new NoSuchElementException();
    }

    /**
     * reads the default info about administrators and branch employees
     * @param users array that read info will be stored in
     * @param filepath filepath of the wanted file
     */
    public void readUserInfo(user[] users, String filepath){
        try {
            String[] values;
            String s;
            File file = new File(filepath);
            Scanner scanner = new Scanner(file);
            for(int i = 0; scanner.hasNextLine(); i++){
                if(i >= users.length) break;
                s = scanner.nextLine();
                values = s.split("\\s");
                if(users instanceof branchEmployee[]){
                    if (i < 5) {
                        users[i] = new branchEmployee(values[0], values[1], values[2], values[3], branches[0]);
                    }
                    else if (i < 10) {
                        users[i] = new branchEmployee(values[0], values[1], values[2], values[3], branches[1]);
                    }
                    else if (i < 15) {
                        users[i] = new branchEmployee(values[0], values[1], values[2], values[3], branches[2]);
                    }
                    else {
                        users[i] = new branchEmployee(values[0], values[1], values[2], values[3], branches[3]);
                    }
                }
                else{
                    users[i] = new administrator(values[0], values[1], values[2], values[3]);
                }
            }
        }
        catch (FileNotFoundException e){
            System.out.println("Source is not found");
        }
    }

    /**
     * reads the default info about branches and store house
     * @param mode determines if the read info will be stored in branches array or in StoreHouse variable
     * @param iteration number of files to read
     */
    public void readBranchInfo(int mode, int iteration){
        File file;
        Scanner scanner;
        String line;
        String[] values;
        int[][] officeChairStock = new int[furniture.OFFICE_CHAIR_MODEL_NUM][furniture.OFFICE_CHAIR_COLOR_NUM];
        int[][] officeDeskStock = new int[furniture.OFFICE_DESK_MODEL_NUM][furniture.OFFICE_DESK_COLOR_NUM];
        int[][] meetingTableStock = new int[furniture.MEETING_TABLE_MODEL_NUM][furniture.MEETING_TABLE_COLOR_NUM];
        int[][] bookCaseStock = new int[furniture.BOOK_CASE_MODEL_NUM][furniture.BOOK_CASE_COLOR_NUM];
        int[][] officeCabinetStock = new int[furniture.OFFICE_CABINET_MODEL_NUM][furniture.OFFICE_CABINET_COLOR_NUM];
        int[] modelNums = {furniture.OFFICE_CHAIR_MODEL_NUM,furniture.OFFICE_DESK_MODEL_NUM, furniture.MEETING_TABLE_MODEL_NUM,
                            furniture.BOOK_CASE_MODEL_NUM, furniture.OFFICE_CABINET_MODEL_NUM};

        try {
            for(int i = 0; i < iteration; i++){
                if (mode == 1) {
                    file = new File("default_branch" + (i + 1) + ".txt");
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
                    branches[i] = new branch("branch" + (i + 1), officeChairStock, officeDeskStock, meetingTableStock, bookCaseStock, officeCabinetStock);
                }
                else {
                    StoreHouse = new storeHouse(officeChairStock, officeDeskStock, meetingTableStock, bookCaseStock, officeCabinetStock);
                }
                scanner.close();
            }
        }
        catch (Exception e){
            System.out.println("Source is not found");
        }
    }
}