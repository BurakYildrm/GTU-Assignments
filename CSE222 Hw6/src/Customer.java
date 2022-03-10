import java.io.*;
import java.util.*;

/**
 * Class that represents a customer.
 */
public class Customer extends User{

    private ArrayList<Product> searchResults;
    private HashSet<String> searchCategories;

    /**
     * Constructs a customer with given name and password.
     * @param name name of the customer
     * @param password password of the customer
     */
    public Customer(String name, String password) {
        super(name, password);
        searchResults = new ArrayList<>();
        searchCategories = new HashSet<>();
    }

    /**
     * Orders the product with given ID as much as given amount.
     * @param ID ID of the product to order
     * @param amount amount of order
     * @param app e-shopping application
     * @throws NoSuchElementException if the ID is not found in products.txt
     */
    public void orderProduct(String ID, int amount, EShoppingApplication app){
        try {
            Product product = findProductByID(ID, app);
            FileWriter fileWriter = new FileWriter("orders.txt", true);
            fileWriter.write("ID: " + generateOrderID() + "\n");
            fileWriter.write("Trader: " + product.getTrader().getName() + "\n");
            fileWriter.write("Customer: " + name + "\n");
            fileWriter.write("Product: " + product.getName() + "\n");
            fileWriter.write("Amount: " + amount + "\n");
            fileWriter.write("Status: Pending\n\n");
            fileWriter.close();
        } catch (IOException e) {
            System.out.println("An error occurred");
        }
    }

    /**
     * Returns the product with given ID.
     * @param ID ID to search
     * @param app e-shopping application
     * @throws NoSuchElementException if the ID is not found in products.txt
     * @return product with given ID
     */
    private Product findProductByID(String ID, EShoppingApplication app){
        try {
            BufferedReader br = new BufferedReader(new FileReader("products.txt"));
            String line;
            while ((line = br.readLine()) != null){
                if(line.contains("ID: " + ID)){
                    break;
                }
                else {
                    for (int i = 0; i < 7; i++) {
                        br.readLine();
                    }
                }
            }
            if(line == null) throw new NoSuchElementException();
            Product product = new Product(line.split(": ")[1], br.readLine().split(": ")[1], br.readLine().split(": ")[1],
                    Integer.parseInt(br.readLine().split(": ")[1]), Integer.parseInt(br.readLine().split(": ")[1]),
                    br.readLine().split(": ")[1], findTrader(br.readLine().split(": ")[1], app));
            br.close();
            return product;
        } catch (IOException e) {
            System.out.println("An error occurred");
            return null;
        }
    }

    /**
     * Generates order ID for new orders.
     * @return order ID
     */
    private String generateOrderID(){
        try {
            int counter = 0;
            BufferedReader br = new BufferedReader(new FileReader("orders.txt"));
            while (br.readLine() != null){
                counter++;
            }
            counter /= 7;
            String orderNum = String.valueOf(++counter);
            StringBuilder s = new StringBuilder();
            for(int i = 0; i < 8 - orderNum.length(); i++){
                s.append(0);
            }
            s.append(orderNum);
            br.close();
            return s.toString();
        } catch (IOException e) {
            System.out.println("An error occurred");
            return null;
        }
    }

    /**
     * Searches the products.txt for given target phrase.
     * @param target target phrase
     * @param app e-shopping application
     * @return a list of products which contains given target phrase in their names or descriptions
     */
    public Queue<Product> searchProduct(String target, EShoppingApplication app){
        try {
            int check = 0;
            searchResults.clear();
            searchCategories.clear();
            BufferedReader nameReader = new BufferedReader(new FileReader("products.txt"));
            BufferedReader descriptionReader = new BufferedReader(new FileReader("products.txt"));
            BufferedReader IDReader = new BufferedReader(new FileReader("products.txt"));
            String IDLine;
            String nameLine;
            String descriptionLine;

            nameReader.readLine();
            for(int i = 0; i < 5; i++){
                descriptionReader.readLine();
            }

            while ((IDLine = IDReader.readLine()) != null){
                nameLine = nameReader.readLine();
                descriptionLine = descriptionReader.readLine();
                if(nameLine.contains(target) || descriptionLine.contains(target)){
                    check++;
                    String[] ID = IDLine.split(": ");
                    String[] name = IDReader.readLine().split(": ");
                    String[] category = IDReader.readLine().split(": ");
                    String[] price = IDReader.readLine().split(": ");
                    String[] discountedPrice = IDReader.readLine().split(": ");
                    String[] description = IDReader.readLine().split(": ");
                    String[] trader = IDReader.readLine().split(": ");
                    searchResults.add(new Product(ID[1], name[1], category[1], Integer.parseInt(price[1]),
                            Integer.parseInt(discountedPrice[1]), description[1], findTrader(trader[1], app)));
                    IDReader.readLine();
                    for(int i = 0; i < 7; i++){
                        nameReader.readLine();
                        descriptionReader.readLine();
                    }
                    searchCategories.addAll(Arrays.asList(category[1].split(" >> ")));
                }
                else {
                    for(int i = 0; i < 7; i++){
                        IDReader.readLine();
                        nameReader.readLine();
                        descriptionReader.readLine();
                    }
                }
            }
            IDReader.close();
            nameReader.close();
            descriptionReader.close();
            if(check == 0){
                throw new NoSuchElementException();
            }
            return new PriorityQueue<>(searchResults);
        } catch (IOException e) {
            System.out.println("An error occurred");
            return null;
        }
    }

