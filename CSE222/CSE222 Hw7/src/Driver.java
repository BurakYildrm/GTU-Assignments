import java.util.*;

enum TreeType{
    BOTH, RED_BLACK, AVL, NEITHER;
}

@SuppressWarnings("unchecked")
public class Driver {
    private static final BinarySearchTree<Integer>[] binarySearchTrees = new BinarySearchTree[40];
    private static final RedBlackTree<Integer>[] redBlackTrees = new RedBlackTree[40];
    private static final TwoThreeTree<Integer>[] twoThreeTrees = new TwoThreeTree[40];
    private static final BTree<Integer>[] bTrees = new BTree[40];
    private static final SkipList<Integer>[] skipLists = new SkipList[40];

    public static void main(String[] args) {
        Random random = new Random();

        System.out.println("-------------------------------------------PART 1-------------------------------------------");
        System.out.println("--------------SkipListSet Methods--------------");
        System.out.println("---------Adding---------");
        ArrayList<Integer> arrayList = new ArrayList<>();
        SkipListSet<Integer> skipListSet = new SkipListSet<>();
        for(int i = 0; i < 10; i++){
            arrayList.add(random.nextInt(1000));
        }
        System.out.println("Elements To Add");
        System.out.println(arrayList);
        System.out.println("Before Adding");
        System.out.println(skipListSet);
        System.out.println("After Adding");
        for(Integer i : arrayList){
            skipListSet.add(i);
        }
        System.out.println(skipListSet);

        System.out.println("\n--------Removing--------");
        System.out.println("Element To Remove: " + arrayList.get(0));
        System.out.println("Before Removing");
        System.out.println(skipListSet);
        System.out.println("After Removing");
        skipListSet.remove(arrayList.get(0));
        System.out.println(skipListSet);

        System.out.println("\n---descendingIterator---");
        Iterator<Integer> iterator = skipListSet.descendingIterator();
        while (iterator.hasNext()){
            System.out.print(iterator.next() + " ");
        }

        System.out.println("\n\n--------------AVLTreeSet Methods--------------");
        System.out.println("---------Adding---------");
        arrayList.clear();
        AVLTreeSet<Integer> avlTreeSet = new AVLTreeSet<>();
        for(int i = 0; i < 10; i++){
            arrayList.add(random.nextInt(1000));
        }
        System.out.println("Elements To Add");
        System.out.println(arrayList);
        System.out.println("Before Adding");
        System.out.println(avlTreeSet);
        System.out.println("After Adding");
        for(Integer i : arrayList){
            avlTreeSet.add(i);
        }
        System.out.println(avlTreeSet);

        System.out.println("\n--------Iterator--------");
        Iterator<Integer> iterator1 = avlTreeSet.iterator();
        while (iterator1.hasNext()){
            System.out.print(iterator1.next() + " ");
        }

        System.out.println("\n\n--------headSet--------");
        System.out.println("toElement: " + arrayList.get(0));
        System.out.println(avlTreeSet.headSet(arrayList.get(0)));

        System.out.println("\n--------tailSet--------");
        System.out.println("fromElement: " + arrayList.get(arrayList.size() - 1));
        System.out.println(avlTreeSet.tailSet(arrayList.get(arrayList.size() - 1)) + "\n");

        System.out.println("-------------------------------------------PART 2-------------------------------------------");
        System.out.println("Trying With A Red-Black Tree");
        RedBlackTree<Integer> redBlackTree = new RedBlackTree<>();
        redBlackTree.add(993);
        redBlackTree.add(211);
        redBlackTree.add(342);
        redBlackTree.add(867);
        redBlackTree.add(777);
        redBlackTree.add(311);
        redBlackTree.add(580);
        redBlackTree.add(318);
        redBlackTree.add(951);
        redBlackTree.add(948);
        redBlackTree.add(778);
        redBlackTree.add(86);
        redBlackTree.add(262);
        redBlackTree.add(259);
        redBlackTree.add(194);
        System.out.println(redBlackTree);
        System.out.println("Result: " + TreeType.values()[TypeChecker.checkType(redBlackTree)] + "\n");

        System.out.println("Trying With An AVL Tree");
        AVLTree<Integer> avlTree = new AVLTree<>();
        avlTree.add(615);
        avlTree.add(836);
        avlTree.add(510);
        avlTree.add(258);
        avlTree.add(699);
        avlTree.add(116);
        avlTree.add(286);
        avlTree.add(343);
        avlTree.add(178);
        avlTree.add(772);
        avlTree.add(713);
        avlTree.add(287);
        avlTree.add(548);
        avlTree.add(932);
        System.out.println(avlTree);
        System.out.println("Result: " + TreeType.values()[TypeChecker.checkType(avlTree)] + "\n");

        System.out.println("Trying With A Binary Search Tree Which Is Neither One Of Them");
        BinarySearchTree<Integer> binarySearchTree = new BinarySearchTree<>();
        for(int i = 0; i < 10; i++){
            binarySearchTree.add(i);
        }
        System.out.println(binarySearchTree);
        System.out.println("Result: " + TreeType.values()[TypeChecker.checkType(binarySearchTree)] + "\n");

        System.out.println("Trying With A Red-Black Tree Which Is Also An AVL Tree");
        RedBlackTree<Integer> redBlackTree1 = new RedBlackTree<>();
        for(int i = 0; i < 10; i++){
            redBlackTree1.add(random.nextInt(1000));
        }
        System.out.println(redBlackTree1);
        System.out.println("Result: " + TreeType.values()[TypeChecker.checkType(redBlackTree1)] + "\n");

        System.out.println("-------------------------------------------PART 3-------------------------------------------");
        insertInitialElements();
        for(int i = 0; i < 4; i++){
            System.out.println("Average running times of adding 100 randomly generated elements to " + 10*((int)Math.pow(2, i)) + "K-sized data structures");
            System.out.printf("%-19s%s%d%s", "Binary Search Tree", ": ", insertIntoBBT(i), "ns\n");
            System.out.printf("%-19s%s%d%s", "Red-Black Tree", ": ", insertIntoRBT(i), "ns\n");
            System.out.printf("%-19s%s%d%s", "2-3 Tree", ": ", insertInto2_3T(i), "ns\n");
            System.out.printf("%-19s%s%d%s", "B-Tree", ": ", insertIntoBT(i), "ns\n");
            System.out.printf("%-19s%s%d%s", "SkipList", ": ", insertIntoSL(i), "ns\n\n");
        }
    }

