/**
 * This is a generic class with one generic parameter which is the generic type E
 */
public interface Collection<E>{
    /**
     * Returns an iterator over the collection
     * @return Iterator over the collection
     */
    Iterator<E> iterator();
    /**
     * Ensures that this collection contains the specified element
     * @param e Element to add
     * @return true if the collection changed after this call
     */
    boolean add(E e);
    /**
     * Adds all of the elements in the specified collection to this collection
     * @param c Collection whose elements will be added
     * @return true if this collection changed after this call
     */
    boolean addAll(Collection<E> c);
    /**
     * Removes all of the elements from this collection
     */
    void clear();
    /**
     * Returns true if this collection contains the specified element
     * @param e Element to be searched in this collection
     * @return true if this collection contains the specified element
     */
    boolean contains(E e);
    /**
     * Returns true if this collection contains all of the elements in the specified collection
     * @param c Collection whose elements will be searched in this collection
     * @return true if this collection contains all of the elements in the specified collection
     */
    boolean containsAll(Collection<E> c);
    /**
     * Returns true if this collection contains no elements
     * @return true if this collection contains no elements
     */
    boolean isEmpty();
    /**
     * Removes a single instance of the specified element from this collection, if it is present
     * @param e Element which will be removed
     * @return true if the given element is removed from this collection
     */
    boolean remove(E e);
    /**
     * Removes all of this collection's elements that are also contained in the specified collection
     * @param c Collection whose elements will be removed from this collection
     * @return true if this collection changed after this call
     */
    boolean removeAll(Collection<E> c);
    /**
     * Retains only the elements in this collection that are contained in the specified collection
     * @param c Collection whose elements will be retained in this collection
     * @return true if this collection changed after this call
     */
    boolean retainAll(Collection<E> c);
    /**
     * Returns the number of elements in this collection
     * @return The number of elements in this collection
     */
    int size();
}
