import java.util.*;

/**
 * Class that represents a navigable set using SkipList.
 * @param <E> type of elements in this set
 */
@SuppressWarnings("unchecked")
public class SkipListSet<E extends Comparable<E>> implements NavigableSet<E>, Iterable<E> {
    private SkipList<E> theData = new SkipList<>();

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
        return theData.iterator();
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
        if(theData.contains(e)) return false;
        theData.add(e);
        return true;
    }

    /**
     * Removes the given element from this tree.
     * @param o element to remove
     * @return true if the given element is present in this set
     */
    public boolean remove(Object o) {
        return theData.remove((E) o) != null;
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
        theData = new SkipList<>();
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

    public NavigableSet<E> headSet(E toElement, boolean inclusive) {
        return null;
    }

    public NavigableSet<E> tailSet(E fromElement, boolean inclusive) {
        return null;
    }

    public Comparator<? super E> comparator() {
        return null;
    }

    public SortedSet<E> subSet(E fromElement, E toElement) {
        return null;
    }

    public SortedSet<E> headSet(E toElement) {
        return null;
    }

    public SortedSet<E> tailSet(E fromElement) {
        return null;
    }

    public E first() {
        return null;
    }

    public E last() {
        return null;
    }

    /**
     * Returns a string representation of this set.
     * @return a string representation of this set
     */
    public String toString() {
        return theData.toString();
    }

    /**
     * Class that represents a iterator with descending order.
     */
    private class DescendingIterator implements Iterator<E>{
        private Stack<E> elements;

        /**
         * Constructs a DescendingIterator.
         */
        public DescendingIterator() {
            elements = new Stack<>();
            for(E element : theData){
                elements.push(element);
            }
        }

        /**
         * Returns true if the iteration has more elements.
         * @return true if the iteration has more elements
         */
        public boolean hasNext() {
            return !elements.isEmpty();
        }

        /**
         * Returns the next element in the iteration.
         * @return the next element in the iteration
         */
        public E next() {
            if(!hasNext()){
                throw new NoSuchElementException();
            }
            return elements.pop();
        }
    }
}
