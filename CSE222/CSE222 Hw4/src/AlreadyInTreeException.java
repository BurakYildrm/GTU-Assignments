/**
 * Thrown by BinarySearchTree's add(E item) method to indicate that the element to be added is already in the tree.
 */
public class AlreadyInTreeException extends Exception{
    /**
     * Constructs a AlreadyInTreeException with null as its error message string.
     */
    public AlreadyInTreeException() {
        super();
    }
}
