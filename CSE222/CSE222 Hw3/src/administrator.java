import java.util.NoSuchElementException;
import java.util.Scanner;

/**
 * class that represents one administrator
 */
public class administrator extends user implements requiredFields {
    private final IDgenerator IDgeneratorE;

    /**
     * constructs a new administrator with given parameters
     * @param name name of the administrator
     * @param surname surname of the administrator
     * @param ID ID of the administrator
     * @param password password of the administrator
     */
    public administrator(String name, String surname, String ID, String password) {
        super(name, surname, ID, password);
        IDgeneratorE = new branchEmployeeIDgenerator();
    }

    /**
     * prints a list of all branches
     */
    public void listOfBranches(automation_system system){
        for(branch Branch : system.getBranches()){
            System.out.println(Branch.getName());
        }
    }

    /**
     * adds new branch
     * @param name name of the branch which will be added
     * @param employeeNum number of employees which will work in this branch
     */
    public void addBranch(String name, int employeeNum, automation_system system){
        String employeeName;
        String employeeSurname;
        String employeePassword;
        Scanner scanner = new Scanner(System.in);
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
        branch newBranch = new branch(name, stocks[0], stocks[1], stocks[2], stocks[3], stocks[4]);
        system.getBranches().add(newBranch);
        for(int i = 0; i < employeeNum; i++) {
            if(system.getTest()){
                employeeName = "employeeName" + i;
                employeeSurname = "employeeSurname" + i;
                employeePassword = "employeePassword" + i;
                addBranchEmployee(employeeName, employeeSurname, employeePassword, newBranch, system);
            } else {
                System.out.println("Enter the name of employee: ");
                employeeName = scanner.next();
                System.out.println("Enter the surname of employee: ");
                employeeSurname = scanner.next();
                System.out.println("Enter the password of employee: ");
                employeePassword = scanner.next();
                addBranchEmployee(employeeName, employeeSurname, employeePassword, newBranch, system);
            }
        }
    }

    /**
     * removes the chosen branch and its employees
     * @param name name of the branch which will be removed
     */
    public void removeBranch(String name, automation_system system){
        int branch = system.getBranchIndex(name);

        if(branch >= system.getBranches().size())
            throw new NoSuchElementException();

        branch targetBranch = system.getBranches().get(branch);

        for(int i = 0; i < system.getBranchEmployees().size(); i++){
            if(system.getBranchEmployees().get(i).getBranch() == targetBranch){
                removeBranchEmployee(system.getBranchEmployees().get(i).getID(), system);
                i--;
            }
        }
        system.getBranches().remove(branch);
    }

    /**
     * adds a new branch employee to the chosen branch
     * @param name name of the branch employee which will be added
     * @param surname surname of the branch employee which will be added
     * @param password password of the branch employee which will be added
     * @param Branch branch of the branch employee which will be added
     */
    public void addBranchEmployee(String name, String surname, String password, branch Branch, automation_system system){
        system.getBranchEmployees().add(new branchEmployee(name, surname, IDgeneratorE.generateID(system), password, Branch));
        System.out.println("ID of the employee: " + system.getBranchEmployees().get(system.getBranchEmployees().size() - 1).getID());
    }

    /**
     * removes the chosen branch employee
     * @param ID ID of the branch employee which will be removed
     */
    public void removeBranchEmployee(String ID, automation_system system){
        int employee;
        for(employee = 0; employee < system.getBranchEmployees().size() &&
                !system.getBranchEmployees().get(employee).getID().equals(ID); employee++);
        if(employee >= system.getBranchEmployees().size())
            throw new NoSuchElementException();
        system.getBranchEmployees().remove(employee);
    }

    /**
     * resupplies all the needed products in online wishlist and changes the status of supply-waiting online orders to completed
     */
    public void resupplyOnlineWishList(automation_system system){
        for(int i = 0; i < modelNums.length; i++) {
            for (int j = 0; j < modelNums[i]; j++) {
                for (int k = 0; k < colorNums[i]; k++) {
                    system.getOnlineWishList()[i][j][k] = 0;
                }
            }
        }
        for(customer Customer : system.getCustomers()){
            for(order Order : Customer.getAllPreviousOrders()){
                if(Order instanceof online && Order.getWillBeSupplied()){
                    Order.setWillBeSupplied(false);
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
    public boolean queryStocks(automation_system system){
        int initialStock;
        int counter = 0;
        for(int i = 0; i < modelNums.length; i++){
            for(int j = 0; j < modelNums[i]; j++){
                for(int k = 0; k < colorNums[i]; k++){
                    initialStock = system.getStoreHouse().getStockInfo(i, j, k);
                    if(initialStock < 50){
                        System.out.println(productNames[i] + " Model" + (j + 1) + " Color" + (k + 1) +
                                " had a stock less than 50 and resupplied to " + (initialStock + 50));
                        system.getStoreHouse().changeStock(i, j, k, initialStock + 50);
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
    public void stocksOfAllProducts(automation_system system){
        for(int i = 0; i < modelNums.length; i++){
            System.out.println(productNames[i]);
            for(int j = 0; j < modelNums[i]; j++){
                for(int k = 0; k < colorNums[i]; k++){
                    System.out.println("Model" + (j + 1) + " Color" + (k + 1) +
                            " Stock = " + system.getStoreHouse().getStockInfo(i, j, k));
                }
            }
        }
    }

    /**
     * print a list of all branch employees
     */
    public void listOfBranchEmployees(automation_system system){
        for(branchEmployee employee : system.getBranchEmployees()){
            System.out.println(employee);
        }
    }
}