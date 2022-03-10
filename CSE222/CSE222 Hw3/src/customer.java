/**
 * class that represents one customer
 */
public class customer extends user implements requiredFields {
    private String email;
    private KWArraylist<order> previousOrders;

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
    public order getPreviousOrder(int index) {
        return previousOrders.get(index);
    }

    public KWArraylist<order> getAllPreviousOrders(){
        return previousOrders;
    }


    /**
     * @return number of previous orders
     */
    public int getPreviousOrdersNum() {
        return previousOrders.size();
    }

    /**
     * constructs a customer with given parameters
     * @param name name of the customer
     * @param surname surname of the customer
     * @param email email of the customer
     * @param password password of the customer
     */
    public customer(String name, String surname, String email, String password, String ID) {
        super(name, surname, ID, password);
        this.email = email;
        previousOrders = new KWArraylist<>();
    }

    /**
     * Adds new order to previousOrders array
     * @param product name of the product
     * @param productModel model number of the product
     * @param productColor color number of the product
     * @param num demanded amount of the product
     * @param willBeSupplied specifies if the requested amount is more than the stocks
     */
    public void addNewOrder(int product, int productModel, int productColor, int num, boolean willBeSupplied){
        previousOrders.add(new order(productNames[product], productModel, productColor, num, willBeSupplied));
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
    public void addNewOrder(int product, int productModel, int productColor, int num, long phoneNum, String address, boolean willBeSupplied){
        previousOrders.add(new online(productNames[product], productModel, productColor, num, phoneNum, address, willBeSupplied));
    }

    /**
     * Searches chosen product across all branches or online stocks
     * @param productName name of the product
     * @param productModel model number of the product
     * @param productColor color number of the product
     * @param mode determines if the search will be done across branches or online stocks
     */
    public void searchProducts(int productName, int productModel, int productColor, int mode, automation_system system){
        if(mode == 2){
            for(branch Branch : system.getBranches()){
                System.out.println(Branch.getName() + " has " + Branch.getStockInfo(productName, productModel, productColor) +
                        " of chosen product in its stocks");
            }
        }
        else{
            System.out.println("Online stock of chosen product is " +
                    system.getStoreHouse().getStockInfo(productName, productModel, productColor));
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
    public void shopOnline(int productName, int productModel, int productColor, int num, String address, long phoneNo, automation_system system){
        boolean result;
        result = system.reduceFromStock(productName, productModel - 1, productColor - 1, num);
        addNewOrder(productName, productModel, productColor, num, phoneNo, address, result);
    }

    /**
     * prints all the previous orders
     */
    public void viewPreviousOrders(){
        System.out.println("Previous Orders(new to old)");
        if(previousOrders.size() == 0){
            System.out.println("There is no previous order to view");
        }
        for (int i = previousOrders.size() - 1; i >= 0; i--){
            System.out.println(previousOrders.get(i));
        }
    }
}
