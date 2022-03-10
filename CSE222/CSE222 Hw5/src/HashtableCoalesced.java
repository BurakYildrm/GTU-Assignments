import java.util.ArrayList;

/** Hash table implementation using open addressing with Coalesced hashing. */
@SuppressWarnings("unchecked")
public class HashtableCoalesced<K, V> implements KWHashMap<K, V> {

    /** Contains key‐value pairs for a hash table. */
    private static class Entry<K, V> {
        /** The key */
        private final K key;
        /** The value */
        private V value;
        /** Index of next element in the chain */
        private int next;
        /** Index of previous element in the chain */
        private int prev;
        /** Number of probes to get to this entry */
        private int probe = 0;

        /**
         * Creates a new key‐value pair with NULL as its prev and next.
         * @param key The key
         * @param value The value
         */
        public Entry(K key, V value) {
            this(key, value, NULL, NULL);
        }

        /**
         * Creates a new key-value pair with given next and prev indexes.
         * @param key key
         * @param value value
         * @param next next index
         * @param prev prev index
         */
        public Entry(K key, V value, int next, int prev){
            this.key = key;
            this.value = value;
            this.next = next;
            this.prev = prev;
        }

        /**
         * Retrieves the key.
         * @return The key
         */
        public K getKey() {
            return key;
        }

        /**
         * Retrieves the value.
         * @return The value
         */
        public V getValue() {
            return value;
        }

        /**
         * Sets the value.
         * @param val The new value
         * @return The old value
         */
        public V setValue(V val) {
            V oldVal = value;
            value = val;
            return oldVal;
        }

        /**
         * Sets the probe.
         * @param probe The new probe value
         */
        public void setProbe(int probe){
            this.probe = probe;
        }

        /**
         * Retrieves the probe.
         * @return The probe
         */
        public int getProbe() {
            return probe;
        }

        /**
         * Returns a string representation of this class.
         * @return a string representation of this class
         */
        public String toString() {
            if(next == NULL){
                if(prev == NULL){
                    return "Key: " + key + "\tnext: NULL\tprev: NULL";
                }
                return "Key: " + key + "\tnext: NULL\tprev: " + prev;
            }
            if(prev == NULL){
                return "Key: " + key + "\tnext: " + next + "\tprev: NULL";
            }
            return "Key: " + key + "\tnext: " + next + "\tprev: " + prev;
        }
    }

    /** The table */
    private Entry<K, V>[] table;
    /** The capacity */
    private static final int START_CAPACITY = 10;
    /** The maximum load factor */
    private static final double LOAD_THRESHOLD = 0.75;
    /** The number of keys */
    private int numKeys;
    /** Integer that indicates null */
    private static final int NULL = -1;

    /**
     * Constructs an empty hash map with initial capacity.
     */
    public HashtableCoalesced() {
        table = new Entry[START_CAPACITY];
    }

    /** Method get for class HashtableCoalesced.
     * @param key The key being sought
     * @return The value associated with this key if found;
    otherwise, null
     */
    @Override
    public V get(Object key) {
        int index = key.hashCode() % table.length;

        if(index < 0){
            index += table.length;
        }

        if(table[index] == null){
            return null;
        }

        while (index != NULL){
            if(table[index].getKey().equals(key)){
                return table[index].getValue();
            }
            index = table[index].next;
        }

        return null;
    }

    /**
     * Returns true if the hash map is empty.
     * @return true if the hash map is empty
     */
    @Override
    public boolean isEmpty() {
        return numKeys == 0;
    }

