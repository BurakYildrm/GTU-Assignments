import java.util.Scanner;

public class test {
    public static void main(String[] args) {
        int choice;
        Scanner sc = new Scanner(System.in);
        System.out.println("Please choose one of the options below");
        System.out.println("1)ArrayList(Ingeter)\n2)ArrayList(String)\n3)LinkedList(Integer)\n4)LinkedList(String)\n5)HashSet(Integer)\n6)HashSet(String)");
        choice = sc.nextInt();
        if(choice == 1){
            ArrayListTestInteger();
        }
        if(choice == 2){
            ArrayListTestString();
        }
        if(choice == 3){
            LinkedListTestInteger();
        }
        if(choice == 4){
            LinkedListTestString();
        }
        if(choice == 5){
            HashSetTestInteger();
        }
        if(choice == 6){
            HashSetTestString();
        }
        sc.close();
    }
    public static void ArrayListTestInteger(){
        ArrayList<Integer> var = new ArrayList<>();
        ArrayList<Integer> var2 = new ArrayList<>();
        var.add(1);
        var.add(2);
        var.add(3);
        var2.add(4);
        var2.add(5);
        var2.add(6);
        Iterator<Integer> iterator = var.iterator();
        System.out.println("ArrayList original:");
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("ArrayList var after addAll var2{4,5,6}");
        var.addAll(var2);
        iterator = var.iterator();
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("ArrayList var contains 1: " + var.contains(1));
        System.out.println("ArrayList var contains 7: " + var.contains(7));
        System.out.println("ArrayList var containsAll var2{4,5,6}: " + var.containsAll(var2));
        var2.set(1, 7);
        System.out.println("ArrayList var containsAll var2{4,7,6}: " + var.containsAll(var2));
        System.out.println("ArrayList var2 isEmpty: " + var2.isEmpty());
        var2.clear();
        System.out.println("ArrayList var2 isEmpty after clear: " + var2.isEmpty());
        System.out.println("ArrayList var get 1: " + var.get(1));
        System.out.println("ArrayList var after remove 4");
        var.remove((Integer)4);
        iterator = var.iterator();
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("ArrayList var after remove index 2");
        var.remove(2);
        iterator = var.iterator();
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("ArrayList var after removeAll var2{1,5}");
        var2.add(1);
        var2.add(5);
        var.removeAll(var2);
        iterator = var.iterator();
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("ArrayList var after retainAll var2{1,5}");
        var.retainAll(var2);
        iterator = var.iterator();
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("ArrayList var after set(1, 3)");
        var.set(1, 3);
        iterator = var.iterator();
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("ArrayList var size: " + var.size());
        try{
            iterator = var.iterator();
            iterator.next();
            iterator.remove();
            System.out.println("ArrayList var after iterator remove at index 0");
            iterator = var.iterator();
            while(iterator.hasNext()) System.out.println(iterator.next());
        }
        catch(DoesNotWorkForQueues exep){
            System.out.println(exep);
        }
    }
    public static void ArrayListTestString(){
        ArrayList<String> var = new ArrayList<>();
        ArrayList<String> var2 = new ArrayList<>();
        var.add("abc");
        var.add("def");
        var.add("ghi");
        var2.add("jkl");
        var2.add("mno");
        var2.add("prs");
        Iterator<String> iterator = var.iterator();
        System.out.println("ArrayList original:");
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("ArrayList var after addAll var2{jkl,mno,prs}");
        var.addAll(var2);
        iterator = var.iterator();
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("ArrayList var contains abc: " + var.contains("abc"));
        System.out.println("ArrayList var contains xyz: " + var.contains("xyz"));
        System.out.println("ArrayList var containsAll var2{jkl,mno,prs}: " + var.containsAll(var2));
        var2.set(1, "xyz");
        System.out.println("ArrayList var containsAll var2{jkl,xyz,prs}: " + var.containsAll(var2));
        System.out.println("ArrayList var2 isEmpty: " + var2.isEmpty());
        var2.clear();
        System.out.println("ArrayList var2 isEmpty after clear: " + var2.isEmpty());
        System.out.println("ArrayList var get 1: " + var.get(1));
        System.out.println("ArrayList var after remove abc");
        var.remove("abc");
        iterator = var.iterator();
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("ArrayList var after remove index 2");
        var.remove(2);
        iterator = var.iterator();
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("ArrayList var after removeAll var2{ghi,jkl}");
        var2.add("ghi");
        var2.add("jkl");
        var.removeAll(var2);
        iterator = var.iterator();
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("ArrayList var after retainAll var2{ghi,jkl}");
        var.retainAll(var2);
        iterator = var.iterator();
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("ArrayList var after set(1, dhf)");
        var.set(1, "dhf");
        iterator = var.iterator();
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("ArrayList var size: " + var.size());
        try{
            iterator = var.iterator();
            iterator.next();
            iterator.remove();
            System.out.println("ArrayList var after iterator remove at index 0");
            iterator = var.iterator();
            while(iterator.hasNext()) System.out.println(iterator.next());
        }
        catch(DoesNotWorkForQueues exep){
            System.out.println(exep);
        }
    }
    public static void LinkedListTestInteger(){
        LinkedList<Integer> var = new LinkedList<>();
        LinkedList<Integer> var2 = new LinkedList<>();
        var.add(1);
        var.add(2);
        var.add(3);
        var2.add(4);
        var2.add(5);
        var2.add(6);
        Iterator<Integer> iterator = var.iterator();
        System.out.println("LinkedList original:");
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("LinkedList var after addAll var2{4,5,6}");
        var.addAll(var2);
        iterator = var.iterator();
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("LinkedList var contains 1: " + var.contains(1));
        System.out.println("LinkedList var contains 7: " + var.contains(7));
        System.out.println("LinkedList var containsAll var2{4,5,6}: " + var.containsAll(var2));
        var2.set(1, 7);
        System.out.println("LinkedList var containsAll var2{4,7,6}: " + var.containsAll(var2));
        System.out.println("LinkedList var2 isEmpty: " + var2.isEmpty());
        var2.clear();
        System.out.println("LinkedList var2 isEmpty after clear: " + var2.isEmpty());
        System.out.println("LinkedList var get 1: " + var.get(1));
        System.out.println("LinkedList var after remove 4");
        var.remove((Integer)4);
        iterator = var.iterator();
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("LinkedList var after remove index 2");
        var.remove(2);
        iterator = var.iterator();
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("LinkedList var after removeAll var2{1,5}");
        var2.add(1);
        var2.add(5);
        var.removeAll(var2);
        iterator = var.iterator();
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("LinkedList var after retainAll var2{1,5}");
        var.retainAll(var2);
        iterator = var.iterator();
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("LinkedList var after set(1, 3)");
        var.set(1, 3);
        iterator = var.iterator();
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("LinkedList var size: " + var.size());
        try{
            System.out.println("LinkedList var element: " + var.element());
            System.out.println("LinkedList var poll: " + var.poll());
            System.out.println("LinkedList var after poll");
            iterator = var.iterator();
            while(iterator.hasNext()) System.out.println(iterator.next());
            System.out.println("LinkedList var after offer 10");
            var.offer(10);
            iterator = var.iterator();
            while(iterator.hasNext()) System.out.println(iterator.next());
        }
        catch(EmptyListException exep){
            System.out.println(exep);
        }
        try{
            System.out.println("LinkedList var after iterator remove at index 0");
            iterator = var.iterator();
            iterator.next();
            iterator.remove();
            iterator = var.iterator();
            while(iterator.hasNext()) System.out.println(iterator.next());
        }
        catch(DoesNotWorkForQueues exep){
            System.out.println(exep);
        }
    }
    public static void LinkedListTestString(){
        LinkedList<String> var = new LinkedList<>();
        LinkedList<String> var2 = new LinkedList<>();
        var.add("abc");
        var.add("def");
        var.add("ghi");
        var2.add("jkl");
        var2.add("mno");
        var2.add("prs");
        Iterator<String> iterator = var.iterator();
        System.out.println("LinkedList original:");
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("LinkedList var after addAll var2{jkl,mno,prs}");
        var.addAll(var2);
        iterator = var.iterator();
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("LinkedList var contains abc: " + var.contains("abc"));
        System.out.println("LinkedList var contains xyz: " + var.contains("xyz"));
        System.out.println("LinkedList var containsAll var2{jkl,mno,prs}: " + var.containsAll(var2));
        var2.set(1, "xyz");
        System.out.println("LinkedList var containsAll var2{jkl,xyz,prs}: " + var.containsAll(var2));
        System.out.println("LinkedList var2 isEmpty: " + var2.isEmpty());
        var2.clear();
        System.out.println("LinkedList var2 isEmpty after clear: " + var2.isEmpty());
        System.out.println("LinkedList var get 1: " + var.get(1));
        System.out.println("LinkedList var after remove abc");
        var.remove("abc");
        iterator = var.iterator();
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("LinkedList var after remove index 2");
        var.remove(2);
        iterator = var.iterator();
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("LinkedList var after removeAll var2{ghi,jkl}");
        var2.add("ghi");
        var2.add("jkl");
        var.removeAll(var2);
        iterator = var.iterator();
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("LinkedList var after retainAll var2{ghi,jkl}");
        var.retainAll(var2);
        iterator = var.iterator();
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("LinkedList var after set(1, dhf)");
        var.set(1, "dhf");
        iterator = var.iterator();
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("LinkedList var size: " + var.size());
        try{
            System.out.println("LinkedList var element: " + var.element());
            System.out.println("LinkedList var poll: " + var.poll());
            System.out.println("LinkedList var after poll");
            iterator = var.iterator();
            while(iterator.hasNext()) System.out.println(iterator.next());
            System.out.println("LinkedList var after offer asd");
            var.offer("asd");
            iterator = var.iterator();
            while(iterator.hasNext()) System.out.println(iterator.next());
        }
        catch(EmptyListException exep){
            System.out.println(exep);
        }
        try{
            System.out.println("LinkedList var after iterator remove at index 0");
            iterator = var.iterator();
            iterator.next();
            iterator.remove();
            iterator = var.iterator();
            while(iterator.hasNext()) System.out.println(iterator.next());
        }
        catch(DoesNotWorkForQueues exep){
            System.out.println(exep);
        }
    }
    public static void HashSetTestInteger(){
        HashSet<Integer> var = new HashSet<>();
        HashSet<Integer> var2 = new HashSet<>();
        var.add(1);
        var.add(2);
        var.add(3);
        var2.add(4);
        var2.add(5);
        var2.add(6);
        Iterator<Integer> iterator = var.iterator();
        System.out.println("HashSet original:");
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("HashSet var after addAll var2{4,5,6}");
        var.addAll(var2);
        iterator = var.iterator();
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("HashSet var contains 1: " + var.contains(1));
        System.out.println("HashSet var contains 7: " + var.contains(7));
        System.out.println("HashSet var containsAll var2{4,5,6}: " + var.containsAll(var2));
        var2.add(7);
        System.out.println("HashSet var containsAll var2{4,5,6,7}: " + var.containsAll(var2));
        System.out.println("HashSet var2 isEmpty: " + var2.isEmpty());
        var2.clear();
        System.out.println("HashSet var2 isEmpty after clear: " + var2.isEmpty());
        System.out.println("HashSet var after remove 4");
        var.remove(4);
        iterator = var.iterator();
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("HashSet var after removeAll var2{1,5}");
        var2.add(1);
        var2.add(5);
        var.removeAll(var2);
        iterator = var.iterator();
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("HashSet var after retainAll var2{1,5}");
        var.retainAll(var2);
        iterator = var.iterator();
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("HashSet var size: " + var.size());
        try{
            iterator = var.iterator();
            iterator.next();
            iterator.remove();
            System.out.println("HashSet var after iterator remove at index 0");
            iterator = var.iterator();
            while(iterator.hasNext()) System.out.println(iterator.next());
        }
        catch(DoesNotWorkForQueues exep){
            System.out.println(exep);
        }
    }
    public static void HashSetTestString(){
        HashSet<String> var = new HashSet<>();
        HashSet<String> var2 = new HashSet<>();
        var.add("abc");
        var.add("def");
        var.add("ghi");
        var2.add("jkl");
        var2.add("mno");
        var2.add("prs");
        Iterator<String> iterator = var.iterator();
        System.out.println("HashSet original:");
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("HashSet var after addAll var2{jkl,mno,prs}");
        var.addAll(var2);
        iterator = var.iterator();
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("HashSet var contains abc: " + var.contains("abc"));
        System.out.println("HashSet var contains xyz: " + var.contains("xyz"));
        System.out.println("HashSet var containsAll var2{jkl,mno,prs}: " + var.containsAll(var2));
        var2.add("xyz");
        System.out.println("HashSet var containsAll var2{jkl,mno,prs,xyz}: " + var.containsAll(var2));
        System.out.println("HashSet var2 isEmpty: " + var2.isEmpty());
        var2.clear();
        System.out.println("HashSet var2 isEmpty after clear: " + var2.isEmpty());
        System.out.println("HashSet var after remove abc");
        var.remove("abc");
        iterator = var.iterator();
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("HashSet var after removeAll var2{ghi,jkl}");
        var2.add("ghi");
        var2.add("jkl");
        var.removeAll(var2);
        iterator = var.iterator();
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("HashSet var after retainAll var2{ghi,jkl}");
        var.retainAll(var2);
        iterator = var.iterator();
        while(iterator.hasNext()) System.out.println(iterator.next());
        System.out.println("HashSet var size: " + var.size());
        try{
            iterator = var.iterator();
            iterator.next();
            iterator.remove();
            System.out.println("HashSet var after iterator remove at index 0");
            iterator = var.iterator();
            while(iterator.hasNext()) System.out.println(iterator.next());
        }
        catch(DoesNotWorkForQueues exep){
            System.out.println(exep);
        }
    }
}