/**
 * Queues order elements in a FIFO (first-in-first-out) manner. There is no random access with this Collection. Some functions throw exceptions
 */
public interface Queue<E> extends Collection<E>{
    /**
     * Inserts the specified element into this queue
     * @param e Element to insert
     * @return true
     */
    boolean add(E e);
    /**
     * Retrieves, but does not remove, the head of this queue
     * @return The head of this queue
     * @throws EmptyListException if this queue is empty
     */
    E element() throws EmptyListException;
    /**
     * nserts the specified element into this queue
     * @param e Element to insert
     * @return true
     */
    boolean offer(E e);
    /**
     * Retrieves and removes the head of this queue
     * @return The head of this queue
     * @throws EmptyListException if this queue is empty
     */
    E poll() throws EmptyListException;
}