    /**
     * Finds the trader with given name.
     * @param name name of the trader
     * @param app e-shopping application
     * @return the trader with given name
     */
    private Trader findTrader(String name, EShoppingApplication app){
        Set<String> keys = app.getTraders().keySet();
        for(String key : keys){
            if(app.getTraders().get(key).getName().equals(name)){
                return app.getTraders().get(key);
            }
        }
        return null;
    }

    /**
     * Sorts the search results.
     * @param mode determines the sort mode(1 for name, 2 for price, 3 for discount percentage)
     * @param order determines the order of sort(1 for increasing order, -1 for decreasing order)
     * @return sorted search results
     */
    public ArrayList<Product> sortSearchResults(int mode, int order){
        ArrayList<Product> sortedResults = new ArrayList<>(searchResults);

        if(mode == 1){
            sortedResults = new ArrayList<>(mergeSort(new LinkedList<>(sortedResults), order));
        }
        else if(mode == 2){
            shellSort(sortedResults, order);
        }
        else if(mode == 3){
            quickSort(sortedResults, 0, sortedResults.size() - 1, order);
        }

        return sortedResults;
    }

    /**
     * Merge sorts the search products by their name in given order.
     * @param array search results
     * @param order sort order
     * @return sorted search results
     */
    private LinkedList<Product> mergeSort(LinkedList<Product> array, int order){
        if(array.size() == 1){
            return array;
        }

        int half = array.size()/2;
        LinkedList<Product> arrayOne = new LinkedList<>(array.subList(0, half));
        LinkedList<Product> arrayTwo = new LinkedList<>(array.subList(half, array.size()));

        arrayOne = mergeSort(arrayOne, order);
        arrayTwo = mergeSort(arrayTwo, order);

        return merge(arrayOne, arrayTwo, order);
    }

    /**
     * Merges two given list in given order.
     * @param a first list
     * @param b second list
     * @param order merge order
     * @return merged list
     */
    private LinkedList<Product> merge(LinkedList<Product> a, LinkedList<Product> b, int order){
        LinkedList<Product> c = new LinkedList<>();

        while (!a.isEmpty() && !b.isEmpty()){
            if(a.get(0).compareTo(b.get(0))*order*-1 < 0){
                c.add(a.remove(0));
            }
            else{
                c.add(b.remove(0));
            }
        }

        while (!a.isEmpty()){
            c.add(a.remove(0));
        }

        while (!b.isEmpty()){
            c.add(b.remove(0));
        }

        return c;
    }

    /**
     * Shell sorts the search products by their price in given order.
     * @param array search results
     * @param order sort order
     */
    private void shellSort(ArrayList<Product> array, int order) {
        int gap = array.size()/2;

        while (gap > 0) {
            for (int i = gap; i < array.size(); i++) {
                int nextPos = i;
                Product nextVal = array.get(nextPos);
                while (nextPos > gap - 1 && array.get(nextPos - gap).getPrice()*order >= nextVal.getPrice()*order) {
                    array.set(nextPos, array.get(nextPos - gap));
                    nextPos -= gap;
                }
                array.set(nextPos, nextVal);
            }
            if(gap == 2){
                gap = 1;
            }
            else{
                gap /= 2.2;
            }
        }
    }

    /**
     * Quick sorts the search products by their discount percentage in given order.
     * @param array array to sort
     * @param low start index of the array
     * @param high end index of the array
     * @param order sort order
     */
    private void quickSort(ArrayList<Product> array, int low, int high, int order){
        if(low < high){
            int pivotPos = partition(array, low, high, order);
            quickSort(array, low, pivotPos - 1, order);
            quickSort(array, pivotPos + 1, high, order);
        }
    }