    /** Method put for class HashtableCoalesced.
     * @post This key‐value pair is inserted in the
    table and numKeys is incremented. If the key is already in the table, its value is changed to the argument value and numKeys is not changed.
     * @param key The key of item being inserted
     * @param value The value for this key
     * @return The old value associated with this key if
    found; otherwise, null
     */
    @Override
    public V put(K key, V value){
        int index = key.hashCode() % table.length;
        int probe = 0;

        if(index < 0){
            index += table.length;
        }

        if(table[index] == null){
            table[index] = new Entry<>(key, value);
            numKeys++;
            double loadFactor = (double) numKeys / table.length;
            if (loadFactor > LOAD_THRESHOLD){
                rehash();
            }
            return null;
        }

        while (true){
            if(table[index].getKey().equals(key)){
                return table[index].setValue(value);
            }
            if(table[index].next == NULL){
                probe = table[index].getProbe() + 1;
                int initialProbe = table[index].getProbe();
                int nextLocation = (index + probe*probe - initialProbe*initialProbe) % table.length;
                if(nextLocation < 0){
                    nextLocation += table.length;
                }
                while (table[nextLocation] != null){
                    probe++;
                    nextLocation = (index + probe*probe - initialProbe*initialProbe) % table.length;
                    if(nextLocation < 0){
                        nextLocation += table.length;
                    }
                }
                Entry<K, V> newEntry = new Entry<>(key, value, NULL, index);
                newEntry.setProbe(probe);
                table[nextLocation] = newEntry;
                table[index].next = nextLocation;
                numKeys++;
                break;
            }
            else {
                index = table[index].next;
            }
        }

        double loadFactor = (double) numKeys / table.length;
        if (loadFactor > LOAD_THRESHOLD){
            rehash();
        }
        return null;
    }

    /**
     * Removes the key-value pair associated with the given key.
     * @param key key
     * @return value of key-value pair associated with the given key
     */
    @Override
    public V remove(Object key) {
        int index = key.hashCode() % table.length;
        int deletedIndex;
        ArrayList<Integer> nextValues = new ArrayList<>();
        ArrayList<Integer> prevValues = new ArrayList<>();

        if(index < 0){
            index += table.length;
        }

        if(table[index] == null){
            return null;
        }

        while (index != NULL) {
            if (table[index].getKey().equals(key)) {
                numKeys--;
                V removedValue = table[index].getValue();
                deletedIndex = index;
                if (table[index].next == NULL) {
                    if (table[index].prev != NULL) {
                        table[table[index].prev].next = NULL;
                    }
                    table[index] = null;
                } else {
                    int temp;
                    int probeDifference;
                    int oldProbe = table[index].probe;
                    while (table[index].next != NULL) {
                        probeDifference = table[table[index].next].probe - oldProbe;
                        oldProbe = table[table[index].next].probe;
                        temp = table[index].next;
                        prevValues.add(table[index].prev);
                        if (table[table[index].next].next != NULL) {
                            nextValues.add(table[index].next);
                        }
                        table[index] = table[table[index].next];
                        table[index].probe -= probeDifference;
                        index = temp;
                    }
                    table[index] = null;
                    index = deletedIndex;
                    for (Integer nextValue : nextValues) {
                        table[index].next = nextValue;
                        index = nextValue;
                    }
                    index = deletedIndex;
                    for(Integer prevValue : prevValues){
                        table[index].prev = prevValue;
                        index = table[index].next;
                    }
                }
                return removedValue;
            }
            index = table[index].next;
        }

        return null;
    }

    /**
     * Returns the number of elements in the hash map.
     * @return the number of elements in the hash map
     */
    @Override
    public int size() {
        return numKeys;
    }

    /**
     * Expands table size when loadFactor exceeds LOAD_THRESHOLD
     * @post The size of the table is doubled and is an odd integer.
    Each entry from the original table is reinserted into the expanded table.
    The value of numKeys is reset to the number of items actually inserted.
     */
    private void rehash() {
        Entry<K, V>[] oldTable = table;
        table = new Entry[2 * oldTable.length + 1];

        numKeys = 0;
        for (Entry<K, V> entry : oldTable) {
            if (entry != null) {
                put(entry.getKey(), entry.getValue());
            }
        }
    }

    /**
     * Returns a string representation of this hash map.
     * @return a string representation of this hash map
     */
    @Override
    public String toString() {
        int count = 0;
        StringBuilder s = new StringBuilder();
        for(Entry<K, V> entry : table){
            s.append(count).append(")").append(entry).append("\n");
            count++;
        }
        return s.toString();
    }
}
