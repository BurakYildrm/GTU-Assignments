/**
 * class that represents one store house
 */
public class storeHouse implements requiredFields {
    private HybridList<stock> allProducts;
    /**
     * constructs a new store house with given parameters
     * @param officeChair stock of office chairs
     * @param officeDesk stock of office desks
     * @param meetingTable stock of meeting tables
     * @param bookCase stock of bookcases
     * @param officeCabinet stock of office cabinets
     */
    public storeHouse(int[][] officeChair, int[][] officeDesk, int[][] meetingTable,
                      int[][] bookCase, int[][] officeCabinet){
        allProducts = new HybridList<>();
        furniture[] temp = new furniture[5];
        temp[0] = new office_chair(officeChair);
        temp[1] = new office_desk(officeDesk);
        temp[2] = new meeting_table(meetingTable);
        temp[3] = new book_case(bookCase);
        temp[4] = new office_cabinet(officeCabinet);
        for (furniture i : temp) {
            allProducts.addAll(i.getModels());
        }
    }

    /**
     * returns the stock info about chosen product in store house
     * @param productName name index of the product
     * @param productModel model number of the product
     * @param productColor color number of the product
     * @return stock info about chosen product
     */
    public int getStockInfo(int productName, int productModel, int productColor){
        return allProducts.get(productName, productModel).get(productColor);
    }

    /**
     * changes the stock of chosen product in store house
     * @param productName name index of the product
     * @param productModel model number of the product
     * @param productColor color number of the product
     * @param num new stock of the chosen product
     */
    public void changeStock(int productName, int productModel, int productColor, int num){
        allProducts.get(productName, productModel).set(productColor, num);
    }
}
