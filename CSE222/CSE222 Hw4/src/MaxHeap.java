import java.io.Serializable;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.NoSuchElementException;

/**
 * Class that represents a max heap.
 * @param <E> the type of elements in this max heap
 */
public class MaxHeap<E extends Comparable<E> > implements Serializable, Iterable<E>, Comparable<MaxHeap<E> > {
    /**
     * Field that stores the element of this max heap.
     */
    private ArrayList<E> theData;

    /**
     * Constructs an empty max heap.
     */
    public MaxHeap() {
        theData = new ArrayList<>();
    }

    /**
     * Constructs a max heap with given element at the top of this max heap.
     * @param item element to place at top
     */
    public MaxHeap(E item){
        theData = new ArrayList<>();
        theData.add(item);
    }

    /**
     * Returns the size of this max heap.
     * @return size of this max heap
     */
    protected int size() {
        return theData.size();
    }

    /**
     * Checks if this max heap contains the given element.
     * @param item element to check
     * @return true if this max heap contains the given element
     */
    public boolean contains(E item){
        for(E i : theData){
            if(i.compareTo(item) == 0){
                return true;
            }
        }
        return false;
    }

    /**
     * Adds the given element to this max heap.
     * @param item element to add
     */
    public void add(E item){
        theData.add(item);
        int child = theData.size() - 1;
        int parent =  (child - 1)/2;
        E temp;

        while (theData.get(child).compareTo(theData.get(parent)) > 0 && parent >= 0){
            temp = theData.get(parent);
            theData.set(parent, theData.get(child));
            theData.set(child, temp);
            child = parent;
            parent = (child - 1)/2;
        }
    }

    /**
     * Finds the given element in this max heap.
     * @param item element to find
     * @return the given element
     * @throws NoSuchElementException if the given element is not present
     */
    public E find(E item){
        for(E element : theData){
            if(item.compareTo(element) == 0){
                return element;
            }
        }
        throw new NoSuchElementException();
    }

    /**
     * Returns the element at the top of this max heap.
     * @return the element at the top of this max heap
     */
    public E getData(){
        return theData.get(0);
    }

    /**
     * Sorts the given arraylist in increasing order.
     * @param arrayToSort array to sort
     */
    private void sort(ArrayList<E> arrayToSort){
        int size = arrayToSort.size() - 1;
        E temp;
        while (size >= 0){
            temp = arrayToSort.get(0);
            arrayToSort.set(0, arrayToSort.get(size));
            arrayToSort.set(size, temp);
            heapify(arrayToSort, size, 0);
            size--;
        }
    }

    /**
     * Removes the element at the top of this max heap.
     * @return the element previously at the top of this max heap
     */
    public E remove(){
        if(theData.size() == 0){
            return null;
        }

        E removedValue = theData.get(0);
        E lastValue = theData.remove(theData.size() - 1);

        if(!theData.isEmpty()) {
            theData.set(0, lastValue);
        }

        heapify(theData, theData.size(), 0);
        return removedValue;
    }

    /**
     * Rearranges the given arraylist according to the max heap properties.
     * @param arrayToHeapify arraylist to rearrange
     * @param size given size of the arraylist
     * @param startIndex starting index to rearrange
     */
    private void heapify(ArrayList<E> arrayToHeapify, int size, int startIndex){
        int parent = startIndex;
        int leftChild = startIndex*2 + 1;
        int rightChild = startIndex*2 + 2;
        int check1;
        int check2;
        E temp;

        while (true){
            check1 = 0;
            check2 = 0;
            if(leftChild < size && arrayToHeapify.get(leftChild).compareTo(arrayToHeapify.get(parent)) > 0){
                check1++;
            }
            if(rightChild < size && arrayToHeapify.get(rightChild).compareTo(arrayToHeapify.get(parent)) > 0){
                check2++;
            }
            if(check1 == 0 && check2 == 0){
                break;
            }
            else{
                if(check1 == 1 && check2 == 1){
                    if(arrayToHeapify.get(leftChild).compareTo(arrayToHeapify.get(rightChild)) > 0){
                        temp = arrayToHeapify.get(leftChild);
                        arrayToHeapify.set(leftChild, arrayToHeapify.get(parent));
                        arrayToHeapify.set(parent, temp);
                        parent = leftChild;
                    }
                    else{
                        temp = arrayToHeapify.get(rightChild);
                        arrayToHeapify.set(rightChild, arrayToHeapify.get(parent));
                        arrayToHeapify.set(parent, temp);
                        parent = rightChild;
                    }
                }
                else if(check1 == 1){
                    temp = arrayToHeapify.get(leftChild);
                    arrayToHeapify.set(leftChild, arrayToHeapify.get(parent));
                    arrayToHeapify.set(parent, temp);
                    parent = leftChild;
                }
                else{
                    temp = arrayToHeapify.get(rightChild);
                    arrayToHeapify.set(rightChild, arrayToHeapify.get(parent));
                    arrayToHeapify.set(parent, temp);
                    parent = rightChild;
                }
                leftChild = parent*2 + 1;
                rightChild = parent*2 + 2;
            }
        }
    }

