import java.util.*;

/**
 * Class that represents a navigable set using AVL Tree.
 * @param <E> type of elements in this set
 */
@SuppressWarnings("unchecked")
public class AVLTreeSet<E extends Comparable<E>> implements NavigableSet<E>, Iterable<E>{
    private AVLTree<E> theData = new AVLTree<>();

    public E lower(E e) {
        return null;
    }

    public E floor(E e) {
        return null;
    }

    public E ceiling(E e) {
        return null;
    }

    public E higher(E e) {
        return null;
    }

    public E pollFirst() {
        return null;
    }

    public E pollLast() {
        return null;
    }

    public int size() {
        return 0;
    }

    public boolean isEmpty() {
        return false;
    }

    public boolean contains(Object o) {
        return false;
    }

    /**
     * Returns an iterator over the elements in this set.
     * @return an iterator over the elements in this set
     */
    public Iterator<E> iterator() {
        return theData.inOrderTraversal().iterator();
    }

    public Object[] toArray() {
        return new Object[0];
    }

    public <T> T[] toArray(T[] a) {
        return null;
    }

    /**
     * Adds the given element to this set.
     * @param e element to add
     * @return true if the given element is not present in this set
     */
    public boolean add(E e) {
        return theData.add(e);
    }

    /**
     * Removes the given element from this set.
     * @param o element to remove
     * @return true if the given element is present in this set
     */
    public boolean remove(Object o) {
        return theData.remove((E) o);
    }

    public boolean containsAll(Collection<?> c) {
        return false;
    }

    public boolean addAll(Collection<? extends E> c) {
        return false;
    }

    public boolean retainAll(Collection<?> c) {
        return false;
    }

    public boolean removeAll(Collection<?> c) {
        return false;
    }

    /**
     * Removes all of the elements from this set.
     */
    public void clear() {
        theData = new AVLTree<>();
    }

    public NavigableSet<E> descendingSet() {
        return null;
    }

    /**
     * Returns an iterator over the elements of this set, in descending order.
     * @return an iterator over the elements of this set, in descending order
     */
    public Iterator<E> descendingIterator() {
        return new DescendingIterator();
    }

    public NavigableSet<E> subSet(E fromElement, boolean fromInclusive, E toElement, boolean toInclusive) {
        return null;
    }

    /**
     * Returns a set consists of elements less than (or equal to if, inclusive is true) toElement.
     * @param toElement upper limit
     * @param inclusive determines if the upper limit is added
     * @return a set consists of elements less than (or equal to if, inclusive is true) toElement.
     */
    public NavigableSet<E> headSet(E toElement, boolean inclusive) {
        ArrayList<E> sorted = theData.inOrderTraversal();
        AVLTreeSet<E> headSet = new AVLTreeSet<>();
        for(E element : sorted){
            if(inclusive ? element.compareTo(toElement) <= 0 : element.compareTo(toElement) < 0){
                headSet.add(element);
            }
            else{
                break;
            }
        }
        return headSet;
    }

    /**
     * Returns a set consists of elements greater than (or equal to, if the inclusive is true) fromElement.
     * @param fromElement lower limit
     * @param inclusive determines if the lower limit is added
     * @return a set consists of elements greater than (or equal to, if the inclusive is true) fromElement
     */
    public NavigableSet<E> tailSet(E fromElement, boolean inclusive) {
        ArrayList<E> sorted = theData.inOrderTraversal();
        AVLTreeSet<E> tailSet = new AVLTreeSet<>();
        for(int i = sorted.size() - 1; i >= 0; i--){
            if(inclusive ? sorted.get(i).compareTo(fromElement) >= 0 : sorted.get(i).compareTo(fromElement) > 0){
                tailSet.add(sorted.get(i));
            }
            else{
                break;
            }
        }
        return tailSet;
    }

    public Comparator<? super E> comparator() {
        return null;
    }

    public SortedSet<E> subSet(E fromElement, E toElement) {
        return null;
    }

    /**
     * Returns a set consists of elements less than toElement.
     * @param toElement upper limit
     * @return a set consists of elements less than toElement
     */
    public SortedSet<E> headSet(E toElement) {
        ArrayList<E> sorted = theData.inOrderTraversal();
        AVLTreeSet<E> headSet = new AVLTreeSet<>();
        for(E element : sorted){
            if(element.compareTo(toElement) < 0){
                headSet.add(element);
            }
            else{
                break;
            }
        }
        return headSet;
    }

    /**
     * Returns a set consists of elements greater than or equal to fromElement.
     * @param fromElement lower limit
     * @return a set consists of elements greater than or equal to fromElement
     */
    public SortedSet<E> tailSet(E fromElement) {
        ArrayList<E> sorted = theData.inOrderTraversal();
        AVLTreeSet<E> tailSet = new AVLTreeSet<>();
        for(int i = sorted.size() - 1; i >= 0; i--){
            if(sorted.get(i).compareTo(fromElement) >= 0){
                tailSet.add(sorted.get(i));
            }
            else{
                break;
            }
        }
        return tailSet;
    }

    public E first() {
        return null;
    }

    public E last() {
        return null;
    }

    /**
     * Class that represents a iterator with descending order.
     */
    private class DescendingIterator implements Iterator<E>{
        private ArrayList<E> sorted;
        private int counter;

        /**
         * Constructs a DescendingIterator.
         */
        public DescendingIterator() {
            sorted = theData.inOrderTraversal();
            counter = sorted.size() - 1;
        }

        /**
         * Returns true if the iteration has more elements.
         * @return true if the iteration has more elements
         */
        public boolean hasNext() {
            return counter >= 0;
        }

        /**
         * Returns the next element in the iteration.
         * @return the next element in the iteration
         */
        public E next() {
            if(!hasNext()){
                throw new NoSuchElementException();
            }
            return sorted.get(counter--);
        }
    }

    /**
     * Returns a string representation of this set.
     * @return a string representation of this set
     */
    public String toString() {
        return theData.inOrderTraversal().toString();
    }
}
