import java.io.Serializable;
import java.util.Scanner;

/**
 * Class that represents a binary search tree.
 * @param <E> the type of elements in min heap
 */
public class BinarySearchTree<E extends Comparable<E> > implements Serializable {
    /**
     * Field that stores root node.
     */
    private Node<E> root;
    /**
     * Field that stores element removed by delete method.
     */
    private E deletedItem;

    /**
     * Class that represent a node.
     * @param <E> the type of elements in min heap
     */
    protected static class Node<E extends Comparable<E>> implements Serializable{
        /**
         * Field that stores the data of this node.
         */
        protected E data;
        /**
         * Field that stores the left child of this node.
         */
        protected Node<E> left;
        /**
         * Field that stores the right child of this node.
         */
        protected Node<E> right;

        /**
         * Constructs a node with given value as its data.
         * @param data value to be assigned to data
         */
        protected Node(E data) {
            this.data = data;
            left = right = null;
        }
    }

    /**
     * Constructs an empty binary search tree.
     */
    public BinarySearchTree() {
        root = null;
    }

    /**
     * Constructs a binary search tree with given node as its root.
     * @param root node to be assigned to root
     */
    private BinarySearchTree(Node<E> root){
        this.root = root;
    }

    /**
     * Constructs a binary search tree with given value as its root's data and given subtrees as root's left and right children.
     * @param data value to be assigned to root's data
     * @param leftTree subtree to be assigned to root's left child
     * @param rightTree subtree to be assigned to root's right child
     */
    public BinarySearchTree(E data, BinarySearchTree<E> leftTree, BinarySearchTree<E> rightTree){
        this.root = new Node<>(data);

        if(leftTree != null){
            root.left = leftTree.root;
        }
        else {
            root.left = null;
        }

        if(rightTree != null){
            root.right = rightTree.root;
        }
        else {
            root.right = null;
        }
    }

    /**
     * Adds the given element to this binary search tree.
     * @param item element to add
     * @return true if the given element is not already present in this binary search tree
     */
    public boolean add(E item){
        try {
            root = add(item, root);
            return true;
        } catch (AlreadyInTreeException e) {
            return false;
        }
    }

    /**
     * Adds the given element to this binary search tree recursively.
     * @param item element to add
     * @param ancestor start node
     * @return the subtree that contains the given element
     * @throws AlreadyInTreeException if the given element is already present in this binary search tree
     */
    private Node<E> add(E item, Node<E> ancestor) throws AlreadyInTreeException {
        if(ancestor == null){
            ancestor = new Node<>(item);
            return ancestor;
        }
        else if(item.compareTo(ancestor.data) == 0){
            throw new AlreadyInTreeException();
        }
        else if(item.compareTo(ancestor.data) < 0){
            ancestor.left = add(item, ancestor.left);
        }
        else{
            ancestor.right = add(item, ancestor.right);
        }
        return ancestor;
    }

    /**
     * Checks if the given is present in this binary search tree.
     * @param item element to check
     * @return true if the given element is present in this binary search tree
     */
    public boolean contains(E item){
        return contains(item, root);
    }

    /**
     * Checks if the given is present in this binary search tree recursively.
     * @param item element to check
     * @param ancestor start node
     * @return true if the given element is present in this binary search tree
     */
    private boolean contains(E item, Node<E> ancestor){
        if(ancestor == null){
            return false;
        }
        else if(item.compareTo(ancestor.data) == 0){
            return true;
        }
        else if(item.compareTo(ancestor.data) < 0){
            return contains(item, ancestor.left);
        }
        else{
            return contains(item, ancestor.right);
        }
    }

    /**
     * Finds the given element in this binary search tree.
     * @param item element to find
     * @return given element in this binary search tree
     */
    public E find(E item){
        return find(item, root);
    }

    /**
     * Finds the given element in this binary search tree recursively.
     * @param item element to find
     * @param ancestor start node
     * @return the given element in this binary search tree
     */
    private E find(E item, Node<E> ancestor){
        if(ancestor == null){
            return null;
        }
        else if(item.compareTo(ancestor.data) == 0){
            return ancestor.data;
        }
        else if(item.compareTo(ancestor.data) < 0){
            return find(item, ancestor.left);
        }
        else{
            return find(item, ancestor.right);
        }
    }

    /**
     * Returns the data of the root node of this binary search tree.
     * @return the data of the root node of this binary search tree
     */
    public E getData(){
        if(root == null){
            return null;
        }
        return root.data;
    }

    /**
     * Returns the left child of the root as a binary search tree.
     * @return the left child of the root as a binary search tree
     */
    public BinarySearchTree<E> getLeftSubtree(){
        if(root != null && root.left != null) {
            return new BinarySearchTree<>(root.left);
        }
        else{
            return null;
        }
    }

    /**
     * Returns the right child of the root as a binary search tree.
     * @return the right child of the root as a binary search tree
     */
    public BinarySearchTree<E> getRightSubtree(){
        if (root != null && root.right != null) {
            return new BinarySearchTree<>(root.right);
        }
        else {
            return null;
        }
    }

