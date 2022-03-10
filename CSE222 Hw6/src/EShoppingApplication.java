import java.io.*;
import java.util.*;

/**
 * Class that represents an e-shopping application.
 */
public class EShoppingApplication {
    private Hashtable<String, Customer> customers = new Hashtable<>();
    private Hashtable<String, Trader> traders = new Hashtable<>();
    private ArrayList<String> IDs = new ArrayList<>();

    protected Hashtable<String, Trader> getTraders() {
        return traders;
    }

    /**
     * Constructs a EShoppingApplication with e-commerce-samples.csv file.
     */
    public EShoppingApplication(boolean isTest) {
        try {
            FileWriter productsFile = new FileWriter("products.txt");
            FileWriter usersFile = new FileWriter("users.txt");
            FileWriter ordersFile = new FileWriter("orders.txt");
            ArrayList<String> traderNames = new ArrayList<>();
            List<String> products;
            String line;
            String password;
            BufferedReader bufferedReaderProducts = new BufferedReader(new FileReader("e-commerce-samples.csv"));
            bufferedReaderProducts.readLine();
            while ((line = bufferedReaderProducts.readLine()) != null){
                products = Arrays.asList(line.split(";"));
                products.set(2, products.get(2).replace("\"[\"\"", ""));
                products.set(2, products.get(2).replace("\"\"]\"", ""));
                productsFile.write("ID: " + products.get(0) + "\n");
                productsFile.write("Name: " + products.get(1) + "\n");
                productsFile.write("Category: " + products.get(2) + "\n");
                productsFile.write("Price: " + products.get(3) + "\n");
                productsFile.write("Discounted Price: " + products.get(4) + "\n");
                productsFile.write("Description: " + products.get(5) + "\n");
                productsFile.write("Trader: " + products.get(6) + "\n\n");
                if(!traderNames.contains(products.get(6))){
                    traderNames.add(products.get(6));
                }
            }
            for(String trader : traderNames){
                IDs.add(generateID(IDs));
                password = generatePassword();
                usersFile.write("ID: " + IDs.get(IDs.size() - 1) + "\nPassword: " + password + "\nTrader: " + trader + "\n\n");
                traders.put(IDs.get(IDs.size() - 1), new Trader(trader, password));
            }
            if(isTest){
                IDs.add(generateID(IDs));
                usersFile.write("ID: " + IDs.get(IDs.size() - 1) + "\nPassword: trader\nTrader: trader\n\n");
                traders.put( IDs.get(IDs.size() - 1), new Trader("trader", "trader"));
            }
            productsFile.close();
            usersFile.close();
            ordersFile.close();
            bufferedReaderProducts.close();
        } catch (IOException e) {
            System.out.println("An error occurred");
        }
    }

    /**
     * Generates random eight-digit ID for new users.
     * @param IDArray arraylist of used IDs
     * @return randomly generated ID
     */
    private String generateID(ArrayList<String> IDArray){
        Random random = new Random();
        int ID;

        do{
            ID = random.nextInt(89999999) + 10000000;
        } while (IDArray.contains(String.valueOf(ID)));

        return String.valueOf(ID);
    }

    /**
     * Generates random six-character password for traders in e-commerce-samples.csv file.
     * @return randomly generated password
     */
    private String generatePassword(){
        String chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvxyz";
        StringBuilder password = new StringBuilder();

        for(int i = 0; i < 6; i++){
            password.append(chars.charAt((int)(chars.length() * Math.random())));
        }

        return password.toString();
    }

    /**
     * Makes sure that user entered an integer.
     * @param scanner scanner which will be used to get user input
     * @param mode determines the value(maxvalue or minvalue) if the user enters '.'(dot)
     * @return integer that user entered
     */
    private int getValidInput(Scanner scanner, int mode){
        int input = -1;
        int check = -1;
        String next;
        do {
            check = 0;
            try {
                next = scanner.nextLine();
                if(next.equals(".")){
                    input = mode == 1 ? Integer.MIN_VALUE : Integer.MAX_VALUE;
                    break;
                }
                input = Integer.parseInt(next);
            } catch (NumberFormatException e) {
                System.out.println("Please enter an integer");
                check = 1;
            }
        } while (check == 1);
        return input;
    }

    /**
     * Makes sure that user entered an integer between lowerBound and upperBound.
     * @param scanner scanner which will be used to get user input
     * @param lowerBound min value that user can enter
     * @param upperBound max value that user can enter
     * @return integer that meets the given requirements
     */
    private int getValidInput(Scanner scanner, int lowerBound, int upperBound){
        int input = -1;
        int check = -1;
        do {
            check = 0;
            try {
                input = Integer.parseInt(scanner.nextLine());
                if(input < lowerBound || input > upperBound){
                    System.out.println("Please enter an integer between " + lowerBound + " and " + upperBound);
                }
            }
            catch (NumberFormatException e){
                System.out.println("Please enter an integer between " + lowerBound + " and " + upperBound);
                check = 1;
            }
        } while (input < lowerBound || input > upperBound || check == 1);
        return input;
    }

