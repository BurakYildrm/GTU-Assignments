import java.util.ArrayList;

/**
 * Class that represents a category tree.
 */
public class CategoryTree {
    private final Node root = new Node("all products");

    /**
     * Class that represents a tree node.
     */
    private static class Node{
        String category;
        ArrayList<Node> children;
        ArrayList<Product> products;

        /**
         * Constructs a node with given category as its category.
         * @param category category value
         */
        public Node(String category) {
            this.category = category;
            children = new ArrayList<>();
            products = new ArrayList<>();
        }

        /**
         * Returns a string representation of this node.
         * @return  a string representation of this node
         */
        @Override
        public String toString() {
            return "category: " + category;
        }
    }

    /**
     * Adds the given product to this tree.
     * @param product product to add
     */
    public void add(Product product){
        String[] categoryLevels = product.getCategory().split(" >> ");
        add(categoryLevels, 0, root, product);
    }

    /**
     * Adds the given product to this tree recursively.
     * @param categoryLevels array of subcategories
     * @param level index of current subcategory
     * @param currentRoot current node
     * @param product product to add
     */
    private void add(String[] categoryLevels, int level, Node currentRoot, Product product){
        if(level >= categoryLevels.length){
            currentRoot.products.add(product);
            return;
        }
        currentRoot.products.add(product);
        int index;
        int check = 0;
        for(index = 0; index < currentRoot.children.size(); index++){
            if(currentRoot.children.get(index).category.equals(categoryLevels[level])){
                check++;
                break;
            }
        }
        if(check == 0){
            currentRoot.children.add(new Node(categoryLevels[level]));
            add(categoryLevels, level + 1, currentRoot.children.get(currentRoot.children.size() - 1), product);
        }
        else{
            add(categoryLevels, level + 1, currentRoot.children.get(index), product);
        }
    }

    /**
     * Returns a list of products at the given category.
     * @param category category to get
     * @return a list of products at the given category.
     */
    public ArrayList<Product> get(String category){
        return get(category.split(" >> "), 0, root);
    }

    /**
     * Returns a list of products at the given category recursively.
     * @param category array of subcategories
     * @param level index of current subcategory
     * @param currentRoot current node
     * @return a list of products at the given category.
     */
    private ArrayList<Product> get(String[] category, int level, Node currentRoot){
        if(currentRoot.category.equals(category[category.length - 1])){
            return currentRoot.products;
        }
        int index;
        for(index = 0; index < currentRoot.children.size(); index++){
            if(currentRoot.children.get(index).category.equals(category[level])){
                break;
            }
        }
        return get(category, level + 1, currentRoot.children.get(index));
    }
}
