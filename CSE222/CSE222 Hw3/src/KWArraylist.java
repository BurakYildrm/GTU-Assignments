import java.util.AbstractList;
import java.util.Arrays;

public class KWArraylist<E> extends AbstractList<E> implements KWList<E>{
    private static final int INITIAL_CAPACITY = 10;
    private E[] theData;
    private int size = 0;
    private int capacity = 0;
    @SuppressWarnings("unchecked")
    /**
     * creates a KWArraylist with initial capacity of 10
     */
    public KWArraylist() {
        capacity = INITIAL_CAPACITY;
        theData = (E[]) new Object[capacity];
    }

    /**
     * @return size oo KWArraylist
     */
    public int size() {
        return size;
    }

    /**
     * reallocates memory if size equals capacity
     */
    private void reallocate() {
        capacity = 2 * capacity;
        theData = Arrays.copyOf(theData, capacity);
    }

    /**
     * adds given entry to KWArraylist
     * @param anEntry
     * @return true
     */
    public boolean add(E anEntry) {
        if (size == capacity) {
            reallocate();
        }
        theData[size] = anEntry;
        size++;
        return true;
    }

    /**
     * adds given entry to given index
     * @param index index to add
     * @param anEntry entry to add
     */
    public void add(int index, E anEntry) {
        if (index < 0 || index > size) {
            throw new ArrayIndexOutOfBoundsException(index);
        }
        if (size == capacity) {
            reallocate();
        }
        for(int i = size; i > index; i--){
            theData[i] = theData[i - 1];
        }
        theData[index] = anEntry;
        size++;
    }

    /**
     * returns the element in specified index
     * @param index index of element to return
     * @return element at specified index
     */
    public E get(int index) {
        if (index < 0 || index >= size) {
            throw new ArrayIndexOutOfBoundsException(index);
        }
        return theData[index];
    }

    /**
     * replaces the element at specified index with the new value
     * @param index index of element to replace
     * @param newValue new value
     * @return replaced element
     */
    public E set(int index, E newValue) {
        if (index < 0 || index >= size) {
            throw new ArrayIndexOutOfBoundsException(index);
        }
        E oldValue = theData[index];
        theData[index] = newValue;
        return oldValue;
    }

    /**
     * removes the element at specified index
     * @param index index of element to remove
     * @return removed element
     */
    public E remove(int index) {
        if (index < 0 || index >= size) {
            throw new ArrayIndexOutOfBoundsException(index);
        }
        E returnValue = theData[index];
        for (int i = index + 1; i < size; i++) {
            theData[i - 1] = theData[i];
        }
        size--;
        return returnValue;
    }
}