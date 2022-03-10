import java.util.*;

public class HybridList<E> implements Iterable<E>{
    private KWLinkedList<KWArraylist<E> > theData;
    private int MAX_NUMBER = 15;

    /**
     * adds the given element to the arraylist at the tail of the linkedlist
     * @param element element to add
     */
    public void add(E element){
        if(theData.size() == 0){
            KWArraylist<E> temp = new KWArraylist<>();
            temp.add(element);
            addAll(temp);
        }
        else {
            add(size() - 1, element);
        }
    }

    /**
     * adds the given element to the arraylist at specified index
     * @param arrayIndex index of arraylist to add
     * @param element element to add
     */
    public void add(int arrayIndex, E element){
        KWArraylist<E> target = theData.get(arrayIndex);
        if(target.size() == MAX_NUMBER){
            for(int i = arrayIndex + 1; i < theData.size(); i++){
                KWArraylist<E> temp = theData.get(i);
                if(temp.size() != MAX_NUMBER){
                    temp.add(element);
                    return;
                }
            }
            KWArraylist<E> newArray = new KWArraylist<>();
            newArray.add(element);
            theData.add(newArray);
        }
        else{
            target.add(element);
        }
    }

    /**
     * adds given arraylist to the linkedlist
     * @param obj arraylist to add
     */
    public void addAll(KWArraylist<E> obj){
        addAll(theData.size(), obj);
    }

    /**
     * adds the given arraylist to the specified index of linkedlist
     * @param index index to add
     * @param obj arraylist to add
     */
    public void addAll(int index, KWArraylist<E> obj){
        KWArraylist<E> temp = null;
        if(obj != null && obj.size() != 0) {
            if (obj.size() >= MAX_NUMBER) {
                int size = obj.size();
                temp = new KWArraylist<>();
                for (int i = MAX_NUMBER; i < size; i++) {
                    temp.add(obj.get(i));
                }
                for (int i = size; i >= MAX_NUMBER; i--) {
                    obj.remove(obj.size() - 1);
                }
            }
            theData.add(index, obj);
            if (temp != null) {
                theData.add(index + 1, temp);
            }
        }
    }

    /**
     * removes the arraylist at specified index
     * @param index index of arraylist to remove
     * @return removed arraylist
     */
    public KWArraylist<E> remove(int index){
        return theData.remove(index);
    }

    /**
     * removes the element at specified index of specified arraylist
     * @param arrayIndex arraylist index of element to remove
     * @param elementIndex index of element to remove
     * @return removed element
     */
    public E remove(int arrayIndex, int elementIndex){
        KWArraylist<E> target = theData.get(arrayIndex);
        E removedValue = target.remove(elementIndex);
        if(target.size() == 0){
            remove(arrayIndex);
        }
        return removedValue;
    }

    /**
     * returns the arraylist at specified index
     * @param arrayIndex index of arraylist to return
     * @return arraylist at specified index
     */
    public List<E> get(int arrayIndex){
        return Collections.unmodifiableList(theData.get(arrayIndex));
    }

    /**
     * returns the element at specied index and specified arraylist
     * @param arrayIndex arraylist index of element to return
     * @param elementIndex index of element to return
     * @return element at specied index and specified arraylist
     */
    public E get(int arrayIndex, int elementIndex){
        return theData.get(arrayIndex).get(elementIndex);
    }

    /**
     * replaces the element at specified arraylist and specified index with new element
     * @param arrayIndex arraylist index of element to replace
     * @param elementIndex index of element to replace
     * @param element new element
     */
    public void set(int arrayIndex, int elementIndex, E element){
        theData.get(arrayIndex).set(elementIndex, element);
    }

    /**
     * replace the arraylist at specified index with new arraylist
     * @param arrayIndex index to replace
     * @param arraylist new arraylist
     */
    public void setAll(int arrayIndex, KWArraylist<E> arraylist){
        theData.set(arrayIndex, arraylist);
    }

    /**
     * returns the size of hybridlist
     * @return size of hybridlist
     */
    public int size(){
        return theData.size();
    }

    /**
     * constructs a new hybridlist
     */
    public HybridList() {
        theData = new KWLinkedList<>();
    }

    public String toString(){
        ListIterator<KWArraylist<E>> listIterator = theData.listIterator();
        if(!listIterator.hasNext()){
            return "[]";
        }
        StringBuilder s = new StringBuilder();
        s.append("[");
        while (listIterator.hasNext()){
            s.append(listIterator.next());
            if(!listIterator.hasNext()){
                s.append("]");
            }
            else {
                s.append(",\n ");
            }
        }
        return s.toString();
    }

    public Iterator<E> iterator() {
        return new HybridListIter();
    }

    private class HybridListIter implements Iterator<E>{
        Iterator<E> iteratorInner;
        Iterator<KWArraylist<E> > iteratorOuter;
        KWArraylist<E> arraylist;
        int countNode = 1;
        int countArrayElements = 0;

        /**
         * constructs a HybridListIter
         */
        private HybridListIter(){
            if(!theData.isEmpty()){
                iteratorOuter = theData.iterator();
                arraylist = iteratorOuter.next();
                iteratorInner = arraylist.iterator();
            }
            else{
                iteratorInner = null;
                iteratorOuter = null;
                arraylist = null;
            }
        }

        /**
         * returns true if rest of the hybridlist has at least one element
         * @return true if rest of the hybridlist has at least one element
         */
        public boolean hasNext() {
            return countNode != theData.size() || countArrayElements != arraylist.size();
        }

        /**
         * returns the next element in current arraylist
         * @return the next element in current arraylist
         */
        public E next() {
            if(!hasNext())
                throw new NoSuchElementException();
            if(countArrayElements == arraylist.size()){
                arraylist = iteratorOuter.next();
                iteratorInner = arraylist.iterator();
                countNode++;
                countArrayElements = 0;
            }
            return arraylist.get(countArrayElements++);
        }
    }
}