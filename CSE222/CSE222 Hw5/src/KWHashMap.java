/** KWHashMap Interface */
public interface KWHashMap<K, V> {
    /**
     * Returns the value associated with the specified key. Returns null if the key is not present.
     * @param key key
     * @return the value associated with the specified key
     */
    V get(Object key);

    /**
     * Returns true if this table contains no key‐value mappings.
     * @return true if this table contains no key‐value mappings
     */
    boolean isEmpty();

    /**
     * Associates the specified value with the specified key. Returns the previous value
     * associated with the specified key, or null if there was no mapping for the key.
     * @param key key
     * @param value value
     * @return the previous value associated with the specified key
     */
    V put(K key, V value);

    /**
     * Removes the mapping for this key from this table if it is present. Returns the previous
     * value associated with the specified key, or null if there was no mapping.
     * @param key key
     * @return the previous value associated with the specified key
     */
    V remove(Object key);

    /**
     * Returns the size of the table.
     * @return the size of the table
     */
    int size();
}
