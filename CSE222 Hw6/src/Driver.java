import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.*;

public class Driver {
    public static void main(String[] args) {
        EShoppingApplication app = new EShoppingApplication(true);
        Customer customer = new Customer("customer", "customer");
        Trader trader = new Trader("trader", "trader");
        System.out.println("----Customer Methods----");
        System.out.println("searching a non-existing product");
        try {
            customer.searchProduct("non-existing", app);
        } catch (NoSuchElementException e) {
            System.out.println(e);
        }

        System.out.println("\nsearching an existing product(Sicons)");
        Queue<Product> searchResults = customer.searchProduct("Sicons", app);
        while (!searchResults.isEmpty()){
            System.out.print(searchResults.poll());
        }

        System.out.println("sorting search result by name in increasing order");
        ArrayList<Product> sorted = customer.sortSearchResults(1, 1);
        for(Product product : sorted){
            System.out.print(product);
        }

        System.out.println("sorting search result by name in decreasing order");
        sorted = customer.sortSearchResults(1, -1);
        for(Product product : sorted){
            System.out.print(product);
        }

        System.out.println("sorting search result by price in increasing order");
        sorted = customer.sortSearchResults(2, 1);
        for(Product product : sorted){
            System.out.print(product);
        }

        System.out.println("sorting search result by price in decreasing order");
        sorted = customer.sortSearchResults(2, -1);
        for(Product product : sorted){
            System.out.print(product);
        }

        System.out.println("sorting search result by discount percentage in increasing order");
        sorted = customer.sortSearchResults(3, 1);
        for(Product product : sorted){
            System.out.print(product);
        }

        System.out.println("sorting search result by discount percentage in decreasing order");
        sorted = customer.sortSearchResults(3, -1);
        for(Product product : sorted){
            System.out.print(product);
        }

        System.out.println("filtering search results by a non-existing category");
        try {
            ArrayList<Product> filtered = customer.filterByCategory("non-existing");
        } catch (NoSuchElementException e) {
            System.out.println(e);
        }

        System.out.println("\nfiltering search results by an existing category(Sicons All Purpose Arnica Dog Shampoo (500 ml))");
        ArrayList<Product> filtered = customer.filterByCategory("Sicons All Purpose Arnica Dog Shampoo (500 ml)");
        for(Product product : filtered){
            System.out.print(product);
        }

        System.out.println("filtering search results by price(100 - 200)");
        filtered = customer.filterByPrice(200, 100);
        for(Product product : filtered){
            System.out.print(product);
        }

        System.out.println("viewing all the products of a non-existing trader");
        try {
            customer.viewTraderProduct("non-existing");
        } catch (NoSuchElementException e) {
            System.out.println(e);
        }

        System.out.println("\nviewing all the products of an existing trader(Alisha)");
        customer.viewTraderProduct("Alisha");

        System.out.println("ordering a non-existing product");
        try {
            customer.orderProduct("non-existing", 1, app);
        } catch (NoSuchElementException e) {
            System.out.println(e);
        }

        System.out.println("\nordering an existing product(ID: SRTEH2FF9KEDEFGF)");
        customer.orderProduct("SRTEH2FF9KEDEFGF", 2, app);
        System.out.println("order");
        customer.viewPreviousOrders();

        System.out.println("viewing previous orders");
        customer.viewPreviousOrders();

        System.out.println("\n----Trader Methods----");
        System.out.println("adding product");
        System.out.println("before adding");
        trader.seeProducts();
        System.out.println("after adding");
        trader.addProduct("new product", "new category", "2", "1",
                "new description");
        trader.seeProducts();
        String ID = "";
        try {
            BufferedReader IDReader = new BufferedReader(new FileReader("products.txt"));
            BufferedReader nameReader = new BufferedReader(new FileReader("products.txt"));
            nameReader.readLine();
            while ((ID = IDReader.readLine()) != null){
                if(nameReader.readLine().contains("Name: new product")){
                    break;
                }
                else{
                    for(int i = 0; i < 7; i++){
                        IDReader.readLine();
                        nameReader.readLine();
                    }
                }
            }
            IDReader.close();
            nameReader.close();
            ID = ID.split(": ")[1];
        } catch (IOException e) {
            System.out.println("An error occurred");
        }

        System.out.println("editing a non-existing product");
        try {
            trader.editProduct("non-existing", "non-existing", 1);
        } catch (NoSuchElementException e) {
            System.out.println(e);
        }

        System.out.println("\nediting an existing product's name(" + ID + ")");
        System.out.println("before editing");
        trader.seeProducts();
        trader.editProduct(ID, "edited name", 1);
        System.out.println("after editing");
        trader.seeProducts();

        System.out.println("editing an existing product's category(" + ID + ")");
        System.out.println("before editing");
        trader.seeProducts();
        trader.editProduct(ID, "edited category", 2);
        System.out.println("after editing");
        trader.seeProducts();

        System.out.println("editing an existing product's price(" + ID + ")");
        System.out.println("before editing");
        trader.seeProducts();
        trader.editProduct(ID, "10000000", 3);
        System.out.println("after editing");
        trader.seeProducts();

        System.out.println("editing an existing product's discounted price(" + ID + ")");
        System.out.println("before editing");
        trader.seeProducts();
        trader.editProduct(ID, "99999", 4);
        System.out.println("after editing");
        trader.seeProducts();

        System.out.println("editing an existing product's description(" + ID + ")");
        System.out.println("before editing");
        trader.seeProducts();
        trader.editProduct(ID, "edited description", 5);
        System.out.println("after editing");
        trader.seeProducts();

        customer.orderProduct(ID, 2, app);
        customer.orderProduct(ID, 5, app);
        System.out.println("viewing orders(added 2 orders for testing)");
        trader.viewOrders();

        System.out.println("meeting a non-existing order");
        try {
            trader.setOrderStatus("non-existing", 1);
        } catch (NoSuchElementException e) {
            System.out.println(e);
        }

        System.out.println("\nmeeting an existing order(ID: 00000002)");
        System.out.println("before meeting");
        trader.viewOrders();
        System.out.println("after meeting");
        trader.setOrderStatus("00000002", 1);
        trader.viewOrders();

        System.out.println("\ncancelling a non-existing order");
        try {
            trader.setOrderStatus("non-existing", 2);
        } catch (NoSuchElementException e) {
            System.out.println(e);
        }

        System.out.println("\ncancelling an existing order(ID: 00000003)");
        System.out.println("before cancelling");
        trader.viewOrders();
        System.out.println("after cancelling");
        trader.setOrderStatus("00000003", 2);
        trader.viewOrders();

        System.out.println("viewing products of this trader");
        trader.seeProducts();

        System.out.println("removing a non-existing product");
        try {
            trader.removeProduct("non-existing");
        } catch (NoSuchElementException e) {
            System.out.println(e);
        }

        System.out.println("\nremoving an existing product(" + ID + ")");
        System.out.println("before removal");
        trader.seeProducts();
        trader.removeProduct(ID);
        System.out.println("after removal");
        trader.seeProducts();

        System.out.println("Do you want to continue with the interactive part(y/n)");
        Scanner scanner = new Scanner(System.in);
        String yn = scanner.nextLine();
        if(yn.equals("y")){
            app = new EShoppingApplication(false);
            app.runApplication();
        }
    }
}
