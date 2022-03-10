import java.io.Serializable;
import java.util.HashMap;

/** HashMap implementation with iterator. */
public class HashMapWithIterator<K, V> extends HashMap<K, V> implements Serializable {

    /**
     * Returns an iterator over the keys.
     * @return an iterator over the keys
     */
    public HashMapIterator<K> iterator(){
        return new MapIterator();
    }

    /**
     * Returns an iterator over the keys starting from the given key.
     * @return an iterator over the keys starting from the given key
     */
    public HashMapIterator<K> iterator(K key){
        return new MapIterator(key);
    }

    /**
     * Class that represents a map iterator.
     */
    @SuppressWarnings("unchecked")
    private class MapIterator implements HashMapIterator<K> {
        /** Key array */
        private final K[] keys = (K[]) keySet().toArray();
        /** Counter of next() method  */
        private int nextCount = 0;
        /** Counter of prev() method  */
        private int prevCount = 0;
        /** Start index of the iterator */
        private int startIndex;

        /**
         * Constructs a map iterator starting from index 0.
         */
        public MapIterator() {
            startIndex = 0;
        }

        /**
         * Constructs a map iterator starting from the index of the given key.
         * @param key key
         */
        public MapIterator(K key) {
            startIndex = 0;
            for(K k : keys){
                if(k.equals(key)){
                    break;
                }
                startIndex++;
            }
            if(startIndex >= keys.length){
                startIndex = 0;
            }
        }

        /**
         * Returns true if there is still at least one key that is not iterated.
         * @return true if there is still at least one key that is not iterated
         */
        public boolean hasNext(){
            return (nextCount != keys.length) && (prevCount != keys.length);
        }

        /**
         * Returns the next key.
         * @return the next key
         */
        public K next(){
            if(!hasNext()){
                return keys[0];
            }
            nextCount++;
            if(prevCount != 0){
                prevCount--;
            }
            int currentIndex = startIndex;
            startIndex = (startIndex + 1) % keys.length;
            return keys[currentIndex];
        }

        /**
         * Returns the previous key.
         * @return the previous key
         */
        public K prev(){
            prevCount++;
            if(nextCount != 0){
                nextCount--;
            }
            if(startIndex == 0){
                startIndex = keys.length - 1;
            }
            else{
                startIndex--;
            }
            return keys[startIndex];
        }
    }
}
