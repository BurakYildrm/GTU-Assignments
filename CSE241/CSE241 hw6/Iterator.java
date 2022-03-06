import java.util.NoSuchElementException;

public class Iterator<E> {
    private int currentIndex, counter = 0;
    private E [] arr;
    private Collection<E> var;
    @SuppressWarnings("unused")
    private boolean check;
    private ArrayList<E> var1;
    /**
     * Returns true if the iteration has more elements
     * @return true if the iteration has more elements
     */
    boolean hasNext(){
        if(currentIndex + 1 < var.size()) return true;
        return false;
    }
    /**
     * Returns the next element in the iteration and advances the iterator
     * @exception NoSuchElementException if there is no next element in the iteration
     * @return Next element in the iteration
     */
    E next(){
        if(this.hasNext() == true){
            check = true;
            counter = 0;
            currentIndex++;
            return arr[currentIndex];
        }
        throw new NoSuchElementException();
    }
    /**
     * Removes from the underlying collection the last element returned by this iterator. This method does not work for Queues, it throws an exception. This method can only be called per next() method call.
     * @throws DoesNotWorkForQueues if the collection calling this iterator is an instance of Queue interface
     */
    void remove() throws DoesNotWorkForQueues{
        if(var instanceof Queue){
            throw new DoesNotWorkForQueues();
        }
        if(counter > 1) throw new IllegalStateException();
        check = false;
        counter++;
        if(var instanceof ArrayList){
            var1 = (ArrayList<E>)var;
            var1.remove(currentIndex);
        }
        else if(var instanceof HashSet){
            var.remove(arr[currentIndex]);
        }
    }
    /**
     * Constructs an iterator with the given array and sets the currentIndex to -1
     * @param array array which will be iterated over
     * @param var collection which calls this iterator
     */
    Iterator(E[] array, Collection<E> var){
        this.var = var;
        currentIndex = -1;
        arr = array;
    }
}