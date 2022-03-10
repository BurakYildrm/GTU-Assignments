import java.io.*;
import java.util.*;

/**
 * Class that represents a trader.
 */
public class Trader extends User{

    /**
     * Constructs a trader with given name and password.
     * @param name name of the trader
     * @param password password of the trader
     */
    public Trader(String name, String password) {
        super(name, password);
    }

    /**
     * Prints all the orders of this trader.
     */
    public void viewOrders(){
        try {
            BufferedReader IDReader = new BufferedReader(new FileReader("orders.txt"));
            BufferedReader traderReader = new BufferedReader(new FileReader("orders.txt"));
            int counter = 0;
            String line;

            traderReader.readLine();
            while ((line = IDReader.readLine()) != null){
                if(traderReader.readLine().contains("Trader: " + name)){
                    counter++;
                    System.out.println(line);
                    for(int i = 0; i < 5; i++){
                        System.out.println(IDReader.readLine());
                        traderReader.readLine();
                    }
                    IDReader.readLine();
                    traderReader.readLine();
                    System.out.println();
                }
                else {
                    for(int i = 0; i < 6; i++){
                        IDReader.readLine();
                        traderReader.readLine();
                    }
                }
            }

            IDReader.close();
            traderReader.close();

            if(counter == 0){
                System.out.println("You don't have any order");
            }
        } catch (IOException e) {
            System.out.println("An error occurred");
        }
    }

