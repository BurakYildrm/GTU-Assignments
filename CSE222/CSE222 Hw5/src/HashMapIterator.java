/** Interface that represents a map iterator. */
public interface HashMapIterator<K> {
    /**
     * Returns true if there is still at least one key that is not iterated.
     * @return true if there is still at least one key that is not iterated
     */
    boolean hasNext();
    /**
     * Returns the next key.
     * @return the next key
     */
    K next();
    /**
     * Returns the previous key.
     * @return the previous key
     */
    K prev();
}
