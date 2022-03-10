import java.io.Serializable;
import java.util.Iterator;
import java.util.NoSuchElementException;

/**
 * Class that represents a binary search tree with max heap as the data of its nodes.
 * @param <E> the type of elements in this class' nodes' max heaps
 */
public class BSTHeapTree<E extends Comparable<E>> implements Serializable {
    /**
     * Field that stores the elements of this tree.
     */
    private BinarySearchTree<MaxHeap<HeapNode<E>>> tree;
    /**
     * Field that determines the maximum number of elements in nodes' max heaps.
     */
    private static final int MAX_HEAP_NODE = 7;
    /**
     * Field that stores the number of occurrences of an element.
     */
    private int numOfOccurrences;
    /**
     * Field that stores the mode of the elements of this tree.
     */
    private HeapNode<E> mode;
    /**
     * Field that stores the number of elements in this tree.
     */
    private int size = 0;

    /**
     * Class that represents both the data and the number of occurrence of that data.
     * @param <E> the type of elements in this heap node
     */
    private static class HeapNode<E extends Comparable<E>> implements Comparable<HeapNode<E>>, Serializable{
        /**
         * Field that stores the data of this heap node.
         */
        E data;
        /**
         * Field that stores the number of occurrences of the data of this heap node.
         */
        int occurrence;

        /**
         * Constructs a heap node with given value as its data.
         * @param data value to be assigned to data
         */
        private HeapNode(E data) {
            this.data = data;
            occurrence = 1;
        }

        /**
         * Compares the data fields of the given heap node and this heap node.
         * @param o heap node to compare
         * @return the result of comparison between data fields of the given heap node and this heap node
         */
        public int compareTo(HeapNode<E> o) {
            return data.compareTo(o.data);
        }

        /**
         * Returns a string representation of this heap node.
         * @return a string representation of this heap node
         */
        public String toString() {
            return data + "," + occurrence;
        }
    }

    /**
     * Constructs an empty tree.
     */
    public BSTHeapTree() {
        tree = new BinarySearchTree<>();
        mode = new HeapNode<>(null);
        mode.occurrence = 0;
    }

    /**
     * Returns the number of elements in this tree.
     * @return the number of elements in this tree
     */
    public int size(){
        return size;
    }

    /**
     * Adds the given element to this tree.
     * @param item element to add
     * @return the number of occurrences of the given element after adding
     */
    public int add(E item){
        tree.setRoot(add(tree.getRoot(), item));
        return numOfOccurrences;
    }

    /**
     * Adds the given element to this tree recursively.
     * @param startNode start node
     * @param item element to add
     * @return the subtree that contains the given element
     */
    private BinarySearchTree.Node<MaxHeap<HeapNode<E>>> add(BinarySearchTree.Node<MaxHeap<HeapNode<E>>> startNode, E item){
        if(startNode == null){
            size++;
            HeapNode<E> newHeapNode = new HeapNode<>(item);
            startNode = new BinarySearchTree.Node<>(new MaxHeap<>(newHeapNode));
            numOfOccurrences = newHeapNode.occurrence;
            return startNode;
        }
        else if(startNode.data.size() < MAX_HEAP_NODE){
            try {
                HeapNode<E> node = startNode.data.find(new HeapNode<>(item));
                node.occurrence++;
                numOfOccurrences = node.occurrence;
                size++;
            }
            catch (NoSuchElementException e){
                size++;
                HeapNode<E> newHeapNode = new HeapNode<>(item);
                startNode.data.add(newHeapNode);
                numOfOccurrences = newHeapNode.occurrence;
            }
            return startNode;
        }
        else if(startNode.data.contains(new HeapNode<>(item))){
            size++;
            HeapNode<E> node = startNode.data.find(new HeapNode<>(item));
            node.occurrence++;
            numOfOccurrences = node.occurrence;
        }
        else if(item.compareTo(startNode.data.getData().data) < 0){
            startNode.left = add(startNode.left, item);
        }
        else{
            startNode.right = add(startNode.right, item);
        }
        return startNode;
    }

    /**
     * Removes the given element from this tree.
     * @param item element to remove
     * @throws NoSuchElementException if the given element is not present in this tree
     * @return number of occurrences of the given element in this tree after removal
     */
    public int remove(E item){
        tree.setRoot(remove(tree.getRoot(), item));
        return numOfOccurrences;
    }

