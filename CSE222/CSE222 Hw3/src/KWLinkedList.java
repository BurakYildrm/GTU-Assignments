import java.util.Iterator;
import java.util.ListIterator;
import java.util.NoSuchElementException;

public class KWLinkedList<E> implements KWList<E>, Iterable<E>{
    private Node<E> head = null;
    private Node<E> tail = null;
    private int size = 0;

    private class Node<E> {
        private E data;
        private Node<E> next = null;
        private Node<E> prev = null;

        /**
         * creates a node and assigns given value to data field
         * @param dataItem value which will be assigned to data field
         */
        private Node(E dataItem) {
            data = dataItem;
        }
    }

    /**
     * adds given element to list
     * @param obj element to add
     * @return true
     */
    public boolean add(E obj){
        add(size, obj);
        return true;
    }

    /**
     * @return size of the list
     */
    public int size() {
        return size;
    }

    /**
     * @return true if the list is empty
     */
    public boolean isEmpty(){
        return size == 0;
    }

    /**
     * adds given element to specified index
     * @param index index to add
     * @param obj element to add
     */
    public void add(int index, E obj) {
        listIterator(index).add(obj);
    }

    /**
     * adds the given element to the head of the list
     * @param obj element to add
     */
    public void addFirst(E obj){
        listIterator(0).add(obj);
    }

    /**
     * adds the given element to the tail of the list
     * @param obj
     */
    public void addLast(E obj){
        listIterator(size).add(obj);
    }

    /**
     * returns an listIterator over the elements of list
     * @param index index to start
     * @return an listIterator over the elements of list
     */
    public ListIterator<E> listIterator(int index) {
        return new KWListIter(index);
    }

    /**
     * returns an listIterator over the elements of list
     * @return an listIterator over the elements of list
     */
    public ListIterator<E> listIterator() {
        return new KWListIter();
    }

    /**
     * returns an Iterator over the elements of list
     * @return an Iterator over the elements of list
     */
    public Iterator<E> iterator() {
        return new KWListIter();
    }

    /**
     * returns the element at specified index
     * @param index index to element to return
     * @return element at specified index
     */
    public E get(int index) {
        return listIterator(index).next();
    }

    /**
     * returns the first element of the list
     * @return the first element of the list
     */
    public E getFirst(){
        if(head == null){
            return null;
        }
        return head.data;
    }

    /**
     * returns the last element of the list
     * @return the last element of the list
     */
    public E getLast(){
        if(tail == null){
            return null;
        }
        return tail.data;
    }

    /**
     * replaces the element at specified index with given element
     * @param index index of element to replace
     * @param element new element
     * @return replaced element
     */
    public E set(int index, E element){
        Node<E> temp = getNode(index);
        if(temp == null){
            throw new IndexOutOfBoundsException();
        }
        E oldValue = temp.data;
        temp.data = element;
        return oldValue;
    }

    /**
     * returns the node at specified index
     * @param index index of node to return
     * @return the node at specified index
     */
    private Node<E> getNode(int index){
        Node<E> node = head;
        for(int i = 0; i < index; i++){
            node = node.next;
        }
        return node;
    }

    /**
     * removes the element at specified index from the list
     * @param index index of element to remove
     * @return removed element
     */
    public E remove(int index){
        Node<E> temp = getNode(index);
        if(temp == null){
            throw new IndexOutOfBoundsException();
        }
        return removeFromList(temp);
    }

    /**
     * removes the given element from the list
     * @param obj element to remove
     * @return true if removal is done successfully
     */
    public boolean remove(E obj){
        ListIterator<E> listIterator = listIterator();
        while (listIterator.hasNext()){
            if(listIterator.next() == obj){
                listIterator.remove();
                return true;
            }
        }
        return false;
    }

    public String toString(){
        StringBuilder stringBuilder = new StringBuilder();
        ListIterator<E> iter = listIterator();
        if(!iter.hasNext()) {
            return "[]";
        }
        stringBuilder.append("[");
        while (iter.hasNext()){
            E e = iter.next();
            stringBuilder.append(e);
            if(!iter.hasNext()){
                stringBuilder.append("]");
            }
            else{
                stringBuilder.append(", ");
            }
        }
        return stringBuilder.toString();
    }

