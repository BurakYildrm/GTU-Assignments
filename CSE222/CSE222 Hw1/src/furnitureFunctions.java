public interface furnitureFunctions {
    /**
     * changes the given product's stock
     * @param model model number of the product
     * @param color color number of the product
     * @param stock stock that will be set
     */
    void changeStock(int model, int color, int stock);
    /**
     * assigns given stocks
     * @param stocks stocks that will br assigned
     */
    void assignStocks(int[][] stocks);
    /**
     * returns the stock info of given product
     * @param model model number of the product
     * @param color color number of the product
     * @return stock info of given product
     */
    int stockInfo(int model, int color);
    int OFFICE_CHAIR_MODEL_NUM = 7;
    int OFFICE_CHAIR_COLOR_NUM = 5;
    int OFFICE_DESK_MODEL_NUM = 5;
    int OFFICE_DESK_COLOR_NUM = 4;
    int MEETING_TABLE_MODEL_NUM = 10;
    int MEETING_TABLE_COLOR_NUM = 4;
    int BOOK_CASE_MODEL_NUM = 12;
    int BOOK_CASE_COLOR_NUM = 1;
    int OFFICE_CABINET_MODEL_NUM = 12;
    int OFFICE_CABINET_COLOR_NUM = 1;
}