    /**
     * Runs the application.
     */
    public void runApplication(){
        int userType = 0;
        int entryType;
        String ID;
        String password;
        Customer customer;
        Trader trader;
        Scanner scanner = new Scanner(System.in);

        while (userType != 3) {
            System.out.println("---E-Shopping Application---\nPlease select a user type to enter the system" +
                    "\n1)Customer\n2)Trader\n3)Close The Application");
            userType = getValidInput(scanner, 1, 3);

            if (userType == 1) {
                int choice = 0;
                System.out.println("1)Login\n2)Sign In");
                entryType = getValidInput(scanner, 1, 2);

                if (entryType == 1) {
                    int count = 0;
                    do {
                        if(count != 0){
                            System.out.println("ID or password doesn't match\n");
                        }
                        System.out.println("Please enter your ID");
                        ID = scanner.nextLine();
                        System.out.println("Please enter your password");
                        password = scanner.nextLine();
                        count++;
                    } while ((customer = customers.get(ID)) == null || !customer.getPassword().equals(password));
                } else {
                    customer = (Customer) SignIn(1);
                }

                System.out.println("Welcome " + customer.getName());
                while (choice != 7) {
                    System.out.println("1)Search Product\n2)Sort Search Results\n3)Filter Search Results\n" +
                            "4)View All Products Of A Trader\n5)Order Product\n6)View Previous Orders\n7)Return To Main Menu");
                    choice = getValidInput(scanner, 1, 7);

                    switch (choice) {
                        case 1:
                            System.out.println("Please enter a phrase to search");
                            String phrase = scanner.nextLine();
                            try {
                                Queue<Product> searchResults = customer.searchProduct(phrase, this);
                                while (!searchResults.isEmpty()){
                                    System.out.print(searchResults.poll());
                                }
                            } catch (NoSuchElementException e) {
                                System.out.println("There is no match");
                            }
                            break;
                        case 2:
                            System.out.println("1)Sort By Name\n2)Sort By Price\n3)Sort By Discount Percentage");
                            int sortChoice = getValidInput(scanner, 1, 3);
                            System.out.println("1)Increasing Order\n2)Decreasing Order");
                            int order = getValidInput(scanner, 1, 2);
                            ArrayList<Product> sortResults = customer.sortSearchResults(sortChoice, order);
                            System.out.println("Sorted search results\n");
                            for (Product product : sortResults) {
                                System.out.print(product);
                            }
                            break;
                        case 3:
                            System.out.println("1)Filter By Category\n2)Filter By Price");
                            int filter = getValidInput(scanner, 1, 2);
                            if (filter == 1) {
                                System.out.println("Please enter the category");
                                String category = scanner.nextLine();
                                try {
                                    ArrayList<Product> filteredResults = customer.filterByCategory(category);
                                    System.out.println("Filtered search results\n");
                                    for (Product product : filteredResults) {
                                        System.out.print(product);
                                    }
                                } catch (NoSuchElementException e) {
                                    System.out.println("There is no product with the category you entered");
                                }
                            } else {
                                System.out.println("Please enter the lower limit(if you don't want a lower limit, enter '.')");
                                int lowerLimit = getValidInput(scanner, 1);
                                System.out.println("Please enter the upper limit(if you don't want a upper limit, enter '.')");
                                int upperLimit = getValidInput(scanner, 2);
                                ArrayList<Product> filteredResults = customer.filterByPrice(upperLimit, lowerLimit);
                                System.out.println("Filtered search results\n");
                                for (Product product : filteredResults) {
                                    System.out.print(product);
                                }
                            }
                            break;
                        case 4:
                            System.out.println("Please enter the name of the trader");
                            String traderName = scanner.nextLine();
                            try {
                                customer.viewTraderProduct(traderName);
                            } catch (NoSuchElementException e) {
                                System.out.println("There is no trader with the name you entered");
                            }
                            break;
                        case 5:
                            System.out.println("Please enter the ID of the product");
                            String productID = scanner.nextLine();
                            System.out.println("Please enter the amount");
                            int amount = getValidInput(scanner, 0);
                            try {
                                customer.orderProduct(productID, amount, this);
                                System.out.println("Your order is received");
                            } catch (NoSuchElementException e) {
                                System.out.println("There is no product with ID you entered");
                            }
                            break;
                        case 6:
                            customer.viewPreviousOrders();
                            break;
                        default:
                            System.out.println("Returning to the main menu\n");
                            break;
                    }
                }
            }
            else if(userType == 2){
                int choice = 0;
                System.out.println("1)Login\n2)Sign In");
                entryType = getValidInput(scanner, 1, 2);

                if (entryType == 1) {
                    int count = 0;
                    do {
                        if(count != 0){
                            System.out.println("ID or password doesn't match\n");
                        }
                        System.out.println("Please enter your ID");
                        ID = scanner.nextLine();
                        System.out.println("Please enter your password");
                        password = scanner.nextLine();
                        count++;
                    } while ((trader = traders.get(ID)) == null || !trader.getPassword().equals(password));
                } else {
                    trader = (Trader) SignIn(2);
                }
                while (choice != 8) {
                    System.out.println("Welcome " + trader.getName());
                    System.out.println("1)Add Product\n2)Remove Product\n3)Edit Product\n4)View Orders\n5)Meet Order" +
                            "\n6)Cancel Order\n7)See Products\n8)Return To Main Menu");
                    choice = getValidInput(scanner, 1, 8);

                    switch (choice) {
                        case 1:
                            System.out.println("Please enter the name of the new product");
                            String productName = scanner.nextLine();
                            System.out.println("Please enter the category of the new product(put >> between subcategories)");
                            String productCategory = scanner.nextLine();
                            System.out.println("Please enter the price of the new product");
                            int price = getValidInput(scanner, 0);
                            System.out.println("Please enter the discounted price of the new product");
                            int discountedPrice = getValidInput(scanner, 0);
                            System.out.println("Please enter the description of the new product");
                            String description = scanner.nextLine();
                            trader.addProduct(productName, productCategory, String.valueOf(price),
                                    String.valueOf(discountedPrice), description);
                            break;
                        case 2:
                            System.out.println("Please enter the ID of the product");
                            String productID = scanner.nextLine();
                            try {
                                trader.removeProduct(productID);
                                System.out.println("Product removed");
                            } catch (NoSuchElementException e) {
                                System.out.println("There is no product with ID you entered");
                            }
                            break;
                        case 3:
                            String newData;
                            System.out.println("Please enter the ID of the product");
                            productID = scanner.nextLine();
                            System.out.println("Please choose an attribute to edit\n1)Name\n2)Category" +
                                    "\n3)Price\n4)Discounted Price\n5)Description");
                            int editChoice = getValidInput(scanner, 1, 5);
                            System.out.println("Please enter the new data");
                            if (editChoice == 3 || editChoice == 4) {
                                newData = String.valueOf(getValidInput(scanner, 0));
                            } else {
                                newData = scanner.nextLine();
                            }
                            trader.editProduct(productID, newData, editChoice);
                            System.out.println("Product edited");
                            break;
                        case 4:
                            trader.viewOrders();
                            break;
                        case 5:
                            System.out.println("Please enter the ID of the order");
                            String orderID = scanner.nextLine();
                            try {
                                trader.setOrderStatus(orderID, 1);
                                System.out.println("Order is met");
                            } catch (NoSuchElementException e) {
                                System.out.println("There is no order with ID you entered");
                            }
                            break;
                        case 6:
                            System.out.println("Please enter the ID of the order");
                            orderID = scanner.nextLine();
                            try {
                                trader.setOrderStatus(orderID, 2);
                                System.out.println("Order is cancelled");
                            } catch (NoSuchElementException e) {
                                System.out.println("There is no order with ID you entered");
                            }
                            break;
                        case 7:
                            trader.seeProducts();
                            break;
                        default:
                            System.out.println("Returning to the main menu\n");
                            break;
                    }
                }
            }
            else{
                System.out.println("GOODBYE");
            }
        }
    }

