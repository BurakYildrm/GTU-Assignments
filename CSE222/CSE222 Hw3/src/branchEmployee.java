/**
 * class that represents one branch employee
 */
public class branchEmployee extends user implements requiredFields {
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
        Branch.getBranchWishList()[name][model][color] += num;
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
    public void sale(customer Customer, int productName, int productModel, int productColor, int num, automation_system system){
        int initialStock = Branch.getStockInfo(productName, productModel, productColor);
        if(num > inquireStock(productName, productModel, productColor)){
            Branch.changeStock(productName, productModel, productColor, 0);
            addToWishList(productName, productModel, productColor, num - initialStock);
            Customer.addNewOrder(productName, productModel + 1, productColor + 1, num, true);
            System.out.println("Due to stock issues right now we can only provide "
                    + initialStock + " of the ordered furniture to the customer but we will send the rest after resupplying the furniture");
        }
        else{
            Branch.changeStock(productName, productModel, productColor, initialStock - num);
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
    public void addProduct(int name, int model, int color, int num){
        int initialStock = Branch.getStockInfo(name, model, color);
        Branch.changeStock(name, model, color, initialStock + num);
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