    /**
     * Removes the given element from this tree recursively.
     * @param startNode start node
     * @param item element to remove
     * @throws NoSuchElementException if the given element is not present in this tree
     * @return the subtree that does not contain the given element anymore
     */
    private BinarySearchTree.Node<MaxHeap<HeapNode<E>>> remove(BinarySearchTree.Node<MaxHeap<HeapNode<E>>> startNode, E item){
        if(startNode == null){
            throw new NoSuchElementException();
        }
        else if(startNode.data.contains(new HeapNode<>(item))){
            size--;
            HeapNode<E> node = startNode.data.find(new HeapNode<>(item));
            node.occurrence--;
            numOfOccurrences = node.occurrence;
            if(startNode.data.size() == 1 && numOfOccurrences == 0){
                return tree.delete(startNode.data, startNode);
            }
            else if(numOfOccurrences == 0){
                startNode.data.remove(node);
                if(startNode.left != null){
                    if(startNode.left.right == null){
                        if(startNode.left.data.size() == 1){
                            HeapNode<E> tempData = startNode.left.data.getData();
                            startNode.left = startNode.left.left;
                            startNode.data.add(tempData);
                            return startNode;
                        }
                        startNode.data.add(startNode.left.data.remove());
                    }
                    else{
                        startNode.data.add(biggestValue(startNode.left));
                    }
                }
                else if(startNode.right != null){
                    if(startNode.right.left == null){
                        if(startNode.right.data.size() == 1){
                            HeapNode<E> tempData = startNode.right.data.getData();
                            startNode.right = startNode.right.right;
                            startNode.data.add(tempData);
                            return startNode;
                        }
                        else {
                            Iterator<HeapNode<E>> iterator = startNode.right.data.iterator();
                            for(int i = 0; i < startNode.right.data.size() - 1; i++){
                                iterator.next();
                            }
                            HeapNode<E> returnValue = iterator.next();
                            startNode.right.data.remove(returnValue);
                            startNode.data.add(returnValue);
                        }
                    }
                    else{
                        for (HeapNode<E> heapNode : startNode.data) {
                            startNode.right.data.add(heapNode);
                        }
                        startNode = startNode.right;
                    }
                }
            }
        }
        else if(item.compareTo(startNode.data.getData().data) < 0){
            startNode.left = remove(startNode.left, item);
        }
        else{
            startNode.right = remove(startNode.right, item);
        }
        return startNode;
    }

    /**
     * Returns the biggest element in the right subtree of the given node.
     * @param startNode node whose biggest element of its right subtree will be found
     * @return the biggest element in the right subtree of the given node
     */
    private HeapNode<E> biggestValue(BinarySearchTree.Node<MaxHeap<HeapNode<E>>> startNode){
        if(startNode.right.right == null){
            HeapNode<E> returnValue;
            if(startNode.right.data.size() == 1){
                returnValue = startNode.right.data.getData();
                startNode.right = startNode.right.left;
            }
            else{
                returnValue = startNode.right.data.remove();
            }
            return returnValue;
        }
        else {
            return biggestValue(startNode.right);
        }
    }

    /**
     * Returns the number of occurrences of the given element in this tree.
     * @param item element to find
     * @throws NoSuchElementException if the given element is not present in this tree
     * @return the number of occurrences of the given element in this tree
     */
    public int find(E item){
        return find(tree.getRoot(), item);
    }

    /**
     * Finds the given element in this tree recursively.
     * @param startNode start node to
     * @param item element to find
     * @throws NoSuchElementException if the given element is not present in this tree
     * @return the number of occurrences of the given element in this tree
     */
    private int find(BinarySearchTree.Node<MaxHeap<HeapNode<E>>> startNode, E item){
        if(startNode == null){
            throw new NoSuchElementException();
        }
        else if(startNode.data.contains(new HeapNode<>(item))){
            return startNode.data.find(new HeapNode<>(item)).occurrence;
        }
        else if(item.compareTo(startNode.data.getData().data) < 0){
            return find(startNode.left, item);
        }
        else{
            return find(startNode.right, item);
        }
    }

    /**
     * Finds the mode of the elements in this tree.
     * @return the mode of the elements in this tree.
     */
    public E find_mode(){
        find_mode(tree.getRoot());
        return mode.data;
    }

    /**
     * Finds the mode of the elements in this tree recursively.
     * @param startNode start node
     */
    private void find_mode(BinarySearchTree.Node<MaxHeap<HeapNode<E>>> startNode){
        if(startNode == null){
            return;
        }
        HeapNode<E> temp = mostOccurrence(startNode.data);
        if(temp.occurrence > mode.occurrence){
            mode = temp;
        }
        find_mode(startNode.left);
        find_mode(startNode.right);
    }

    /**
     * Finds the element with most occurrence in the given max heap.
     * @param heap max heap to find in
     * @return the element with most occurrence
     */
    private HeapNode<E> mostOccurrence(MaxHeap<HeapNode<E>> heap){
        HeapNode<E> data = heap.getData();
        for(HeapNode<E> temp : heap){
            if(temp.occurrence > data.occurrence){
                data = temp;
            }
        }
        return data;
    }

    /**
     * Returns a string representation of this tree.
     * @return a string representation of this tree
     */
    public String toString() {
        return tree.toString();
    }

    /**
     * Returns the element at the top of the root node's max heap.
     * @return the element at the top of the root node's max heap
     */
    public E getData(){
        return tree.getData().getData().data;
    }
}