    /**
     * Checks if this max heap is empty.
     * @return true if this max heap is empty
     */
    public boolean isEmpty(){
        return theData.size() == 0;
    }

    /**
     * Removes the given element from this max heap.
     * @param item element to remove
     */
    protected void remove(E item){
        theData.remove(item);
        for(int i = theData.size()/2 - 1; i >= 0; i--){
            heapify(theData, theData.size(), i);
        }
    }

    /**
     * Returns a string representation of this max heap. The string representation consists of a list of the max heap's elements in decreasing order.
     * Adjacent elements are separated by the character " " (space). Elements are converted to strings as by their toString() methods.
     * @return a string representation of this max heap
     */
    public String toString() {
        ArrayList<E> sorted = new ArrayList<>(theData);
        sort(sorted);
        StringBuilder s = new StringBuilder();
        while (!sorted.isEmpty()){
            s.append(sorted.remove(sorted.size() - 1)).append(" ");
        }
        return s.toString();
    }

    /**
     * Returns a tree representation of this max heap. Child elements are represent with a new line and additional whitespace.
     * @return a tree representation of this max heap
     */
    public String treeStructure(){
        return treeStructure(0, new StringBuilder(), 1).toString();
    }

    /**
     * Returns a tree representation of this max heap recursively. Child elements are represent with a new line and additional whitespace.
     * @param index index to start
     * @param s StringBuilder to append the elements of this max heap
     * @param level current level of the max heap
     * @return a tree representation of this max heap
     */
    private StringBuilder treeStructure(int index, StringBuilder s, int level){
        for(int i = 1; i < level; i++){
            s.append(" ");
        }
        if(index >= theData.size()){
            return s.append("null\n");
        }
        s.append(theData.get(index)).append("\n");
        treeStructure(2*index + 1, s, level + 1);
        treeStructure(2*index + 2, s, level + 1);
        return s;
    }

    /**
     * Returns an iterator over the elements in this max heap in decreasing order.
     * @return an iterator over the elements in this max heap in decreasing order
     */
    public Iterator<E> iterator() {
        return new MaxHeapIter();
    }

    /**
     * Compares the top of this MaxHeap with the top of the given max heap.
     * @param o max heap to compare
     * @return result of the comparison between tops of these max heaps
     */
    public int compareTo(MaxHeap<E> o) {
        return theData.get(0).compareTo(o.theData.get(0));
    }

    /**
     * Class to represent a max heap iterator.
     */
    private class MaxHeapIter implements Iterator<E> {
        /**
         * Field that stores the counter.
         */
        private int count;
        /**
         * Field that stores the last element returned by next().
         */
        private E lastItemReturned;
        /**
         * Field that stores the elements of this max heap in decreasing order.
         */
        private ArrayList<E> sortedArray;

        /**
         * Constructs a max heap iterator.
         */
        public MaxHeapIter() {
            sortedArray = new ArrayList<>(theData);
            sort(sortedArray);
            count = sortedArray.size() - 1;
            lastItemReturned = null;
        }

        /**
         * Returns true if the iteration has more elements.
         * @return true if the iteration has more elements
         */
        public boolean hasNext() {
            return count >= 0;
        }

        /**
         * Returns the next element in the iteration.
         * @return the next element in the iteration
         */
        public E next() {
            lastItemReturned =  sortedArray.get(count--);
            return lastItemReturned;
        }
    }
}
