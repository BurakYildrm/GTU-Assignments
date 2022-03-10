import java.util.NoSuchElementException;
import java.util.Scanner;

public class driver {
    public static void main(String[] args) {
        automation_system var = new automation_system();
        Scanner scanner = new Scanner(System.in);
        int choiceMain = 0;
        while (choiceMain != 4) {
            System.out.println("WELCOME\nPlease choose a user type to login\n1)Customer\n2)Branch Employee\n3)Administrator\n4)Close the system");
            choiceMain = var.getValidInput(scanner, 1, 4);
            if (choiceMain == 1) {
                automation_system.customer userC = null;
                int choice = 0;
                int check = -1;
                int onlineBranch = 0;
                String password;
                String email = "";
                String furnitureName;
                String address;
                int modelNum = 0;
                int colorNum = 0;
                int number = 0;
                long phone = 0;
                System.out.println("Options\n1)Signup\n2)Login");
                choice = var.getValidInput(scanner, 1, 2);
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
                                userC = var.findCustomer(email, password);
                            } catch (NoSuchElementException e) {
                                System.out.println("Email or password does not match");
                                check = 1;
                            }
                        }
                    } while (check == 1);
                }
                else {
                    var.register();
                    userC = var.getCustomers()[var.getCustomers().length - 1];
                }
                if(!email.equals(String.valueOf(-1))) {
                    choice = -1;
                    while (choice != 5) {
                        System.out.println("CUSTOMER MENU");
                        System.out.println("1)Search Product\n2)List Of Products\n3)Shop Online\n4)View Previous Orders\n5)Return To Main Menu");
                        choice = var.getValidInput(scanner, 1, 5);
                        if (choice == 1) {
                            System.out.println("1)Online stock\n2)Branch stock");
                            onlineBranch = var.getValidInput(scanner, 1, 2);
                            userC.listOfProducts();
                            scanner.nextLine();
                            System.out.println("Enter the furniture name(use lowercase letters && enter -1 to exit): ");
                            furnitureName = scanner.nextLine();
                            if (furnitureName.equals(String.valueOf(-1))) {
                                System.out.println("returning to customer menu");
                            } else {
                                try {
                                    System.out.println("Enter the model num: ");
                                    modelNum = var.getValidInput(scanner);
                                    System.out.println("Enter the color num: ");
                                    colorNum = var.getValidInput(scanner);
                                    userC.searchProducts(furnitureName, modelNum - 1, colorNum - 1, onlineBranch);
                                } catch (ArrayIndexOutOfBoundsException e) {
                                    System.out.println("This furniture does not exist");
                                }
                            }
                        } else if (choice == 2) {
                            userC.listOfProducts();
                        } else if (choice == 3) {
                            userC.listOfProducts();
                            scanner.nextLine();
                            System.out.println("Enter the furniture name(use lowercase letters && enter -1 to exit): ");
                            furnitureName = scanner.nextLine();
                            if (furnitureName.equals(String.valueOf(-1))) {
                                System.out.println("returning to customer menu");
                            } else {
                                try {
                                    System.out.println("Enter the model num: ");
                                    modelNum = var.getValidInput(scanner);
                                    System.out.println("Enter the color num: ");
                                    colorNum = var.getValidInput(scanner);
                                    System.out.println("How many of the given product do you want: ");
                                    number = var.getValidInput(scanner);
                                    System.out.println("Enter your address: ");
                                    address = scanner.next();
                                    System.out.println("Enter your phone number(only digits and no whitespaces): ");
                                    do {
                                        check = 0;
                                        try {
                                            phone = Long.parseLong(scanner.next());
                                        } catch (NumberFormatException e) {
                                            System.out.println("Please enter an phone number");
                                            check = 1;
                                        }
                                    } while (check == 1);
                                    userC.shopOnline(furnitureName, modelNum, colorNum, number, address, phone);
                                    System.out.println("Shopping is done");
                                } catch (ArrayIndexOutOfBoundsException e) {
                                    System.out.println("This furniture does not exist");
                                }
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
                int check = -1;
                int choice;
                int modelNum = 0;
                int colorNum = 0;
                int number = 0;
                String ID;
                String password;
                String furnitureName;
                automation_system.branchEmployee userB = null;
                automation_system.customer Customer = null;
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
                            userB = var.findBranchEmployee(ID, password);
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
                        choice = var.getValidInput(scanner, 1, 7);
                        if (choice == 1) {
                            System.out.println("1)See All Of The Stocks\n2)See The Stock Info Of A Specific Furniture");
                            stockChoice = var.getValidInput(scanner, 1, 2);
                            if (stockChoice == 1) {
                                userB.stocksOfAllProducts();
                            } else {
                                scanner.nextLine();
                                System.out.println("Enter the furniture name(use lowercase letters && enter -1 to exit): ");
                                furnitureName = scanner.nextLine();
                                if (furnitureName.equals(String.valueOf(-1))) {
                                    System.out.println("returning to branch employee menu");
                                } else {
                                    try {
                                        System.out.println("Enter the model num: ");
                                        modelNum = var.getValidInput(scanner);
                                        System.out.println("Enter the color num: ");
                                        colorNum = var.getValidInput(scanner);
                                        System.out.println("Stock of chosen furniture in this branch is " +
                                                userB.inquireStock(var.getNameIndex(furnitureName), modelNum - 1, colorNum - 1));
                                    } catch (ArrayIndexOutOfBoundsException e) {
                                        System.out.println("This furniture does not exist");
                                    }
                                }
                            }
                        } else if (choice == 2) {
                            if (var.isWishListEmpty(userB.getBranch().getBranchWishList())) {
                                System.out.println("Wishlist is empty");
                            } else {
                                if (var.getStocksChangeRequest() == null) {
                                    System.out.println("You have to get administrator's permission");
                                } else if (var.getStocksChangeRequest().getEmployeeID().equals(userB.getID()) && var.getStocksChangeRequest().getPermission().equals(request.Permission.GRANTED)) {
                                    var.getStocksChangeRequest().setPermission(request.Permission.NO_REQUEST);
                                    var.emptyWishlist(userB.getBranch().getBranchWishList());
                                    for (int i = 0; i < var.getCustomers().length; i++) {
                                        for (int j = 0; j < var.getCustomers()[i].getPreviousOrdersNum(); j++) {
                                            if (!(var.getCustomers()[i].getPreviousOrders(j) instanceof online) && var.getCustomers()[i].getPreviousOrders(j).getWillBeSupplied()) {
                                                var.getCustomers()[i].getPreviousOrders(j).setWillBeSupplied(false);
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
                            choice = var.getValidInput(scanner, 1, 2);
                            if (choice == 2) {
                                do {
                                    try {
                                        check = 0;
                                        System.out.println("Enter customer's email: ");
                                        String email = scanner.next();
                                        System.out.println("Enter customer's ID: ");
                                        ID = scanner.next();
                                        Customer = var.findCustomerForSale(email, ID);
                                    } catch (NoSuchElementException e) {
                                        System.out.println("Email or password does not match");
                                        check = 1;
                                    }
                                } while (check == 1);
                            } else {
                                var.register();
                                Customer = var.getCustomers()[var.getCustomers().length - 1];
                            }
                            try {
                                scanner.nextLine();
                                System.out.println("Enter the furniture name(use lowercase letters): ");
                                furnitureName = scanner.nextLine();
                                System.out.println("Enter the model num: ");
                                modelNum = var.getValidInput(scanner);
                                System.out.println("Enter the color num: ");
                                colorNum = var.getValidInput(scanner);
                                System.out.println("How many of the given product do you want: ");
                                number = var.getValidInput(scanner);
                                userB.sale(Customer, furnitureName, modelNum - 1, colorNum - 1, number);
                                System.out.println("sale is done");
                            } catch (ArrayIndexOutOfBoundsException e) {
                                System.out.println("This furniture does not exist");
                            }

                        } else if (choice == 4) {
                            if (var.getStocksChangeRequest() == null) {
                                System.out.println("Request sent");
                                var.setStocksChangeRequest(new request(userB.getID()));
                            } else if (var.getStocksChangeRequest().getPermission().equals(request.Permission.PARKED) ||
                                    var.getStocksChangeRequest().getPermission().equals(request.Permission.GRANTED)) {
                                System.out.println("Only one request can be made at a time");
                            } else {
                                System.out.println("Request sent");
                                var.setStocksChangeRequest(new request(userB.getID()));
                            }
                        } else if (choice == 5) {
                            if (var.getStocksChangeRequest() == null) {
                                System.out.println("You have to get administrator's permission");
                            } else if (var.getStocksChangeRequest().getEmployeeID().equals(userB.getID()) && var.getStocksChangeRequest().getPermission().equals(request.Permission.GRANTED)) {
                                scanner.nextLine();
                                System.out.println("Enter the furniture name(use lowercase letters && enter -1 to exit): ");
                                furnitureName = scanner.nextLine();
                                if (furnitureName.equals(String.valueOf(-1))) {
                                    System.out.println("returning to branch employee menu");
                                } else {
                                    try {
                                        System.out.println("Enter the model num: ");
                                        modelNum = var.getValidInput(scanner);
                                        System.out.println("Enter the color num: ");
                                        colorNum = var.getValidInput(scanner);
                                        System.out.println("Enter the number of products you want to add: ");
                                        number = var.getValidInput(scanner, 1, 20);
                                        userB.addProduct(furnitureName, modelNum - 1, colorNum - 1, number);
                                        System.out.println("Furniture added");
                                        var.getStocksChangeRequest().setPermission(request.Permission.NO_REQUEST);
                                    } catch (ArrayIndexOutOfBoundsException e) {
                                        System.out.println("This furniture does not exist");
                                    }
                                }
                            } else {
                                System.out.println("You have to get administrator's permission");
                            }
                        } else if (choice == 6) {
                            if (var.isWishListEmpty(userB.getBranch().getBranchWishList())) {
                                System.out.println("Wishlist is empty");
                            } else {
                                var.seeWishList(userB.getBranch().getBranchWishList());
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
                int check = -1;
                int choice;
                int number;
                int permissionOption;
                automation_system.administrator userA = null;
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
                            userA = var.findAdministrator(ID, password);
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
                        choice = var.getValidInput(scanner, 1, 12);
                        if (choice == 1) {
                            System.out.println("Enter the name of the branch you want to add(enter -1 to exit): ");
                            name = scanner.next();
                            if (name.equals(String.valueOf(-1))) {
                                System.out.println("returning to administrator menu");
                            } else {
                                System.out.println("Enter the number of employees that will work in this branch: ");
                                number = var.getValidInput(scanner, 1, var.MAX_EMPLOYEE_NUM);
                                userA.addBranch(name, number);
                                System.out.println(name + " added as branch");
                            }
                        } else if (choice == 2) {
                            System.out.println("Enter the name of the branch you want to remove(enter -1 to exit): ");
                            branchName = scanner.next();
                            if (branchName.equals(String.valueOf(-1))) {
                                System.out.println("returning to administrator menu");
                            } else {
                                try {
                                    userA.removeBranch(branchName);
                                    System.out.println(branchName + " removed from branches");
                                } catch (ArrayIndexOutOfBoundsException e) {
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
                            } while (var.getBranchIndex(branchName) >= var.getBranches().length);
                            if (branchName.equals(String.valueOf(-1))) {
                                System.out.println("returning to administrator menu");
                            } else {
                                System.out.println("Enter the name of employee: ");
                                name = scanner.next();
                                System.out.println("Enter the surname of employee: ");
                                surname = scanner.next();
                                System.out.println("Enter the password of employee: ");
                                password = scanner.next();
                                userA.addBranchEmployee(name, surname, password, var.getBranches()[var.getBranchIndex(branchName)]);
                                System.out.println(name + " " + surname + " added as branch employee");
                            }
                        } else if (choice == 4) {
                            userA.listOfBranchEmployees();
                            System.out.println("Enter the ID of branch employee you want to remove(enter -1 to exit): ");
                            try {
                                ID = scanner.next();
                                if (ID.equals(String.valueOf(-1))) {
                                    System.out.println("returning to administrator menu");
                                } else {
                                    userA.removeBranchEmployee(ID);
                                    System.out.println("Employee removed");
                                }
                            } catch (ArrayIndexOutOfBoundsException e) {
                                System.out.println("This employee does not exist");
                            }
                        } else if (choice == 5) {
                            if (!userA.queryStocks()) {
                                System.out.println("All of the stocks are enough");
                            }
                        } else if (choice == 6) {
                            if (var.isWishListEmpty(var.getOnlineWishList())) {
                                System.out.println("Online wishlist is empty");
                            } else {
                                userA.resupplyOnlineWishList();
                                System.out.println("Online wishlist resupplied and supply-waiting online orders of the customers are delivered");
                            }
                        } else if (choice == 7) {
                            if (var.getStocksChangeRequest() == null) {
                                System.out.println("There is no request to view");
                            } else if (var.getStocksChangeRequest().getPermission().equals(request.Permission.NO_REQUEST)) {
                                System.out.println("There is no request to view");
                            } else {
                                System.out.println(var.findBranchEmployee(var.getStocksChangeRequest().getEmployeeID()).getName() + " " +
                                        var.findBranchEmployee(var.getStocksChangeRequest().getEmployeeID()).getSurname() + " wants to change the stocks in " +
                                        var.findBranchEmployee(var.getStocksChangeRequest().getEmployeeID()).getBranch().getName());
                                System.out.println("Do you allow this?\n1)Yes\n2)No");
                                permissionOption = var.getValidInput(scanner, 1, 2);
                                if (permissionOption == 1) {
                                    userA.givePermission(var.getStocksChangeRequest());
                                } else {
                                    userA.denyPermission(var.getStocksChangeRequest());
                                }
                            }
                        } else if (choice == 8) {
                            userA.listOfBranchEmployees();
                        } else if (choice == 9) {
                            userA.stocksOfAllProducts();
                        } else if (choice == 10) {
                            if (var.isWishListEmpty(var.getOnlineWishList())) {
                                System.out.println("Wishlist is empty");
                            } else {
                                var.seeWishList(var.getOnlineWishList());
                            }
                        }
                        else if(choice == 11){
                            userA.listOfBranches();
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