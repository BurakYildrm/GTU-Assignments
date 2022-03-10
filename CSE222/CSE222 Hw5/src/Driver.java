import java.util.*;

public class Driver {
    public static void main(String[] args) {
        HashMapWithIterator<Integer, String> hashMapWithIterator = new HashMapWithIterator<>();
        HashtableLinkedList<Integer, String> hashtableLinkedList = new HashtableLinkedList<>();
        HashtableTreeSet<Integer, String> hashtableTreeSet = new HashtableTreeSet<>();
        HashtableCoalesced<Integer, String> hashtableCoalesced = new HashtableCoalesced<>();
        ArrayList<Integer> randomlyGeneratedNumbers = new ArrayList<>();
        HashMapIterator<Integer> mapIterator;
        Random random = new Random();
        StringBuilder value = new StringBuilder();
        long time1;
        long time2;

        System.out.println("-----------PART1-----------");
        hashMapWithIterator.put(11, "a");
        hashMapWithIterator.put(22, "b");
        hashMapWithIterator.put(33, "c");
        hashMapWithIterator.put(44, "d");
        hashMapWithIterator.put(55, "e");
        mapIterator = hashMapWithIterator.iterator();
        System.out.println("input: {11, 22, 33, 44, 55}");
        System.out.println("\n---zero parameter constructor---");
        System.out.println("iterating with next() method");
        while (mapIterator.hasNext()){
            System.out.print(mapIterator.next() + " ");
        }
        System.out.println("\n\niterating with prev() method");
        mapIterator = hashMapWithIterator.iterator();
        while (mapIterator.hasNext()){
            System.out.print(mapIterator.prev() + " ");
        }
        System.out.println("\n\n---parameterized constructor---");
        System.out.println("start key : 55");
        System.out.println("iterating with next() method");
        mapIterator = hashMapWithIterator.iterator(55);
        while (mapIterator.hasNext()){
            System.out.print(mapIterator.next() + " ");
        }
        System.out.println("\niterating with prev() method");
        mapIterator = hashMapWithIterator.iterator(55);
        while (mapIterator.hasNext()){
            System.out.print(mapIterator.prev() + " ");
        }
        System.out.println("\n\nstart key : 77 (non-existing key)");
        System.out.println("iterating with next() method");
        mapIterator = hashMapWithIterator.iterator(77);
        while (mapIterator.hasNext()){
            System.out.print(mapIterator.next() + " ");
        }
        System.out.println("\niterating with prev() method");
        mapIterator = hashMapWithIterator.iterator(77);
        while (mapIterator.hasNext()){
            System.out.print(mapIterator.prev() + " ");
        }
        System.out.println("\n\n-----------PART2-----------");
        System.out.println("-----small-sized data-----");
        System.out.println("adding(upper bound is 5000)");
        System.out.println("number of data added: 10");
        for(int i = 0; i < 10; i++){
            randomlyGeneratedNumbers.add(random.nextInt(5000));
        }

        time1 = System.nanoTime();
        for(Integer i : randomlyGeneratedNumbers){
            hashtableLinkedList.put(i, String.valueOf(i));
        }
        time2 = System.nanoTime();
        System.out.println("HashtableLinkedList performance: " + (time2 - time1) + " nanoseconds");

        time1 = System.nanoTime();
        for(Integer i : randomlyGeneratedNumbers){
            hashtableTreeSet.put(i, String.valueOf(i));
        }
        time2 = System.nanoTime();
        System.out.println("HashtableTreeSet performance: " + (time2 - time1) + " nanoseconds");

        time1 = System.nanoTime();
        for(Integer i : randomlyGeneratedNumbers){
            hashtableCoalesced.put(i, String.valueOf(i));
        }
        time2 = System.nanoTime();
        System.out.println("HashtableCoalesced performance: " + (time2 - time1) + " nanoseconds");

        System.out.println("\naccessing existing elements(first 5 elements)");
        time1 = System.nanoTime();
        for(int i = 0; i < 5; i++){
            value.append(hashtableLinkedList.get(randomlyGeneratedNumbers.get(i))).append(" ");
        }
        time2 = System.nanoTime();
        System.out.println("HashtableLinkedList\naccessed elements: " + value + "\nperformance: " + (time2 - time1) + " nanoseconds\n");

        value = new StringBuilder();
        time1 = System.nanoTime();
        for(int i = 0; i < 5; i++){
            value.append(hashtableTreeSet.get(randomlyGeneratedNumbers.get(i))).append(" ");
        }
        time2 = System.nanoTime();
        System.out.println("HashtableTreeSet\naccessed elements: " + value + "\nperformance: " + (time2 - time1) + " nanoseconds\n");

        value = new StringBuilder();
        time1 = System.nanoTime();
        for(int i = 0; i < 5; i++){
            value.append(hashtableCoalesced.get(randomlyGeneratedNumbers.get(i))).append(" ");
        }
        time2 = System.nanoTime();
        System.out.println("HashtableCoalesced\naccessed elements: " + value + "\nperformance: " + (time2 - time1) + " nanoseconds");

        System.out.println("\naccessing non-existing elements(5001 to 5006)");
        value = new StringBuilder();
        time1 = System.nanoTime();
        for(int i = 5001; i < 5006; i++){
            value.append(hashtableLinkedList.get(i)).append(" ");
        }
        time2 = System.nanoTime();
        System.out.println("HashtableLinkedList\naccessed elements: " + value + "\nperformance: " + (time2 - time1) + " nanoseconds\n");

        value = new StringBuilder();
        time1 = System.nanoTime();
        for(int i = 5001; i < 5006; i++){
            value.append(hashtableTreeSet.get(i)).append(" ");
        }
        time2 = System.nanoTime();
        System.out.println("HashtableTreeSet\naccessed elements: " + value + "\nperformance: " + (time2 - time1) + " nanoseconds\n");

        value = new StringBuilder();
        time1 = System.nanoTime();
        for(int i = 5001; i < 5006; i++){
            value.append(hashtableCoalesced.get(i)).append(" ");
        }
        time2 = System.nanoTime();
        System.out.println("HashtableCoalesced\naccessed elements: " + value + "\nperformance: " + (time2 - time1) + " nanoseconds");

        System.out.println("\nremoving an existing element(first 5 elements)");
        value = new StringBuilder();
        time1 = System.nanoTime();
        for(int i = 0; i < 5; i++){
            value.append(hashtableLinkedList.remove(randomlyGeneratedNumbers.get(i))).append(" ");
        }
        time2 = System.nanoTime();
        System.out.println("HashtableLinkedList\nremoved elements: " + value + "\nperformance: " + (time2 - time1) + " nanoseconds\n");

        value = new StringBuilder();
        time1 = System.nanoTime();
        for(int i = 0; i < 5; i++){
            value.append(hashtableTreeSet.remove(randomlyGeneratedNumbers.get(i))).append(" ");
        }
        time2 = System.nanoTime();
        System.out.println("HashtableTreeSet\nremoved elements: " + value + "\nperformance: " + (time2 - time1) + " nanoseconds\n");

        value = new StringBuilder();
        time1 = System.nanoTime();
        for(int i = 0; i < 5; i++){
            value.append(hashtableCoalesced.remove(randomlyGeneratedNumbers.get(i))).append(" ");
        }
        time2 = System.nanoTime();
        System.out.println("HashtableCoalesced\nremoved elements: " + value + "\nperformance: " + (time2 - time1) + " nanoseconds");

        System.out.println("\nremoving non-existing elements(5001 to 5006)");
        value = new StringBuilder();
        time1 = System.nanoTime();
        for(int i = 5001; i < 5006; i++){
            value.append(hashtableLinkedList.remove(i)).append(" ");
        }
        time2 = System.nanoTime();
        System.out.println("HashtableLinkedList\nremoved elements: " + value + "\nperformance: " + (time2 - time1) + " nanoseconds\n");

        value = new StringBuilder();
        time1 = System.nanoTime();
        for(int i = 5001; i < 5006; i++){
            value.append(hashtableTreeSet.remove(i)).append(" ");
        }
        time2 = System.nanoTime();
        System.out.println("HashtableTreeSet\nremoved elements: " + value + "\nperformance: " + (time2 - time1) + " nanoseconds\n");

        value = new StringBuilder();
        time1 = System.nanoTime();
        for(int i = 5001; i < 5006; i++){
            value.append(hashtableCoalesced.remove(i)).append(" ");
        }
        time2 = System.nanoTime();
        System.out.println("HashtableCoalesced\nremoved elements: " + value + "\nperformance: " + (time2 - time1) + " nanoseconds");

        System.out.println("-----medium-sized data-----");
        hashtableLinkedList = new HashtableLinkedList<>();
        hashtableTreeSet = new HashtableTreeSet<>();
        hashtableCoalesced = new HashtableCoalesced<>();
        randomlyGeneratedNumbers.clear();
        System.out.println("adding(upper bound is 5000)");
        System.out.println("number of data added: 100");
        for(int i = 0; i < 100; i++){
            randomlyGeneratedNumbers.add(random.nextInt(5000));
        }

        time1 = System.nanoTime();
        for(Integer i : randomlyGeneratedNumbers){
            hashtableLinkedList.put(i, String.valueOf(i));
        }
        time2 = System.nanoTime();
        System.out.println("HashtableLinkedList performance: " + (time2 - time1) + " nanoseconds");

        time1 = System.nanoTime();
        for(Integer i : randomlyGeneratedNumbers){
            hashtableTreeSet.put(i, String.valueOf(i));
        }
        time2 = System.nanoTime();
        System.out.println("HashtableTreeSet performance: " + (time2 - time1) + " nanoseconds");

        time1 = System.nanoTime();
        for(Integer i : randomlyGeneratedNumbers){
            hashtableCoalesced.put(i, String.valueOf(i));
        }
        time2 = System.nanoTime();
        System.out.println("HashtableCoalesced performance: " + (time2 - time1) + " nanoseconds");

        System.out.println("\naccessing existing elements(first 50 elements)");
        time1 = System.nanoTime();
        for(int i = 0; i < 50; i++){
            hashtableLinkedList.get(randomlyGeneratedNumbers.get(i));
        }
        time2 = System.nanoTime();
        System.out.println("HashtableLinkedList performance: " + (time2 - time1) + " nanoseconds\n");

        time1 = System.nanoTime();
        for(int i = 0; i < 50; i++){
            hashtableTreeSet.get(randomlyGeneratedNumbers.get(i));
        }
        time2 = System.nanoTime();
        System.out.println("HashtableTreeSet performance: " + (time2 - time1) + " nanoseconds\n");

        time1 = System.nanoTime();
        for(int i = 0; i < 50; i++){
            hashtableCoalesced.get(randomlyGeneratedNumbers.get(i));
        }
        time2 = System.nanoTime();
        System.out.println("HashtableCoalesced performance: " + (time2 - time1) + " nanoseconds");

        System.out.println("\naccessing non-existing elements(5001 to 5051)");
        time1 = System.nanoTime();
        for(int i = 5001; i < 5051; i++){
            hashtableLinkedList.get(i);
        }
        time2 = System.nanoTime();
        System.out.println("HashtableLinkedList performance: " + (time2 - time1) + " nanoseconds\n");

        time1 = System.nanoTime();
        for(int i = 5001; i < 5051; i++){
            hashtableTreeSet.get(i);
        }
        time2 = System.nanoTime();
        System.out.println("HashtableTreeSet performance: " + (time2 - time1) + " nanoseconds\n");

        time1 = System.nanoTime();
        for(int i = 5001; i < 5051; i++){
            hashtableCoalesced.get(i);
        }
        time2 = System.nanoTime();
        System.out.println("HashtableCoalesced performance: " + (time2 - time1) + " nanoseconds");

        System.out.println("\nremoving existing elements(first 50 elements)");
        time1 = System.nanoTime();
        for(int i = 0; i < 50; i++){
            hashtableLinkedList.remove(randomlyGeneratedNumbers.get(i));
        }
        time2 = System.nanoTime();
        System.out.println("HashtableLinkedList performance: " + (time2 - time1) + " nanoseconds\n");

        time1 = System.nanoTime();
        for(int i = 0; i < 50; i++){
            hashtableTreeSet.remove(randomlyGeneratedNumbers.get(i));
        }
        time2 = System.nanoTime();
        System.out.println("HashtableTreeSet performance: " + (time2 - time1) + " nanoseconds\n");

        time1 = System.nanoTime();
        for(int i = 0; i < 50; i++){
            hashtableCoalesced.remove(randomlyGeneratedNumbers.get(i));
        }
        time2 = System.nanoTime();
        System.out.println("HashtableCoalesced performance: " + (time2 - time1) + " nanoseconds");

        System.out.println("\nremoving non-existing elements(5001 to 5051)");
        time1 = System.nanoTime();
        for(int i = 5001; i < 5051; i++){
            hashtableLinkedList.remove(i);
        }
        time2 = System.nanoTime();
        System.out.println("HashtableLinkedList performance: " + (time2 - time1) + " nanoseconds\n");

        time1 = System.nanoTime();
        for(int i = 5001; i < 5051; i++){
            hashtableTreeSet.remove(i);
        }
        time2 = System.nanoTime();
        System.out.println("HashtableTreeSet performance: " + (time2 - time1) + " nanoseconds\n");

        time1 = System.nanoTime();
        for(int i = 5001; i < 5051; i++){
            hashtableCoalesced.remove(i);
        }
        time2 = System.nanoTime();
        System.out.println("HashtableCoalesced performance: " + (time2 - time1) + " nanoseconds");

        System.out.println("-----large-sized data-----");
        hashtableLinkedList = new HashtableLinkedList<>();
        hashtableTreeSet = new HashtableTreeSet<>();
        hashtableCoalesced = new HashtableCoalesced<>();
        randomlyGeneratedNumbers.clear();
        System.out.println("adding(upper bound is 5000)");
        System.out.println("number of data added: 1000");
        for(int i = 0; i < 1000; i++){
            randomlyGeneratedNumbers.add(random.nextInt(5000));
        }

        time1 = System.nanoTime();
        for(Integer i : randomlyGeneratedNumbers){
            hashtableLinkedList.put(i, String.valueOf(i));
        }
        time2 = System.nanoTime();
        System.out.println("HashtableLinkedList performance: " + (time2 - time1) + " nanoseconds");

        time1 = System.nanoTime();
        for(Integer i : randomlyGeneratedNumbers){
            hashtableTreeSet.put(i, String.valueOf(i));
        }
        time2 = System.nanoTime();
        System.out.println("HashtableTreeSet performance: " + (time2 - time1) + " nanoseconds");

        time1 = System.nanoTime();
        for(Integer i : randomlyGeneratedNumbers){
            hashtableCoalesced.put(i, String.valueOf(i));
        }
        time2 = System.nanoTime();
        System.out.println("HashtableCoalesced performance: " + (time2 - time1) + " nanoseconds");

        System.out.println("\naccessing existing elements(first 500 elements)");
        time1 = System.nanoTime();
        for(int i = 0; i < 500; i++){
            hashtableLinkedList.get(randomlyGeneratedNumbers.get(i));
        }
        time2 = System.nanoTime();
        System.out.println("HashtableLinkedList performance: " + (time2 - time1) + " nanoseconds\n");

        time1 = System.nanoTime();
        for(int i = 0; i < 500; i++){
            hashtableTreeSet.get(randomlyGeneratedNumbers.get(i));
        }
        time2 = System.nanoTime();
        System.out.println("HashtableTreeSet performance: " + (time2 - time1) + " nanoseconds\n");

        time1 = System.nanoTime();
        for(int i = 0; i < 500; i++){
            hashtableCoalesced.get(randomlyGeneratedNumbers.get(i));
        }
        time2 = System.nanoTime();
        System.out.println("HashtableCoalesced performance: " + (time2 - time1) + " nanoseconds");

        System.out.println("\naccessing non-existing elements(5001 to 5501)");
        time1 = System.nanoTime();
        for(int i = 5001; i < 5501; i++){
            hashtableLinkedList.get(i);
        }
        time2 = System.nanoTime();
        System.out.println("HashtableLinkedList performance: " + (time2 - time1) + " nanoseconds\n");

        time1 = System.nanoTime();
        for(int i = 5001; i < 5501; i++){
            hashtableTreeSet.get(i);
        }
        time2 = System.nanoTime();
        System.out.println("HashtableTreeSet performance: " + (time2 - time1) + " nanoseconds\n");

        time1 = System.nanoTime();
        for(int i = 5001; i < 5501; i++){
            hashtableCoalesced.get(i);
        }
        time2 = System.nanoTime();
        System.out.println("HashtableCoalesced performance: " + (time2 - time1) + " nanoseconds");

        System.out.println("\nremoving existing elements(first 500 elements)");
        time1 = System.nanoTime();
        for(int i = 0; i < 500; i++){
            hashtableLinkedList.remove(randomlyGeneratedNumbers.get(i));
        }
        time2 = System.nanoTime();
        System.out.println("HashtableLinkedList performance: " + (time2 - time1) + " nanoseconds\n");

        time1 = System.nanoTime();
        for(int i = 0; i < 500; i++){
            hashtableTreeSet.remove(randomlyGeneratedNumbers.get(i));
        }
        time2 = System.nanoTime();
        System.out.println("HashtableTreeSet performance: " + (time2 - time1) + " nanoseconds\n");

        time1 = System.nanoTime();
        for(int i = 0; i < 500; i++){
            hashtableCoalesced.remove(randomlyGeneratedNumbers.get(i));
        }
        time2 = System.nanoTime();
        System.out.println("HashtableCoalesced performance: " + (time2 - time1) + " nanoseconds");

        System.out.println("\nremoving non-existing elements(5001 to 5501)");
        time1 = System.nanoTime();
        for(int i = 5001; i < 5501; i++){
            hashtableLinkedList.remove(i);
        }
        time2 = System.nanoTime();
        System.out.println("HashtableLinkedList performance: " + (time2 - time1) + " nanoseconds\n");

        time1 = System.nanoTime();
        for(int i = 5001; i < 5501; i++){
            hashtableTreeSet.remove(i);
        }
        time2 = System.nanoTime();
        System.out.println("HashtableTreeSet performance: " + (time2 - time1) + " nanoseconds\n");

        time1 = System.nanoTime();
        for(int i = 5001; i < 5501; i++){
            hashtableCoalesced.remove(i);
        }
        time2 = System.nanoTime();
        System.out.println("HashtableCoalesced performance: " + (time2 - time1) + " nanoseconds");
    }
}
