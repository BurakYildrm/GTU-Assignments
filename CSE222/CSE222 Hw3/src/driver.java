import java.util.Scanner;

public class driver {
    public static void main(String[] args) {
        automation_system system = new automation_system();
        system.setTest(true);
        system.newCustomer("customerName", "customerSurname", "c@gmail.com", "1234");
        customer Customer;
        branchEmployee employee;
        administrator admin;

        System.out.println("CUSTOMER FUNCTIONS");
        System.out.println("logging in as non-existing customer");
        try {
            System.out.println("email: non-existing, password: non-existing");
            Customer = system.findCustomer("non-existing", "non-existing");
        } catch (Exception e) {
            System.out.println(e);
        }

        System.out.println("\nlogging in as existing customer");
        System.out.println("email: c@gmail.com, password: 1234");
        Customer = system.findCustomer("c@gmail.com", "1234");
        System.out.println("customer name: " + Customer.getName() + " customer surname: " + Customer.getSurname());
        System.out.println("logged in successfully");

        System.out.println("\nsearching a non-existing furniture in online stock");
        try {
            System.out.println("furniture index: 6, model index: 1, color index: 1");
            Customer.searchProducts(6, 1, 2, 1, system);
        } catch (Exception e) {
            System.out.println(e);
        }

        System.out.println("\nsearching an existing furniture in online stock");
        System.out.println("furniture index: 1, model index: 1, color index: 1");
        Customer.searchProducts(1, 1, 1, 1, system);
        System.out.println("search completed successfully");

        System.out.println("\nsearching a non-existing furniture in branch stock");
        try {
            System.out.println("furniture index: 20, model index: 1, color index: 1");
            Customer.searchProducts(20, 1, 1, 2, system);
        } catch (Exception e) {
            System.out.println(e);
        }

        System.out.println("\nsearching an existing furniture in branch stock");
        System.out.println("furniture index: 3, model index: 5, color index: 0");
        Customer.searchProducts(3, 5, 0, 2, system);
        System.out.println("search completed successfully");

        System.out.println("\nlist of products");
        Customer.listOfProducts();
        System.out.println("products are listed successfully");

        System.out.println("\nshopping a non-existing furniture");
        try {
            System.out.println("furniture index: 45, model index: 3, color index: 3, amount: 10, address: address, phone: 1234");
            Customer.shopOnline(45, 3, 3, 10, "address", 1234, system);
        } catch (Exception e) {
            System.out.println(e);
        }

        System.out.println("\nshopping an existing furniture");
        System.out.println("furniture index: 2, model index: 7, color index: 3, amount: 10, address: address, phone: 1234");
        Customer.shopOnline(2, 7, 3, 10, "address", 1234, system);
        Customer.viewPreviousOrders();
        System.out.println("shopping is done successfully");

        System.out.println("\nviewing previous orders");
        Customer.viewPreviousOrders();
        System.out.println("orders are viewed successfully");

        System.out.println("\n\nBRANCH EMPLOYEE FUNCTIONS");
        System.out.println("logging in as non-existing branch employee");
        try {
            System.out.println("ID: non-existing, password: non-existing");
            employee = system.findBranchEmployee("non-existing", "non-existing");
        } catch (Exception e) {
            System.out.println(e);
        }

        System.out.println("\nlogging in as existing branch employee");
        System.out.println("ID: 66001, password: MM00001");
        employee = system.findBranchEmployee("66001", "MM00001");
        System.out.println("branch employee name: " + employee.getName() + " branch employee surname: " + employee.getSurname());
        System.out.println("logged in successfully");

        System.out.println("\nviewing stock of a non-existing furniture");
        try {
            System.out.println("furniture index: 10, model index: 3, color index: 4");
            System.out.println("stock: " + employee.inquireStock(10, 3, 4));
        } catch (Exception e) {
            System.out.println(e);
        }

        System.out.println("\nviewing stock of an existing furniture");
        System.out.println("furniture index: 4, model index: 11, color index: 0");
        System.out.println("stock: " + employee.inquireStock(4, 11, 0));
        System.out.println("stock is viewed successfully");

        System.out.println("\nviewing stocks of all furniture");
        employee.stocksOfAllProducts();

        System.out.println("\nresupplying empty wishlist");
        if(system.isWishListEmpty(employee.getBranch().getBranchWishList()))
            System.out.println("wishlist is empty");

        System.out.println("\nresupplying non-empty wishlist without the permission");
        employee.addToWishList(1, 2, 3, 10);
        system.getStocksChangeRequest().setPermission(request.Permission.PARKED);
        System.out.println("permission: " + system.getStocksChangeRequest().getPermission());
        if(system.getStocksChangeRequest().getPermission().equals(request.Permission.PARKED))
            System.out.println("this operation cannot be done without the permission");
        system.getStocksChangeRequest().setID(employee.getID());

        System.out.println("\nresupplying non-empty wishlist with the permission");
        employee.addToWishList(1, 2, 3, 10);
        system.getStocksChangeRequest().setPermission(request.Permission.GRANTED);
        System.out.println("permission: " + system.getStocksChangeRequest().getPermission());
        System.out.println("before resupply");
        system.seeWishList(employee.getBranch().getBranchWishList());
        if(!system.isWishListEmpty(employee.getBranch().getBranchWishList()) &&
                system.getStocksChangeRequest().getPermission().equals(request.Permission.GRANTED))
            system.emptyWishlist(employee.getBranch().getBranchWishList());
        System.out.println("after resupply");
        if(system.isWishListEmpty(employee.getBranch().getBranchWishList())){
            System.out.println("wishlist is empty");
        }
        System.out.println("wishlist is resupplied successfully");

        System.out.println("\nselling to a non-existing customer");
        try {
            System.out.println("customer email: non-existing, customer ID: non-existing");
            employee.sale(system.findCustomerForSale("non-existing", "non-existing"),
                    3, 5, 6, 10, system);
        } catch (Exception e) {
            System.out.println(e);
        }

        System.out.println("\nselling a non-existing furniture to an existing customer");
        try {
            System.out.println("customer email: c@gmail.com, customer ID: 67001");
            System.out.println("furniture index: 10, model index: 2, color index: 3, amount: 4");
            employee.sale(system.findCustomerForSale("c@gmail.com", "67001"),
                    10, 2, 3, 4, system);
        } catch (Exception e) {
            System.out.println(e);
        }

        System.out.println("\nselling an existing furniture to an existing customer");
        System.out.println("customer email: c@gmail.com, customer ID: 67001");
        System.out.println("furniture index: 1, model index: 3, color index: 2, amount: 10");
        employee.sale(system.findCustomerForSale("c@gmail.com", "67001"),
                1, 3, 2, 10, system);
        System.out.println("previous orders of the customer");
        system.findCustomer("c@gmail.com", "1234").viewPreviousOrders();
        System.out.println("sale is done successfully");

        System.out.println("\nrequesting permission");
        system.getStocksChangeRequest().setPermission(request.Permission.PARKED);
        System.out.println("permission is requested successfully");

        System.out.println("\nadding without permission");
        System.out.println("permission: " + system.getStocksChangeRequest().getPermission());
        if(!system.getStocksChangeRequest().getEmployeeID().equals(employee.getID()) ||
                !system.getStocksChangeRequest().getPermission().equals(request.Permission.GRANTED)){
            System.out.println("this operation cannot be done without the permission");
        }

        System.out.println("\nadding a non-existing furniture with permission");
        system.getStocksChangeRequest().setPermission(request.Permission.GRANTED);
        System.out.println("permission: " + system.getStocksChangeRequest().getPermission());
        if(system.getStocksChangeRequest().getPermission().equals(request.Permission.GRANTED) &&
                system.getStocksChangeRequest().getEmployeeID().equals(employee.getID())){
            try {
                System.out.println("furniture index: 10, model index: 1, color index: 1, amount: 1");
                employee.addProduct(10, 1, 1, 1);
            } catch (Exception e) {
                System.out.println(e);
            }
        }

        System.out.println("\nadding an existing furniture with permission");
        System.out.println("permission: " + system.getStocksChangeRequest().getPermission());
        if(system.getStocksChangeRequest().getPermission().equals(request.Permission.GRANTED) &&
                system.getStocksChangeRequest().getEmployeeID().equals(employee.getID())){
            System.out.println("furniture index: 0, model index: 2, color index: 3, amount: 10");
            System.out.println("before adding");
            System.out.println("stock: " + employee.inquireStock(0, 2 ,3));
            employee.addProduct(0, 2, 3, 10);
            System.out.println("after adding");
            System.out.println("stock: " + employee.inquireStock(0, 2, 3));
        }

        System.out.println("\nviewing empty wishlist");
        if(system.isWishListEmpty(employee.getBranch().getBranchWishList())){
            System.out.println("wishlist is empty");
        }

        System.out.println("\nviewing non-empty wishlist");
        employee.addToWishList(1, 1, 1, 20);
        system.seeWishList(employee.getBranch().getBranchWishList());
        System.out.println("wishlist is viewed successfully");

        System.out.println("\n\nADMINISTRATOR FUNCTIONS");
        System.out.println("logging in as non-existing administrator");
        try {
            System.out.println("ID: non-existing, password: non-existing");
            admin = system.findAdministrator("non-existing", "non-existing");
        } catch (Exception e) {
            System.out.println(e);
        }

        System.out.println("\nlogging in as existing administrator");
        System.out.println("ID: 65001, password: JD10000");
        admin = system.findAdministrator("65001", "JD10000");
        System.out.println("administrator name: " + admin.getName() + " administrator surname: " + admin.getSurname());
        System.out.println("logged in successfully");

        System.out.println("\nadding branch");
        System.out.println("branch to add: newbranch");
        System.out.println("number of branch employees to add: 3");
        System.out.println("before adding");
        System.out.println("branches");
        admin.listOfBranches(system);
        System.out.println("employees");
        admin.listOfBranchEmployees(system);
        admin.addBranch("newbranch", 3, system);
        System.out.println("after adding");
        System.out.println("branches");
        admin.listOfBranches(system);
        System.out.println("employees");
        admin.listOfBranchEmployees(system);
        System.out.println("new branch is added successfully");

        System.out.println("\nremoving a non-existing branch");
        try {
            System.out.println("branch to remove: non-existing");
            admin.removeBranch("non-existing", system);
        } catch (Exception e) {
            System.out.println(e);
        }

        System.out.println("\nremoving an existing branch");
        System.out.println("branch to remove: branch1");
        System.out.println("before removal");
        System.out.println("branches");
        admin.listOfBranches(system);
        System.out.println("employees");
        admin.listOfBranchEmployees(system);
        admin.removeBranch("branch1", system);
        System.out.println("after removal");
        System.out.println("branches");
        admin.listOfBranches(system);
        System.out.println("employees");
        admin.listOfBranchEmployees(system);
        System.out.println("branch1 is removed successfully");

        System.out.println("\nadding branch employee to a non-existing branch");
        try {
            System.out.println("name of new branch employee: name");
            System.out.println("surname of new branch employee: surname");
            System.out.println("password of new branch employee: 1234");
            System.out.println("branch of new branch employee: non-existing");
            admin.addBranchEmployee("name", "surname", "1234",
                    system.getBranches().get(system.getBranchIndex("non-existing")), system);
        } catch (Exception e) {
            System.out.println(e);
        }

        System.out.println("\nadding branch employee to an existing branch");
        System.out.println("name of new branch employee: name");
        System.out.println("surname of new branch employee: surname");
        System.out.println("password of new branch employee: 1234");
        System.out.println("branch of new branch employee: branch2");
        System.out.println("before adding");
        admin.listOfBranchEmployees(system);
        admin.addBranchEmployee("name", "surname", "1234",
                system.getBranches().get(system.getBranchIndex("branch2")), system);
        System.out.println("after adding");
        admin.listOfBranchEmployees(system);
        System.out.println("a new branch employee is added to branch2 successfully");

        System.out.println("\nremoving a non-existing branch employee");
        try {
            System.out.println("ID of branch employee to remove: non-existing");
            admin.removeBranchEmployee("non-existing", system);
        } catch (Exception e) {
            System.out.println(e);
        }

        System.out.println("\nremoving an existing branch employee");
        System.out.println("ID of branch employee to remove: 66012");
        System.out.println("before removal");
        admin.listOfBranchEmployees(system);
        admin.removeBranchEmployee("66012", system);
        System.out.println("after removal");
        admin.listOfBranchEmployees(system);
        System.out.println("branch employee with ID number 66012 is removed successfully");

        System.out.println("\nquerying enough stocks");
        if(!admin.queryStocks(system)){
            System.out.println("stocks are enough");
        }

        System.out.println("\nquerying not enough stocks");
        System.out.println("initial stock: 0");
        system.getStoreHouse().changeStock(1, 1, 1, 0);
        admin.queryStocks(system);
        System.out.println("stock is filled successfully");

        System.out.println("\nresupplying empty online wishlist");
        if(system.isWishListEmpty(system.getOnlineWishList())){
            System.out.println("wishlist is empty");
        }

        System.out.println("\nresupplying non-empty online wishlist");
        system.addToWishList(1, 1, 1, 15);
        System.out.println("before resupplying");
        system.seeWishList(system.getOnlineWishList());
        admin.resupplyOnlineWishList(system);
        System.out.println("after resupplying");
        if(system.isWishListEmpty(system.getOnlineWishList())){
            System.out.println("wishlist is empty");
        }
        System.out.println("online wishlist is resupplied successfully");

        System.out.println("\nviewing requests");
        system.getStocksChangeRequest().setPermission(request.Permission.PARKED);
        system.getStocksChangeRequest().setID("66020");
        if(system.getStocksChangeRequest().getEmployeeID() != null &&
        system.getStocksChangeRequest().getPermission().equals(request.Permission.PARKED)) {
            System.out.println(system.findBranchEmployee(system.getStocksChangeRequest().getEmployeeID()).getName() + " " +
                    system.findBranchEmployee(system.getStocksChangeRequest().getEmployeeID()).getSurname() + " wants to change the stocks in " +
                    system.findBranchEmployee(system.getStocksChangeRequest().getEmployeeID()).getBranch().getName());
            system.getStocksChangeRequest().setPermission(request.Permission.GRANTED);
            System.out.println("permission: " + system.getStocksChangeRequest().getPermission());
            System.out.println(system.getStocksChangeRequest());
            system.getStocksChangeRequest().setPermission(request.Permission.DENIED);
            System.out.println("permission: " + system.getStocksChangeRequest().getPermission());
            System.out.println(system.getStocksChangeRequest());
        } else {
            System.out.println("there is no request to view");
        }

        System.out.println("\nview list of branch employees");
        admin.listOfBranchEmployees(system);
        System.out.println("branch employees are listed successfully");

        System.out.println("\nviewing stocks of all products");
        admin.stocksOfAllProducts(system);
        System.out.println("stocks of all products are viewed successfully");

        System.out.println("\nviewing empty wishlist");
        if(system.isWishListEmpty(system.getOnlineWishList())){
            System.out.println("wishlist is empty");
        }

        System.out.println("\nviewing non-empty wishlist");
        system.addToWishList(1, 1, 1, 10);
        system.seeWishList(system.getOnlineWishList());
        System.out.println("wishlist is viewed successfully");

        System.out.println("\nview list of branches");
        admin.listOfBranches(system);
        System.out.println("branches are listed successfully");

        System.out.println("Do you want to proceed with the interactive part?(y/n)");
        if(new Scanner(System.in).nextLine().equals("y")){
            system.setTest(false);
            system.runSystem();
        }
    }
}