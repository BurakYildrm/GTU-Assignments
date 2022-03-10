public class furniture implements furnitureInterface {
    private KWArraylist<stock> models;

    /**
     * constructs a furniture object with given stocks
     * @param stocks initial stock info
     */
    public furniture(int[][] stocks) {
        models = new KWArraylist<>();
        for(int i = 0; i < stocks.length; i++){
            models.add(new stock(i, stocks[i]));
        }
    }

    public KWArraylist<stock> getModels(){
        return models;
    }

    /**
     * changes the given product's stock
     * @param model model number of the product
     * @param color color number of the product
     * @param stock stock that will be set
     * @return true if the stock of given product is changed
     */
    @Override
    public boolean changeStock(int model, int color, int stock) {
        for (stock target : models) {
            if (target.getModel() == model) {
                target.set(color, stock);
                return true;
            }
        }
        return false;
    }

    /**
     * returns the stock info of given product
     * @param model model number of the product
     * @param color color number of the product
     * @return -1 if the given product is not present
     */
    @Override
    public int stockInfo(int model, int color) {
        for(stock target : models){
            if (target.getModel() == model) {
                return target.get(color);
            }
        }
        return -1;
    }

    public String toString(){
        StringBuilder s = new StringBuilder();
        for(stock target : models){
            for(int i = 0; i < target.getColorNum(); i++){
                s.append("model number: ").append(target.getModel()).append("\tcolor number: ")
                        .append(i).append("\tstock: ").append(target.get(i)).append("\n");
            }
        }
        return s.toString();
    }
}
class office_chair extends furniture{
    public office_chair(int[][] stocks) {
        super(stocks);
    }
}
class office_desk extends furniture{
    public office_desk(int[][] stocks) {
        super(stocks);
    }
}
class meeting_table extends furniture{
    public meeting_table(int[][] stocks) {
        super(stocks);
    }
}
class book_case extends furniture{
    public book_case(int[][] stocks) {
        super(stocks);
    }
}
class office_cabinet extends furniture{
    public office_cabinet(int[][] stocks) {
        super(stocks);
    }
}