    private static void insertInitialElements(){
        Set<Integer> set = new HashSet<>();
        Random random = new Random();

        for(int i = 0; i < 40; i++){
            int power = i/10;
            while (set.size() != 10000*(Math.pow(2, power))){
                set.add(random.nextInt());
            }
            binarySearchTrees[i] = new BinarySearchTree<>();
            for(Integer j : set){
                binarySearchTrees[i].add(j);
            }
            set.clear();
        }

        for(int i = 0; i < 40; i++){
            int power = i/10;
            while (set.size() != 10000*(Math.pow(2, power))){
                set.add(random.nextInt());
            }
            redBlackTrees[i] = new RedBlackTree<>();
            for(Integer j : set){
                redBlackTrees[i].add(j);
            }
            set.clear();
        }

        for(int i = 0; i < 40; i++){
            int power = i/10;
            while (set.size() != 10000*(Math.pow(2, power))){
                set.add(random.nextInt());
            }
            twoThreeTrees[i] = new TwoThreeTree<>();
            for(Integer j : set){
                twoThreeTrees[i].insert(j);
            }
            set.clear();
        }

        for(int i = 0; i < 40; i++){
            int power = i/10;
            while (set.size() != 10000*(Math.pow(2, power))){
                set.add(random.nextInt());
            }
            bTrees[i] = new BTree<>(6);
            for(Integer j : set){
                bTrees[i].insert(j);
            }
            set.clear();
        }

        for(int i = 0; i < 40; i++){
            int power = i/10;
            while (set.size() != 10000*(Math.pow(2, power))){
                set.add(random.nextInt());
            }
            skipLists[i] = new SkipList<>();
            for(Integer j : set){
                skipLists[i].add(j);
            }
            set.clear();
        }
    }