    /**
     * Prints all the products of this trader.
     */
    public void seeProducts(){
        try {
            BufferedReader IDReader = new BufferedReader(new FileReader("products.txt"));
            BufferedReader traderReader = new BufferedReader(new FileReader("products.txt"));
            String line;
            int count = 0;

            for (int i = 0; i < 6; i++){
                traderReader.readLine();
            }

            while ((line = IDReader.readLine()) != null){
                if(traderReader.readLine().contains("Trader: " + name)){
                    count++;
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
                else {
                    for(int i = 0; i < 7; i++){
                        traderReader.readLine();
                        IDReader.readLine();
                    }
                }
            }

            IDReader.close();
            traderReader.close();
            if(count == 0){
                System.out.println("You don't have any product");
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * Sets the status of the order with given order ID.
     * @param orderID ID of the order
     * @param mode determines the status of the order(1 is met, 2 is cancelled)
     */
    public void setOrderStatus(String orderID, int mode){
        try {
            int check = 0;
            File oldFile = new File("orders.txt");
            File newFile = new File("temp.txt");
            BufferedReader br = new BufferedReader(new FileReader(oldFile));
            FileWriter fileWriter = new FileWriter(newFile);
            String line;
            while ((line = br.readLine()) != null){
                if(line.contains("ID: " + orderID)){
                    check++;
                    fileWriter.write(line + "\n");
                    for(int i = 0; i < 4; i++){
                        fileWriter.write(br.readLine() + "\n");
                    }
                    br.readLine();
                    if(mode == 1){
                        fileWriter.write("Status: Met\n");
                    }
                    else{
                        fileWriter.write("Status: Cancelled\n");
                    }
                    fileWriter.write(br.readLine() + "\n");
                }
                else{
                    fileWriter.write(line + "\n");
                }
            }
            br.close();
            fileWriter.close();
            if(check == 0){
                newFile.delete();
                throw new NoSuchElementException();
            }
            oldFile.delete();
            newFile.renameTo(new File("orders.txt"));
        } catch (IOException e) {
            System.out.println("An error occurred");
        }
    }

    /**
     * Adds new product to products.txt
     * @param productName name of the product
     * @param productCategory category of the product
     * @param productPrice price of the product
     * @param productDiscountedPrice discounted price of the product
     * @param productDescription description of the product
     */
    public void addProduct(String productName, String productCategory, String productPrice,
                           String productDiscountedPrice, String productDescription){
        try {
            FileWriter fileWriter = new FileWriter("products.txt", true);
            fileWriter.write("ID: " + generateID() + "\n");
            fileWriter.write("Name: " + productName + "\n");
            fileWriter.write("Category: " + productCategory + "\n");
            fileWriter.write("Price: " + productPrice + "\n");
            fileWriter.write("Discounted Price: " + productDiscountedPrice + "\n");
            fileWriter.write("Description: " + productDescription + "\n");
            fileWriter.write("Trader: " + name + "\n\n");
            fileWriter.close();
        } catch (IOException e) {
            System.out.println("An error occurred");
        }
    }

    /**
     * Generates random sixteen-character product ID.
     * @return generated product ID
     */
    private String generateID(){
        File file = new File("products.txt");
        String chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        StringBuilder s = new StringBuilder();

        do {
            for (int i = 0; i < 16; i++) {
                s.append(chars.charAt((int)(chars.length() * Math.random())));
            }
        } while (!isIDUnique(file, s.toString()));

        return s.toString();
    }

    /**
     * Checks if the ID is unique.
     * @param file products.txt file
     * @param target ID to check
     * @return true if the ID is unique
     */
    private boolean isIDUnique(File file, String target){
        try {
            BufferedReader br = new BufferedReader(new FileReader(file));
            String line;

            while ((line = br.readLine()) != null){
                if(line.contains(target)){
                    return false;
                }
                for(int i = 0; i < 7; i++){
                    br.readLine();
                }
            }

            br.close();
        } catch (IOException e) {
            System.out.println("An error occurred");
        }

        return true;
    }

    /**
     * Removes the product with given ID.
     * @param ID ID of the product
     * @throws NoSuchElementException if there is no product with given ID
     */
    public void removeProduct(String ID){
        try {
            File oldFile = new File("products.txt");
            File newFile = new File("temp.txt");
            BufferedReader br = new BufferedReader(new FileReader(oldFile));
            FileWriter fileWriter = new FileWriter(newFile);
            int check = 0;
            String line;
            while ((line = br.readLine()) != null){
                if(line.contains("ID: " + ID)){
                    check++;
                    for(int i = 0; i < 7; i++){
                        br.readLine();
                    }
                }
                else {
                    fileWriter.write(line + "\n");
                }
            }
            br.close();
            fileWriter.close();
            if(check == 0){
                newFile.delete();
                throw new NoSuchElementException();
            }
            oldFile.delete();
            newFile.renameTo(new File("products.txt"));
        } catch (IOException e) {
            System.out.println("An error occurred");
        }
    }

    /**
     * Edits the product with given ID.
     * @param ID ID of the product
     * @param newData new data
     * @param mode determines which attribute is going to be edited(1 for name, 2 for category, 3 for price, 4 for discounted price, 5 for description)
     * @throws NoSuchElementException if there is no product with given ID
     */
    public void editProduct(String ID, String newData, int mode){
        ArrayList<String> attributeNames = new ArrayList<>();
        attributeNames.add("Name");
        attributeNames.add("Category");
        attributeNames.add("Price");
        attributeNames.add("Discounted Price");
        attributeNames.add("Description");

        try {
            File oldFile = new File("products.txt");
            File newFile = new File("temp.txt");
            BufferedReader br = new BufferedReader(new FileReader(oldFile));
            FileWriter fileWriter = new FileWriter(newFile);
            int check = 0;
            String line;
            while ((line = br.readLine()) != null){
                //System.out.println(line + " target ID: " + ID);
                if(line.contains("ID: " + ID)){
                    check++;
                    fileWriter.write(line + "\n");
                    for(int i = 0; i < mode - 1; i++){
                        fileWriter.write(br.readLine() + "\n");
                    }
                    br.readLine();
                    fileWriter.write(attributeNames.get(mode - 1) + ": " + newData + "\n");
                    for (int i = 0; i < 7 - mode; i++){
                        fileWriter.write(br.readLine() + "\n");
                    }
                }
                else {
                    fileWriter.write(line + "\n");
                }
            }
            br.close();
            fileWriter.close();
            if(check == 0){
                newFile.delete();
                throw new NoSuchElementException();
            }
            oldFile.delete();
            newFile.renameTo(new File("products.txt"));
        } catch (IOException e) {
            System.out.println("An error occurred");
        }
    }
}