    /**
     * Checks if the root of this binary search tree is a leaf node.
     * @return true if the root of this binary search tree is a leaf node
     */
    public boolean isLeaf(){
        if(root == null){
            return false;
        }
        else{
            return root.left == null && root.right == null;
        }
    }

    /**
     * Removes the given element from this binary search tree.
     * @param item element to remove
     * @return true if this binary search tree contained the given element
     */
    public boolean remove(E item){
        E result = delete(item);
        return result != null;
    }

    /**
     * Deletes the given element from this binary search tree.
     * @param item element to delete
     * @return deleted element
     */
    public E delete(E item){
        root = delete(item, root);
        return deletedItem;
    }

    /**
     * Deletes the given element from this binary search tree recursively.
     * @param item element to delete
     * @param ancestor start node
     * @return node whose children do not contain the given element anymore
     */
    protected Node<E> delete(E item, Node<E> ancestor){
        if(ancestor == null){
            deletedItem = null;
            return null;
        }
        else if(item.compareTo(ancestor.data) < 0){
            ancestor.left = delete(item, ancestor.left);
        }
        else if(item.compareTo(ancestor.data) > 0){
            ancestor.right = delete(item, ancestor.right);
        }
        else{
            deletedItem = ancestor.data;
            if(ancestor.left != null && ancestor.right != null){
                if (ancestor.left.right == null) {
                    ancestor.data = ancestor.left.data;
                    ancestor.left = ancestor.left.left;
                }
                else {
                    ancestor.data = biggestValue(ancestor.left);
                }
            }
            else if(ancestor.left != null){
                return ancestor.left;
            }
            else if(ancestor.right != null){
                return ancestor.right;
            }
            else{
                return null;
            }
        }
        return ancestor;
    }

    /**
     * Returns the biggest element in the right subtree of the given node.
     * @param startNode node whose biggest element of its right subtree will be found
     * @return the biggest element in the right subtree of the given node
     */
    private E biggestValue(Node<E> startNode){
        if(startNode.right.right == null){
            E returnVal = startNode.right.data;
            startNode.right = startNode.right.left;
            return returnVal;
        }
        return biggestValue(startNode.right);
    }

    /**
     * Returns a tree representation of this binary search tree. Child elements are represent with a new line and additional whitespace.
     * @return a tree representation of this binary search tree
     */
    @Override
    public String toString() {
        StringBuilder s = new StringBuilder();
        preOrderTraversal(root, s, 1);
        return s.toString();
    }

    /**
     * Returns a tree representation of this binary search tree recursively. Child elements are represent with a new line and additional whitespace.
     * @return a tree representation of this binary search tree
     */
    private void preOrderTraversal(Node<E> ancestor, StringBuilder s, int level) {
        for(int i = 1; i < level; i++){
            s.append(" ");
        }
        if (ancestor == null) {
            s.append("null\n");
        }
        else {
            s.append(ancestor.data).append("\n");
            preOrderTraversal(ancestor.left, s, level + 1);
            preOrderTraversal(ancestor.right, s, level + 1);
        }
    }

    /**
     * Returns a string representation of this binary search tree. The string representation consists of a list of this binary search tree's elements in increasing order.
     * Adjacent elements are separated by the characters " " (space). Elements are converted to strings as by their toString() methods.
     * @return a string representation of this binary search tree
     */
    public String inOrderTraversal(){
        return inOrderTraversal(root, new StringBuilder()).toString();
    }

    /**
     * Returns a string representation of this binary search tree recursively. The string representation consists of a list of this binary search tree's elements in increasing order.
     * Adjacent elements are separated by the character " " (space). Elements are converted to strings as by their toString() methods.
     * @param ancestor start node
     * @param s StringBuilder to append elements of this binary search tree
     * @return a string representation of this binary search tree
     */
    private StringBuilder inOrderTraversal(Node<E> ancestor, StringBuilder s){
        if(ancestor == null){
            return s.append("");
        }
        inOrderTraversal(ancestor.left, s);
        s.append(ancestor.data).append(" ");
        inOrderTraversal(ancestor.right, s);
        return s;
    }

    /**
     * Returns the root node of this binary search tree.
     * @return the root node of this binary search tree
     */
    protected Node<E> getRoot(){
        return root;
    }

    /**
     * Sets the root node of this binary search tree to the given node.
     * @param newRoot node to be assigned to root of this binary search tree
     */
    protected void setRoot(Node<E> newRoot){
        root = newRoot;
    }

    /**
     * Constructs a binary search tree from given scanner.
     * @param scanner scanner to construct binary search tree from
     * @return a binary search tree constructed from given scanner
     */
    public static BinarySearchTree<String> readBinaryTree(Scanner scanner){
        String data = scanner.next();
        if(data.equals("null")){
            return null;
        }
        BinarySearchTree<String> leftTree = readBinaryTree(scanner);
        BinarySearchTree<String> rightTree = readBinaryTree(scanner);
        return new BinarySearchTree<String >(data, leftTree, rightTree);
    }
}