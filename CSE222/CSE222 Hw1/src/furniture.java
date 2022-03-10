public class furniture implements furnitureFunctions{
    int[][] models;

    public furniture(int[][] stocks) {
        models = new int[stocks.length][stocks[0].length];
        assignStocks(stocks);
    }

    /**
     * changes the given product's stock
     * @param model model number of the product
     * @param color color number of the product
     * @param stock stock that will be set
     */
    @Override
    public void changeStock(int model, int color, int stock) {
        models[model][color] = stock;
    }

    /**
     * assigns given stocks
     * @param stocks stocks that will br assigned
     */
    @Override
    public void assignStocks(int[][] stocks) {
        return;
    }

    /**
     * returns the stock info of given product
     * @param model model number of the product
     * @param color color number of the product
     * @return stock info of given product
     */
    @Override
    public int stockInfo(int model, int color) {
        return models[model][color];
    }
}
class office_chair extends furniture{
    public office_chair(int[][] stocks) {
        super(stocks);
    }
    /**
     * assigns given stocks
     * @param stocks stocks that will br assigned
     */
    @Override
    public void assignStocks(int[][] stocks) {
        for(int i = 0; i < OFFICE_CHAIR_MODEL_NUM; i++){
            for(int j = 0; j < OFFICE_CHAIR_COLOR_NUM; j++){
                changeStock(i, j, stocks[i][j]);
            }
        }
    }
}
class office_desk extends furniture{
    public office_desk(int[][] stocks) {
        super(stocks);
    }
    /**
     * assigns given stocks
     * @param stocks stocks that will br assigned
     */
    @Override
    public void assignStocks(int[][] stocks) {
        for(int i = 0; i < OFFICE_DESK_MODEL_NUM; i++){
            for(int j = 0; j < OFFICE_DESK_COLOR_NUM; j++){
                changeStock(i, j, stocks[i][j]);
            }
        }
    }
}
class meeting_table extends furniture{
    public meeting_table(int[][] stocks) {
        super(stocks);
    }
    /**
     * assigns given stocks
     * @param stocks stocks that will br assigned
     */
    @Override
    public void assignStocks(int[][] stocks) {
        for(int i = 0; i < MEETING_TABLE_MODEL_NUM; i++){
            for(int j = 0; j < MEETING_TABLE_COLOR_NUM; j++){
                changeStock(i, j, stocks[i][j]);
            }
        }
    }
}
class book_case extends furniture{
    public book_case(int[][] stocks) {
        super(stocks);
    }
    /**
     * assigns given stocks
     * @param stocks stocks that will br assigned
     */
    @Override
    public void assignStocks(int[][] stocks) {
        for(int i = 0; i < BOOK_CASE_MODEL_NUM; i++){
            for(int j = 0; j < BOOK_CASE_COLOR_NUM; j++){
                changeStock(i, j, stocks[i][j]);
            }
        }
    }
}
class office_cabinet extends furniture{
    public office_cabinet(int[][] stocks) {
        super(stocks);
    }
    /**
     * assigns given stocks
     * @param stocks stocks that will br assigned
     */
    @Override
    public void assignStocks(int[][] stocks) {
        for(int i = 0; i < OFFICE_CABINET_MODEL_NUM; i++){
            for(int j = 0; j < OFFICE_CABINET_COLOR_NUM; j++){
                changeStock(i, j, stocks[i][j]);
            }
        }
    }
}
