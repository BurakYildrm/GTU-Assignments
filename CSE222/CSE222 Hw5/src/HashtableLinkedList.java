import java.util.LinkedList;

/** Hash table implementation using chaining with LinkedList. */
@SuppressWarnings("unchecked")
public class HashtableLinkedList<K, V> implements KWHashMap<K, V> {

    /** Contains key‐value pairs for a hash table. */
    private static class Entry<K, V> {
        /** The key */
        private final K key;
        /** The value */
        private V value;
        /**
         * Creates a new key‐value pair.
         * @param key The key
         * @param value The value
         */
        public Entry(K key, V value) {
            this.key = key;
            this.value = value;
        }
        /** Retrieves the key.
         * @return The key
         */
        public K getKey() {
            return key;
        }
        /** Retrieves the value.
         * @return The value
         */
        public V getValue() {
            return value;
        }
        /** Sets the value.
         * @param val The new value
         * @return The old value
         */
        public V setValue(V val) {
            V oldVal = value;
            value = val;
            return oldVal;
        }
    }

    /** The table */
    private LinkedList<Entry<K, V>>[] table;
    /** The number of keys */
    private int numKeys;
    /** The capacity */
    private static final int CAPACITY = 101;
    /** The maximum load factor */
    private static final double LOAD_THRESHOLD = 3.0;

    /**
     * Constructs an empty hash map with initial capacity.
     */
    public HashtableLinkedList() {
        table = new LinkedList[CAPACITY];
    }

    /** Method get for class HashtableLinkedList.
     * @param key The key being sought
     * @return The value associated with this key if found;
     otherwise, null
     */
    @Override
    public V get(Object key) {
        int index = key.hashCode() % table.length;
        if (index < 0)
            index += table.length;
        if (table[index] == null)
            return null;

        for (Entry<K, V> nextItem : table[index]) {
            if (nextItem.getKey().equals(key))
                return nextItem.getValue();
        }

        return null;
    }

    /** Method put for class HashtableLinkedList.
     * @post This key‐value pair is inserted in the
     table and numKeys is incremented. If the key is already in the table, its value is changed to the argument value and numKeys is not changed.
     * @param key The key of item being inserted
     * @param value The value for this key
     * @return The old value associated with this key if
     found; otherwise, null
     */
    @Override
    public V put(K key, V value) {
        int index = key.hashCode() % table.length;
        if (index < 0)
            index += table.length;
        if (table[index] == null) {
            table[index] = new LinkedList<>();
        }

        for (Entry<K, V> nextItem : table[index]) {
            if (nextItem.getKey().equals(key)) {
                V oldVal = nextItem.getValue();
                nextItem.setValue(value);
                return oldVal;
            }
        }

        table[index].addFirst(new Entry<>(key, value));
        numKeys++;
        if (numKeys > (LOAD_THRESHOLD * table.length))
            rehash();
        return null;
    }

    /** Expands table size when loadFactor exceeds LOAD_THRESHOLD
     * @post The size of the table is doubled and is an odd integer.
     Each entry from the original table is reinserted into the expanded table.
     The value of numKeys is reset to the number of items actually inserted.
     */
    private void rehash() {
        LinkedList<Entry<K, V>>[] oldTable = table;
        table = new LinkedList[2 * oldTable.length + 1];
        numKeys = 0;
        for (LinkedList<Entry<K, V>> entries : oldTable) {
            if(entries != null) {
                for (Entry<K, V> entry : entries) {
                    put(entry.getKey(), entry.getValue());
                }
            }
        }
    }

    /**
     * Returns true if the hash map is empty.
     * @return true if the hash map is empty.
     */
    public boolean isEmpty(){
        return numKeys == 0;
    }

    /**
     * Removes the key-value pair associated with the given key.
     * @param key key
     * @return value of key-value pair associated with the given key
     */
    public V remove(Object key) {
        int index = key.hashCode() % table.length;
        if(index < 0){
            index += table.length;
        }
        if(table[index] == null){
            return null;
        }

        for(Entry<K, V> entry : table[index]){
            if(entry.getKey().equals(key)){
                V value = entry.getValue();
                numKeys--;
                table[index].remove(entry);
                if(table[index].isEmpty()){
                    table[index] = null;
                }
                return value;
            }
        }
        return null;
    }

    /**
     * Returns the number of elements in the hash map.
     * @return the number of elements in the hash map
     */
    public int size() {
        return numKeys;
    }
}