    /**
     * Adds new user to the users.txt and proper fields.
     * @param mode determines whether the new user is a customer or a trader(1 for customer, 2 for trader)
     * @return new user
     */
    private User SignIn(int mode){
        try {
            String name;
            String password;
            String ID;
            Scanner scanner = new Scanner(System.in);
            FileWriter fileWriter = new FileWriter("users.txt", true);
            User newUser;

            System.out.println("Please enter your name");
            name = scanner.nextLine();
            System.out.println("Please enter your password");
            password = getValidPassword(scanner);
            ID = generateID(IDs);
            System.out.println("Your ID is " + ID);

            if(mode == 1){
                newUser = new Customer(name, password);
                customers.put(ID, (Customer) newUser);
                fileWriter.write("ID: " + ID + "\nPassword: " + password + "\nCustomer: " + name + "\n\n");
            }
            else{
                newUser = new Trader(name, password);
                traders.put(ID, (Trader) newUser);
                fileWriter.write("ID: " + ID + "\nPassword: " + password + "\nTrader: " + name + "\n\n");
            }

            fileWriter.close();
            return newUser;
        } catch (IOException e) {
            System.out.println("An error occurred");
            return null;
        }
    }

    /**
     * Makes sure that the entered password is six-character long.
     * @param scanner scanner which will be used to get user input
     * @return six-character long password
     */
    private String getValidPassword(Scanner scanner){
        String password = "";
        int count = 0;
        do{
            if(count != 0){
                System.out.println("Your password's length should be 6");
            }
            password = scanner.nextLine();
            count++;
        } while (password.length() != 6);
        return password;
    }
}
