public class order {
    private String product;
    private int productModel;
    private int productColor;
    private int number;
    private boolean willBeSupplied;
    /**
     * @return color value of the order
     */
    public int getProductColor() {
        return productColor;
    }

    /**
     * @return willBeSupplied value of the order
     */
    public boolean getWillBeSupplied() {
        return willBeSupplied;
    }

    /**
     * @return furniture name of the order
     */
    public String getProduct() {
        return product;
    }


    /**
     * @return number of the order
     */
    public int getNumber() {
        return number;
    }

    /**
     * @return model value of the order
     */

    public int getProductModel() {
        return productModel;
    }


    /**
     * @param willBeSupplied sets the value of willBeSupplied of the order
     */
    public void setWillBeSupplied(boolean willBeSupplied) {
        this.willBeSupplied = willBeSupplied;
    }

    public order(String product, int productModel, int productColor, int number, boolean willBeSupplied) {
        this.product = product;
        this.number = number;
        this.productModel = productModel;
        this.productColor = productColor;
        this.willBeSupplied = willBeSupplied;
    }

    /**
     * Returns different states according to the parameter
     * @param val value that determines the state of the order
     * @return state of order
     */
    protected String status(boolean val){
        if(val){
            return "waiting for supplies";
        }
        return "completed";
    }

    @Override
    public String toString() {
        return "product = " + product +
                ", productModel = Model" + productModel +
                ", productColor = Color" + productColor +
                ", number = " + number +
                ", status = " + status(willBeSupplied);
    }
}
class online extends order{

    private long phoneNum;
    private String address;
    public online(String product, int productModel, int productColor, int number, long phoneNum, String address, boolean willBeSupplied) {
        super(product, productModel, productColor, number, willBeSupplied);
        this.phoneNum = phoneNum;
        this.address = address;
    }

    @Override
    public String toString() {
        return "product = " + getProduct() +
                ", productModel = Model" + getProductModel() +
                ", productColor = Color" + getProductColor() +
                ", number = " + getNumber() +
                ", phoneNum = " + phoneNum +
                ", address = " + address +
                ", status = " + status(getWillBeSupplied());
    }
}