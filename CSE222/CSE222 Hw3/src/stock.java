public class stock{
    private int model;
    private int[] colors;

    /**
     * constructs a stock with given values
     * @param model model index
     * @param colors stocks of colors
     */
    public stock(int model, int[] colors) {
        this.model = model;
        this.colors = colors;
    }

    /**
     * returns the model index
     * @return the model index
     */
    public int getModel() {
        return model;
    }

    /**
     * returns the size of colors
     * @return the size of colors
     */
    public int getColorNum(){
        return colors.length;
    }

    /**
     * returns the element at specified index
     * @param color element of index to return
     * @return he element at specified index
     */
    public int get(int color){
        return colors[color];
    }

    /**
     * sets the element at specified index with given stock number
     * @param color index of element to set
     * @param stock stock number
     */
    public void set(int color, int stock){
        colors[color] = stock;
    }
}