public interface KWList<E>{
    int size();
    boolean add(E obj);
    void add(int index, E obj);
    E get(int index);
    E set(int index, E e);
    E remove(int index);
}