    private static long insertIntoBBT(int mode){
        long start;
        long end;
        long total = 0;
        Set<Integer> set = new HashSet<>();
        ArrayList<Integer> arrayList;
        Random random = new Random();
        BinarySearchTree<Integer> temp;

        for(int i = 10*mode; i < 10*(1 + mode); i++){
            while (set.size() != 100){
                set.add(random.nextInt());
            }
            arrayList = new ArrayList<>(set);
            temp = binarySearchTrees[i];
            start = System.nanoTime();
            for(Integer element : arrayList){
                temp.add(element);
            }
            end = System.nanoTime();
            total += (end - start);
            set.clear();
        }
        return total/10;
    }

    private static long insertIntoRBT(int mode){
        long start;
        long end;
        long total = 0;
        Set<Integer> set = new HashSet<>();
        ArrayList<Integer> arrayList;
        Random random = new Random();
        RedBlackTree<Integer> temp;

        for(int i = 10*mode; i < 10*(1 + mode); i++){
            while (set.size() != 100){
                set.add(random.nextInt());
            }
            arrayList = new ArrayList<>(set);
            temp = redBlackTrees[i];
            start = System.nanoTime();
            for(Integer element : arrayList){
                temp.add(element);
            }
            end = System.nanoTime();
            total += (end - start);
            set.clear();
        }
        return total/10;
    }

    private static long insertInto2_3T(int mode){
        long start;
        long end;
        long total = 0;
        Set<Integer> set = new HashSet<>();
        ArrayList<Integer> arrayList;
        Random random = new Random();
        TwoThreeTree<Integer> temp;

        for(int i = 10*mode; i < 10*(1 + mode); i++){
            while (set.size() != 100){
                set.add(random.nextInt());
            }
            arrayList = new ArrayList<>(set);
            temp = twoThreeTrees[i];
            start = System.nanoTime();
            for(Integer element : arrayList){
                temp.insert(element);
            }
            end = System.nanoTime();
            total += (end - start);
            set.clear();
        }
        return total/10;
    }

    private static long insertIntoBT(int mode){
        long start;
        long end;
        long total = 0;
        Set<Integer> set = new HashSet<>();
        ArrayList<Integer> arrayList;
        Random random = new Random();
        BTree<Integer> temp;

        for(int i = 10*mode; i < 10*(1 + mode); i++){
            while (set.size() != 100){
                set.add(random.nextInt());
            }
            arrayList = new ArrayList<>(set);
            temp = bTrees[i];
            start = System.nanoTime();
            for(Integer element : arrayList){
                temp.insert(element);
            }
            end = System.nanoTime();
            total += (end - start);
            set.clear();
        }
        return total/10;
    }

    private static long insertIntoSL(int mode){
        long start;
        long end;
        long total = 0;
        Set<Integer> set = new HashSet<>();
        ArrayList<Integer> arrayList;
        Random random = new Random();
        SkipList<Integer> temp;

        for(int i = 10*mode; i < 10*(1 + mode); i++){
            while (set.size() != 100){
                set.add(random.nextInt());
            }
            arrayList = new ArrayList<>(set);
            temp = skipLists[i];
            start = System.nanoTime();
            for(Integer element : arrayList){
                temp.add(element);
            }
            end = System.nanoTime();
            total += (end - start);
            set.clear();
        }
        return total/10;
    }
}