    /**
     * removes the given node from the list
     * @param nodeToRemove node to remove
     * @return data field of removed node
     */
    private E removeFromList(Node<E> nodeToRemove){
        if(nodeToRemove.prev == null){
            head = nodeToRemove.next;
        }
        else{
            nodeToRemove.prev.next = nodeToRemove.next;
        }

        if(nodeToRemove.next == null){
            tail = nodeToRemove.prev;
        }
        else{
            nodeToRemove.next.prev = nodeToRemove.prev;
        }

        nodeToRemove.prev = null;
        nodeToRemove.next = null;

        size--;
        return nodeToRemove.data;
    }

    private class KWListIter implements ListIterator<E> {
        private Node<E> nextItem;
        private Node<E> lastItemReturned;
        private int index = 0;

        /**
         * constructs a KWListIter at given index
         * @param i index to start
         */
        public KWListIter(int i) {
            if (i < 0 || i > size) {
                throw new IndexOutOfBoundsException("Invalid index " + i);
            }
            lastItemReturned = null;
            if (i == size) {
                index = size;
                nextItem = null;
            }
            else {
                nextItem = head;
                for (index = 0; index < i; index++) {
                    nextItem = nextItem.next;
                }
            }
        }

        /**
         * constructs a KWListIter
         */
        public KWListIter(){
            this(0);
        }

        /**
         * returns true if current node has a next node
         * @return true if current node has a next node
         */
        public boolean hasNext() {
            return nextItem != null;
        }

        /**
         * returns the data field of the next node
         * @return data field of the next node
         */
        public E next() {
            if (!hasNext()) {
                throw new NoSuchElementException();
            }
            lastItemReturned = nextItem;
            nextItem = nextItem.next;
            index++;
            return lastItemReturned.data;
        }

        /**
         * returns true if current node has a previous node
         * @return true if current node has a previous node
         */
        public boolean hasPrevious() {
            if(size == 0){
                return false;
            }
            return nextItem == null || nextItem.prev != null;
        }
        /**
         * returns the data field of the previous node
         * @return data field of the previous node
         */
        public E previous() {
            if (!hasPrevious()) {
                throw new NoSuchElementException();
            }
            if (nextItem == null) {
                nextItem = tail;
            } else {
                nextItem = nextItem.prev;
            }
            lastItemReturned = nextItem;
            index--;
            return lastItemReturned.data;
        }

        /**
         * returns the index of current node's next node
         * @return ndex of current node's next node
         */
        public int nextIndex() {
            return index;
        }
        /**
         * returns the index of current node's previous node
         * @return index of current node's previous node
         */
        public int previousIndex() {
            return index - 1;
        }

        /**
         * removes the node which is returned by next()
         */
        public void remove() {
            if(lastItemReturned == null){
                throw new IllegalStateException();
            }
            removeFromList(lastItemReturned);
            lastItemReturned = null;
        }

        /**
         * replaces the data field of the node which is returned by next() with given element
         * @param e new element
         */
        public void set(E e) {
            if(lastItemReturned == null){
                throw new IllegalStateException();
            }
            lastItemReturned.data = e;
        }

        /**
         * adds the given element to the next of last node returned
         * @param obj element to add
         */
        public void add(E obj) {
            if (head == null) { // Add to an empty list.
                head = new Node<>(obj);
                tail = head;
            } else if (nextItem == head) {
                Node<E> newNode = new Node<>(obj);
                newNode.next = nextItem;
                nextItem.prev = newNode;
                head = newNode;
            } else if (nextItem == null) {
                Node<E> newNode = new Node<>(obj);
                tail.next = newNode;
                newNode.prev = tail;
                tail = newNode;
            } else {
                Node<E> newNode = new Node<>(obj);
                newNode.prev = nextItem.prev;
                nextItem.prev.next = newNode;
                newNode.next = nextItem;
                nextItem.prev = newNode;
            }
            size++;
            index++;
            lastItemReturned = null;
        }
    }
}