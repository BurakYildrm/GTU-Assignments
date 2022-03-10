import java.io.Serializable;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.ListIterator;
import java.util.NoSuchElementException;

/**
 * Class that represents a min heap.
 * @param <E> the type of elements in this min heap
 */
public class Heap<E extends Comparable<E> > implements Serializable, Iterable<E> {
    /**
     * Field that stores the elements of this min heap.
     */
    private ArrayList<E> theData;

    /**
     * Constructs an empty min heap.
     */
    public Heap() {
        theData = new ArrayList<>();
    }

    /**
     * Checks if this min heap contains the given element.
     * @param item element to check
     * @return true if this min heap contains the given element
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
     * Adds the given element to this min heap.
     * @param item element to add
     */
    public void add(E item){
        theData.add(item);
        int child = theData.size() - 1;
        int parent =  (child - 1)/2;
        E temp;

        while (theData.get(parent).compareTo(theData.get(child)) > 0 && parent >= 0){
            temp = theData.get(parent);
            theData.set(parent, theData.get(child));
            theData.set(child, temp);
            child = parent;
            parent = (child - 1)/2;
        }
    }

    /**
     * Finds the given element in this min heap.
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
     * Merges the given min heap with this min heap according to the min heap properties.
     * @param other min heap to merge with this min heap
     */
    public void merge(Heap<E> other){
        while (!other.isEmpty()){
            add(other.remove());
        }
    }

    /**
     * Removes the iᵗʰ largest element from this min heap.
     * @param index index that specifies the iᵗʰ largest value in this min heap to remove
     * @return the iᵗʰ largest element
     */
    public E removeSpecifiedLargestElement(int index){
        ArrayList<E> sortedArray = new ArrayList<>(theData);
        sort(sortedArray);
        E returnValue = sortedArray.remove(index - 1);
        theData.clear();
        while (!sortedArray.isEmpty()) {
            add(sortedArray.remove(sortedArray.size() - 1));
        }
        return returnValue;
    }

    /**
     * Sorts the given arraylist in decreasing order.
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
     * Removes the element at the top of this min heap.
     * @return the element previously at the top of this min heap
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
     * Rearranges the given arraylist according to the min heap properties.
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
            if(leftChild < size && arrayToHeapify.get(parent).compareTo(arrayToHeapify.get(leftChild)) > 0){
                check1++;
            }
            if(rightChild < size && arrayToHeapify.get(parent).compareTo(arrayToHeapify.get(rightChild)) > 0){
                check2++;
            }
            if(check1 == 0 && check2 == 0){
                break;
            }
            else{
                if(check1 == 1 && check2 == 1){
                    if(arrayToHeapify.get(leftChild).compareTo(arrayToHeapify.get(rightChild)) < 0){
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
     * Checks if this min heap is empty.
     * @return true if this min heap is empty
     */
    public boolean isEmpty(){
        return theData.size() == 0;
    }

    /**
     * Returns a string representation of this min heap. The string representation consists of a list of the min heap's elements in increasing order.
     * Adjacent elements are separated by the characters " " (space). Elements are converted to strings as by their toString() methods.
     * @return a string representation of this min heap.
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
     * Returns a tree representation of this min heap. Child elements are represent with a new line and additional whitespace.
     * @return a tree representation of this min heap.
     */
    public String treeStructure(){
        return treeStructure(0, new StringBuilder(), 1).toString();
    }

    /**
     * Returns a tree representation of this min heap recursively. Child elements are represent with a new line and additional whitespace.
     * @param index index to start
     * @param s StringBuilder to append the elements of this min heap
     * @param level current level of the min heap
     * @return a tree representation of this min heap
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
     * Returns a list iterator over the elements of this min heap in increasing order.
     * @return a list iterator over the elements of this min heap in increasing order
     */
    public ListIterator<E> heapIterator(){
        return new HeapIter();
    }

    /**
     * Returns an iterator over the elements of this min heap in increasing order.
     * @return an iterator over the elements of this min heap in increasing order
     */
    public Iterator<E> iterator() {
        return new HeapIter();
    }

    /**
     * Class that represents a heap iterator.
     */
    private class HeapIter implements ListIterator<E> {
        /**
         * Field that stores the counter.
         */
        private int count;
        /**
         * Field that stores the last element returned by next().
         */
        private E lastItemReturned;
        /**
         * Field that stores the elements of this min heap in increasing order.
         */
        private ArrayList<E> sortedArray;

        /**
         * Constructs a heap iterator.
         */
        public HeapIter() {
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

        /**
         * Replaces the last element returned by next() with the given element.
         * @param e element to replace with the last element returned by next()
         */
        public void set(E e){
            if(lastItemReturned == null){
                throw new IllegalStateException();
            }
            int index = findIndex(lastItemReturned);
            theData.set(index, e);
            for(int i = theData.size()/2 - 1; i >= 0; i--){
                heapify(theData, theData.size(), i);
            }
        }

        /**
         * Finds the index of the given element in theData field.
         * @param item element to find the index
         * @return index of the given element
         */
        private int findIndex(E item){
            int index;
            for(index = 0; index < theData.size() && theData.get(index).compareTo(item) != 0; index++);
            return index;
        }

        /**
         * This operation is not supported in this min heap.
         * @throws UnsupportedOperationException
         */
        public boolean hasPrevious() {
            throw new UnsupportedOperationException();
        }

        /**
         * This operation is not supported in this min heap.
         * @throws UnsupportedOperationException
         */
        public E previous() {
            throw new UnsupportedOperationException();
        }

        /**
         * This operation is not supported in this min heap.
         * @throws UnsupportedOperationException
         */
        public int nextIndex() {
            throw new UnsupportedOperationException();
        }

        /**
         * This operation is not supported in this min heap.
         * @throws UnsupportedOperationException
         */
        public int previousIndex() {
            throw new UnsupportedOperationException();
        }

        /**
         * This operation is not supported in this min heap.
         * @throws UnsupportedOperationException
         */
        public void remove() {
            throw new UnsupportedOperationException();
        }

        /**
         * This operation is not supported in this min heap.
         * @throws UnsupportedOperationException
         */
        public void add(E e) {
            throw new UnsupportedOperationException();
        }
    }
}