    /**
     * Partitions the given array.
     * @param array array to partition
     * @param low start index of the array
     * @param high end index of the array
     * @param order sort order
     * @return the pivot index
     */
    private int partition(ArrayList<Product> array, int low, int high, int order){
        Product pivot = array.get(high);
        Product temp;
        int smallerIndex = low;

        for(int i = low; i < high; i++){
            if(discountPercentage(array.get(i))*order < discountPercentage(pivot)*order){
                temp = array.set(i, array.get(smallerIndex));
                array.set(smallerIndex, temp);
                smallerIndex++;
            }
        }

        array.set(high, array.get(smallerIndex));
        array.set(smallerIndex, pivot);
        return smallerIndex;
    }

    /**
     * Returns the discount percentage of the given product.
     * @param product product to calculate discount percentage
     * @return the discount percentage of the given product
     */
    private double discountPercentage(Product product){
        double discount = product.getPrice() - product.getDiscountedPrice();
        return discount*100/product.getPrice();
    }

    /**
     * Filters the search results by given category.
     * @param category category to sort
     * @return a list contains filtered products
     */
    public ArrayList<Product> filterByCategory(String category){
        if(!searchCategories.contains(category)){
            throw new NoSuchElementException();
        }

        CategoryTree categoryTree = new CategoryTree();

        for(Product product : searchResults){
            categoryTree.add(product);
        }

        return categoryTree.get(findCategoryOrder(category));
    }

    /**
     * Finds(if present) the upper categories of the given category.
     * @param category category
     * @return the given category with its upper categories
     */
    private String findCategoryOrder(String category){
        StringBuilder s = new StringBuilder();

        for(Product product : searchResults){
            if(product.getCategory().contains(category)){
                String[] arr = product.getCategory().split(" >> ");
                for(int i = 0; !arr[i].equals(category); i++){
                    s.append(arr[i]).append(" >> ");
                }
                break;
            }
        }

        s.append(category);
        return s.toString();
    }

    /**
     * Filters the search result by price.
     * @param upperLimit max value of the product's price
     * @param lowerLimit min value of the product's price
     * @return a list contains filtered products
     */
    public ArrayList<Product> filterByPrice(int upperLimit, int lowerLimit){
        ArrayList<Product> filteredResults = new ArrayList<>();

        for(Product product : searchResults){
            int price = product.getPrice();
            if(price >= lowerLimit && price <= upperLimit){
                filteredResults.add(product);
            }
        }

        return filteredResults;
    }

    /**
     * Prints all the products of the trader with given name.
     * @param traderName name of the trader
     * @throws NoSuchElementException if there is no product with given name as its trader
     */
    public void viewTraderProduct(String traderName){
        try {
            BufferedReader IDReader = new BufferedReader(new FileReader("products.txt"));
            BufferedReader traderReader = new BufferedReader(new FileReader("products.txt"));
            String line;
            int check = 0;

            for(int i = 0; i < 6; i++){
                traderReader.readLine();
            }

            while ((line = IDReader.readLine()) != null){
                if(traderReader.readLine().contains(traderName)){
                    check++;
                    System.out.println(line);
                    for(int i = 0; i < 5; i++){
                        System.out.println(IDReader.readLine());
                        traderReader.readLine();
                    }
                    for(int i = 0; i < 2;i++) {
                        traderReader.readLine();
                        IDReader.readLine();
                    }
                    System.out.println();
                }
                else{
                    for(int i = 0; i < 7; i++){
                        traderReader.readLine();
                        IDReader.readLine();
                    }
                }
            }

            IDReader.close();
            traderReader.close();

            if(check == 0){
                throw new NoSuchElementException();
            }
        } catch (IOException e) {
            System.out.println("An error occurred");
        }
    }

    /**
     * Prints the previous orders of this customer.
     */
    public void viewPreviousOrders(){
        try {
            BufferedReader IDReader = new BufferedReader(new FileReader("orders.txt"));
            BufferedReader customerReader = new BufferedReader(new FileReader("orders.txt"));
            String line;
            int count = 0;

            for(int i = 0; i < 2; i++){
                customerReader.readLine();
            }

            while ((line = IDReader.readLine()) != null){
                if(customerReader.readLine().contains("Customer: " + name)){
                    count++;
                    System.out.println(line);
                    for(int i = 0; i < 5; i++){
                        System.out.println(IDReader.readLine());
                        customerReader.readLine();
                    }
                    IDReader.readLine();
                    customerReader.readLine();
                    System.out.println();
                }
                else{
                    for(int i = 0; i < 6; i++){
                        IDReader.readLine();
                        customerReader.readLine();
                    }
                }
            }

            IDReader.close();
            customerReader.close();

            if(count == 0){
                System.out.println("You don't have any previous order");
            }
        } catch (IOException e) {
            System.out.println("An error is occurred");
        }
    }
}
