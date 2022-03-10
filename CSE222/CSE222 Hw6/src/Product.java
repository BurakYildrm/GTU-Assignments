/**
 * Class that represents a product.
 */
public class Product implements Comparable<Product> {
    private String ID;
    private String name;
    private String category;
    private int price;
    private int discountedPrice;
    private String description;
    private Trader trader;

    /**
     * Constructs a product with given parameters.
     * @param ID ID of the product
     * @param name name of the product
     * @param category category of the product
     * @param price price of the product
     * @param discountedPrice discounted price of the product
     * @param description description of the product
     * @param trader trader of the product
     */
    public Product(String ID, String name, String category, int price, int discountedPrice, String description, Trader trader) {
        this.ID = ID;
        this.name = name;
        this.category = category;
        this.price = price;
        this.discountedPrice = discountedPrice;
        this.description = description;
        this.trader = trader;
    }

    /**
     * Returns the name of this product.
     * @return name of this product
     */
    public String getName() {
        return name;
    }

    /**
     * Returns the ID of this product.
     * @return ID of this product
     */
    public String getID() {
        return ID;
    }

    /**
     * Returns the description of this product.
     * @return description of this product
     */
    public String getDescription() {
        return description;
    }

    /**
     * Returns the trader of this product.
     * @return trader of this product
     */
    public Trader getTrader() {
        return trader;
    }

    /**
     * Returns the category of this product.
     * @return category of this product
     */
    public String getCategory() {
        return category;
    }

    /**
     * Returns the price of this product.
     * @return price of this product
     */
    public int getPrice() {
        return price;
    }

    /**
     * Returns the discounted price of this product.
     * @return discounted price of this product
     */
    public int getDiscountedPrice() {
        return discountedPrice;
    }

    /**
     * Sets the name of this product.
     * @param name new name
     */
    public void setName(String name) {
        this.name = name;
    }

    /**
     * Sets the description of this product.
     * @param description new description
     */
    public void setDescription(String description) {
        this.description = description;
    }

    /**
     * Sets the category of this product.
     * @param category new category
     */
    public void setCategory(String category) {
        this.category = category;
    }

    /**
     * Sets the price of this product.
     * @param price new price
     */
    public void setPrice(int price) {
        this.price = price;
    }

    /**
     * Sets the discounted price of this product.
     * @param discountedPrice new discounted price
     */
    public void setDiscountedPrice(int discountedPrice) {
        this.discountedPrice = discountedPrice;
    }

    /**
     * Returns the negative of the result of the comparison between two products' names.
     * @param o product to compare to
     * @return negative of the result of the comparison between two products' names
     */
    @Override
    public int compareTo(Product o) {
        return name.compareTo(o.name)*-1;
    }

    /**
     * Returns a string representation of this product.
     * @return a string representation of this product
     */
    @Override
    public String toString() {
        return "ID: " + ID + "\nName: " + name + "\nCategory: " + category + "\nPrice: " + price +
                "\nDiscounted Price: " + discountedPrice + "\nDescription: " + description + "\nTrader: " + trader.getName() + "\n\n";
    }
}
