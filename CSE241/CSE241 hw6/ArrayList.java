public class ArrayList<E> implements List<E> {
    /**
     * Returns an iterator over the collection
     * @return iterator over the collection
     */
    @Override
    public Iterator<E> iterator(){
        return new Iterator<E>(arr, this);
    }
    /**
     * Appends the specified element to the end of this list
     * @param e Specified element to append to the end of this list
     * @return true
     */
    @SuppressWarnings("unchecked")
    @Override
    public boolean add(E e){
        temp = arr;
        arr = (E[]) new Object[temp.length + 1];
        for(int i = 0; i < temp.length; i++){
            arr[i] = temp[i];
        }
        arr[arr.length - 1] = e;
        return true;
    }
    /**
     * Appends all of the specified collection's elements to the end of this list
     * @param c Specified collection to append to the end of this list
     * @return true
     */
    @Override
    public boolean addAll(Collection<E> c){
        Iterator<E> iterator = c.iterator();
        while(iterator.hasNext()){
            this.add(iterator.next()); 
        }
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
     * Checks if the given parameter is present in this list
     * @param e Parameter which will be searched in this list
     * @return true if the given parameter is in this list
     */
    @Override
    public boolean contains(E e){
        for(E var : arr){
            if(var == e) return true;
        }
        return false;
    }
    /**
     * Checks if all of the given collection's elements are in this list
     * @param c Collection whose elements will be searched in this list
     * @return true if this list has all of the given collection's elements
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
     * Checks if the list is empty
     * @return true if this list has no elements in it
     */
    @Override
    public boolean isEmpty(){
        if(arr.length == 0) return true;
        return false;
    }
    /**
     * Returns the element at the given index
     * @param index Index of the returned element
     * @return The element at the given index
     */
    public E get(int index){
        return arr[index];
    }
    /**
     * Removes the given element from this list
     * @param e Element which will be removed
     * @return true if the given element is in this list
     */
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
        this.remove(index);
        return true;
    }
    /**
     * Removes the element at the given index from this list
     * @param index Index of the element which will be removed
     * @exception ArrayIndexOutOfBoundsException on input error
     */
    @SuppressWarnings("unchecked")
    public void remove(int index){
        if(index < 0 || index >= arr.length) throw new ArrayIndexOutOfBoundsException();
        temp = arr;
        arr = (E[])new Object[temp.length - 1];
        for(int i = 0; i < index; i++){
            arr[i] = temp[i];
        }
        for(int i = index; i < arr.length; i++){
            arr[i] = temp[i + 1];
        }
    }
    /**
     * Removes all of the elements of given collection if they are present in this list
     * @param c Collection whose elements will be removed
     * @return true if this list changed after this call
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
     * Retains the elements of the given collection in this list
     * @param c Collection whose elements will be retained in this list
     * @return true if this list changed after this call
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
     * Replaces the element in the given index with the given element
     * @param index index of the element to change
     * @param e element which will replace the element in given index
     */
    public void set(int index, E e){
        arr[index] = e;
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
     * Constructs an empty list
     */
    @SuppressWarnings("unchecked")
    public ArrayList(){
        arr = (E[]) new Object[0];
    }
    private E[] arr, temp;
}