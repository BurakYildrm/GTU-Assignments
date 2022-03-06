public class HashSet<E> implements Set<E> {
    /**
     * Returns an iterator over the collection
     * @return iterator over the collection
     */
    @Override
    public Iterator<E> iterator(){
        return new Iterator<E>(arr, this);
    }
    /**
     * Adds the given element to this set if it is not already in this set
     * @param e Given element to add to this set
     * @return true if the given element is not already in this set
     */
    @SuppressWarnings("unchecked")
    @Override
    public boolean add(E e){
        int check = 0;
        for(int i = 0; i < arr.length; i++){
            if(arr[i] == e) return false;
        }
        if(check == 0){
            temp = arr;
            arr = (E[]) new Object[temp.length + 1];
            for(int i = 0; i < temp.length; i++){
                arr[i] = temp[i];
            }
            arr[arr.length - 1] = e;
        }
        return true;
    }
    /**
     * Adds all of the specified collection's elements to this set if they are not already in this set
     * @param c Specified collection to add to the this set
     * @return true if this set is changed after this call
     */
    @Override
    public boolean addAll(Collection<E> c){
        Iterator<E> iterator = c.iterator();
        int counter = 0;
        while(iterator.hasNext()){
            if(this.add(iterator.next()) == false) counter++; 
        }
        if(counter == c.size()) return false;
        return true;
    }
    /**
     * removes all the elements in this list
     */
    @SuppressWarnings("unchecked")
    @Override
    public void clear(){
        arr = (E[]) new Object[0];
        System.gc();
    }
    /**
     * Checks if the given parameter is present in this set
     * @param e Parameter which will be searched in this set
     * @return true if the given parameter is in this set
     */
    @Override
    public boolean contains(E e){
        for(E var : arr){
            if(var == e) return true;
        }
        return false;
    }
    /**
     * Checks if all of the given collection's elements are in this set
     * @param c Collection whose elements will be searched in this set
     * @return true if this set has all of the given collection's elements
     */
    @Override
    public boolean containsAll(Collection<E> c){
        Iterator<E> iterator = c.iterator();
        int counter = 0;
        E temporary;
        while(iterator.hasNext()){
            temporary = iterator.next();
            for(int j = 0; j < arr.length; j++){
                if(temporary == arr[j]){
                    counter++;
                    break;
                }
            }
        }
        if(counter == c.size()) return true;
        return false;
    }
    /**
     * Checks if the set is empty
     * @return true if this set has no elements in it
     */
    @Override
    public boolean isEmpty(){
        if(arr.length == 0) return true;
        return false;
    }
    /**
     * Removes the given element from this set
     * @param e Element which will be removed
     * @return true if the given element is in this set
     */
    @SuppressWarnings("unchecked")
    @Override
    public boolean remove(E e){
        int index = -1;
        for(int i = 0; i < arr.length; i++){
            if(arr[i] == e){
                index = i;
                break;
            }
        }
        if(index == -1) return false;
        temp = arr;
        arr = (E[])new Object[temp.length - 1];
        for(int i = 0; i < index; i++){
            arr[i] = temp[i];
        }
        for(int i = index; i < arr.length; i++){
            arr[i] = temp[i + 1];
        }
        return true;
    }
    /**
     * Removes all of the elements of given collection if they are present in this set
     * @param c Collection whose elements will be removed
     * @return true if this set changed after this call
     */
    @Override
    public boolean removeAll(Collection<E> c){
        Iterator<E> iterator = c.iterator();
        E temporary;
        int size = arr.length;
        while(iterator.hasNext()){
            temporary = iterator.next();
            for(int j = 0; j < arr.length; j++){
                if(arr[j] == temporary){
                    this.remove(temporary);
                }
            }
        }
        if(size == arr.length) return false;
        return true;
    }
    /**
     * Retains the elements of the given collection in this set
     * @param c Collection whose elements will be retained in this set
     * @return true if this set changed after this call
     */
    @SuppressWarnings("unchecked")
    @Override
    public boolean retainAll(Collection<E> c){
        int counter = 0;
        temp = arr;
        arr = (E[]) new Object[0];
        Iterator<E> iterator = c.iterator();
        while(iterator.hasNext()){
            this.add(iterator.next());
        }
        if(temp.length != arr.length) return true;
        else{
            for(int i = 0; i < temp.length; i++){
                for(int j = 0; j < arr.length; j++){
                    if(temp[i] == arr[j]){
                        counter++;
                        break;
                    }
                }
            }
            if(counter == temp.length) return false;
            return true;
        }
    }
    /**
     * Returns the length of this list
     * @return The length of this list
     */
    @Override
    public int size(){
        return arr.length;
    }
    /**
     * Constructs an empty set
     */
    @SuppressWarnings("unchecked")
    public HashSet(){
        arr = (E[]) new Object[0];
    }
    private E[] arr, temp;
}
