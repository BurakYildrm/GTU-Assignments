import java.util.*;

/**
 * Tests the implemented classes.
 */
public class driver {
    /**
     * Main function.
     * @param args the command line parameters
     */
    public static void main(String[] args) {
        part1();
        part2();
    }

    /**
     * Tests the class implemented in part1.
     */
    public static void part1(){
        Heap<Integer> heap1 = new Heap<>();
        heap1.add(100);
        heap1.add(40);
        heap1.add(23);
        heap1.add(78);
        heap1.add(12);
        heap1.add(1);
        heap1.add(59);
        heap1.add(150);
        Heap<Integer> heap2 = new Heap<>();
        heap2.add(5);
        heap2.add(9);
        heap2.add(37);
        heap2.add(94);
        heap2.add(72);
        System.out.println("PART1\n\nadded numbers: 100, 40, 23, 78, 12, 1, 59, 150");
        System.out.println("\nprinting heap\n" + heap1);
        System.out.println("\nprinting heap as tree structure\n" + heap1.treeStructure());
        System.out.println("\nsearching an existing element(100)");
        System.out.println("result: " + heap1.find(100));
        System.out.println("\nsearching a non-existing element(3)");
        System.out.printf("result: ");
        try {
            heap1.find(3);
        }
        catch (NoSuchElementException e){
            System.out.println(e);
        }
        System.out.println("\nheap2: " + heap2);
        System.out.println("heap2 as tree structure\n" + heap2.treeStructure());
        System.out.println("merging heap with heap2");
        System.out.println("heap before merge: " + heap1 + "\nas tree structure\n" + heap1.treeStructure());
        heap1.merge(heap2);
        System.out.println("heap after merge: " + heap1 + "\nas tree structure\n" + heap1.treeStructure());
        System.out.println("\nremoving 2nd largest element");
        System.out.println("before removal: " + heap1 + "\nas tree structure\n" + heap1.treeStructure());
        System.out.println("removed element: " + heap1.removeSpecifiedLargestElement(2));
        System.out.println("after removal: " + heap1 + "\nas tree structure\n" + heap1.treeStructure());
        System.out.println("\nsetting 3rd element to 200");
        System.out.println("before setting: " + heap1 + "\nas tree structure\n" + heap1.treeStructure());
        ListIterator<Integer> iterator = heap1.heapIterator();
        for(int i = 0; i < 3; i++){
            iterator.next();
        }
        iterator.set(200);
        System.out.println("after setting: " + heap1 + "\nas tree structure\n" + heap1.treeStructure());
    }

    /**
     * Tests the classes implemented in part2.
     */
    public static void part2(){
        ArrayList<Integer> arrayList = new ArrayList<>();
        BSTHeapTree<Integer> bstHeapTree = new BSTHeapTree<>();
        Random random = new Random();
        int temp;
        for(int i = 0; i < 3000; i++){
            temp = random.nextInt(5000);
            arrayList.add(temp);
            bstHeapTree.add(temp);
        }
        Collections.sort(arrayList);

        System.out.println("\nPART2\nfinding occurrences of existing elements\n");
        int count = 1;
        for(int i = 0, j = 0; i < 100; i++, j++){
            System.out.println("element to find occurrence: " + arrayList.get(j));
            System.out.println("tree result: " + bstHeapTree.find(arrayList.get(j)));
            while (arrayList.get(j).equals(arrayList.get(j + 1))){
                count++;
                j++;
            }
            System.out.println("arraylist result: " + count + "\n");
            count = 1;
        }

        System.out.println("\nfinding occurrences of non-existing elements\n");
        for(int i = 0; i < 10; i++){
            System.out.println("element to find occurrence: " + (i + 5001));
            try {
                bstHeapTree.find(i + 5001);
            }
            catch (NoSuchElementException e){
                System.out.println("tree result: " + e);
            }
            System.out.println("arraylist result: " + arrayList.indexOf(i + 5001) + "\n");
        }

        System.out.println("\nfinding mode\n");
        System.out.println("tree result: " + bstHeapTree.find_mode());
        int mode = arrayList.get(0);
        int index = 0;
        while (index < arrayList.size()){
            temp = occurrences(arrayList, arrayList.get(index));
            if(temp > count){
                count = temp;
                mode = arrayList.get(index);
            }
            index += temp;
        }
        System.out.println("arraylist result: " + mode);

        System.out.println("\nremoving existing elements\n");
        for(int i = 0; i < 100; i++){
            temp = arrayList.get(0);
            System.out.println("element to remove: " + temp);
            System.out.println("occurrence before removal");
            System.out.println("tree result: " + bstHeapTree.find(temp));
            System.out.println("arraylist result: " + occurrences(arrayList, temp));
            System.out.println("occurrence after removal");
            System.out.println("tree result: " + bstHeapTree.remove(arrayList.remove(0)));
            System.out.println("arraylist result: " + occurrences(arrayList, temp) + "\n");
        }

        System.out.println("\nremoving non-existing elements");
        for(int i = 0; i < 10; i++){
            System.out.println("element to remove: " + (i + 5001));
            try {
                bstHeapTree.remove((i + 5001));
            } catch (NoSuchElementException e) {
                System.out.println("tree result: " + e);
            }
            System.out.println("arraylist result: " + arrayList.remove((Object) (i + 5001)) + "\n");
        }
    }

    /**
     * Finds the occurrence of the given element int the given arraylist.
     * @param arrayList arraylist to find in
     * @param target element to find the occurrence
     * @return the occurrence of the given element
     */
    public static int occurrences(ArrayList<Integer> arrayList, Integer target){
        int startIndex = arrayList.indexOf(target);
        if(startIndex == -1){
            return 0;
        }
        int count = 0;
        while (startIndex < arrayList.size() && arrayList.get(startIndex).equals(target)){
            count++;
            startIndex++;
        }
        return count;
    }
}
