/**
 * Class to check tree type.
 */
public class TypeChecker {
    /**
     * Field that stores the number of black nodes.
     */
    private static int numOfBlackNodes = 0;

    /**
     * Checks the type of the given binary search tree.
     * @param tree tree to check type
     * @param <E> type of elements in the tree
     * @return 1 if both avl and red-black, 2 if red-black, 3 if avl, 4 if neither avl nor red-black
     */
    public static <E extends Comparable<E>> int checkType(BinarySearchTree<E> tree){
        numOfBlackNodes = numOfBlackNodes(tree.root);
        boolean isRedBlackTree;
        boolean isAVL;

        try {
            checkRedBlackTree(tree.root, 0, false);
            isRedBlackTree = true;
        } catch (IllegalStateException e) {
            isRedBlackTree = false;
        }

        try {
            checkAVLTree(tree.root);
            isAVL = true;
        } catch (IllegalStateException e) {
            isAVL = false;
        }

        if (isRedBlackTree && isAVL) return 0;
        else if (isRedBlackTree) return 1;
        else if (isAVL) return 2;
        else return 3;
    }

    /**
     * Checks if the given tree is a red-black tree.
     * @param root root of the tree
     * @param currentHeight current number of black nodes
     * @param isParentRed determines if the parent is red
     * @param <E> type of elements in the tree
     */
    private static <E extends Comparable<E>> void checkRedBlackTree(BinaryTree.Node<E> root, int currentHeight, boolean isParentRed){
        if(root == null){
            if(currentHeight != numOfBlackNodes){
                throw new IllegalStateException();
            }
            return;
        }
        if(root.isRed && isParentRed) throw new IllegalStateException();
        if(!root.isRed) currentHeight++;
        int temp = currentHeight;
        checkRedBlackTree(root.left, currentHeight, root.isRed);
        currentHeight = temp;
        checkRedBlackTree(root.right, currentHeight, root.isRed);
    }

    /**
     * Calculates the number of black nodes on one path.
     * @param root root of the tree
     * @param <E> type of elements in the tree
     * @return the number of black nodes on one path
     */
    private static <E extends Comparable<E>> int numOfBlackNodes(BinaryTree.Node<E> root){
        if(root == null){
            return 0;
        }
        return !root.isRed ? numOfBlackNodes(root.left) + 1 : numOfBlackNodes(root.left);
    }

    /**
     * Checks if the given tree is an avl tree.
     * @param root root of the tree
     * @param <E> type of elements in the tree
     */
    private static <E extends Comparable<E>> void checkAVLTree(BinaryTree.Node<E> root){
        if(root != null){
            int heightLeft = height(root.left, 0, 0);
            int heightRight = height(root.right, 0, 0);
            if(Math.abs(heightRight - heightLeft) > 1) throw new IllegalStateException();
            checkAVLTree(root.left);
            checkAVLTree(root.right);
        }
    }

    /**
     * Calculates the height of the given node.
     * @param root node to calculate height
     * @param tempHeight temporary height
     * @param height real height
     * @param <E> type of elements in the tree
     * @return the height of the given node
     */
    private static <E extends Comparable<E>> int height(BinaryTree.Node<E> root, int tempHeight, int height){
        if(root == null){
            return Math.max(tempHeight, height);
        }
        int temp = ++tempHeight;
        height = height(root.left, tempHeight, height);
        tempHeight = temp;
        height = height(root.right, tempHeight, height);
        return height;
    }
}
