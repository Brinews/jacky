package net.datastructures;
import java.util.Comparator;
import java.util.Iterator;

/**
 * Main tester
 * @author coder
 */
public class Main {

    /**
     * @param args input arguments
     */
    public static void main(String[] args) {
        AVLTree<Integer, String> avl = new AVLTree<Integer, String>();

        avl.insert(10, "Name");
        avl.insert(18, "ID");
        avl.insert(10, "Same");
        avl.insert(28, "AGE");
        avl.insert(10, "Game");
        avl.insert(38, "USE");
        avl.insert(10, "Dame");

        System.out.println(avl.find(10).getValue());

        Iterable<Entry<Integer, String>> itr = avl.findAll(10);
        Iterator iter = itr.iterator();

        while (iter.hasNext()) {
            Entry<Integer, String> e = (Entry<Integer, String>) iter.next();

            System.out.println(e.getValue());
        }

        AVLTree<Integer, String> a1 = new AVLTree<Integer, String>();
        a1.insert(1, "one");
        a1.insert(2, "two");
        AVLTree<Integer, String> a2 = new AVLTree<Integer, String>();
        a2.insert(3, "three");
        a2.insert(5, "five");
        a2.insert(5, "five");
        a2.insert(4, "five");
        a2.insert(3, "five");
        a2.insert(9, "five");
        a2.insert(0, "five");
        a2.insert(5, "five");
        a2.insert(8, "five");
        a2.insert(15, "five");
        a2.insert(25, "five");

        AVLTree<Integer, String> a3 = 
            ExtendedAVLTree.merge(a1, a2);

        Iterable<Entry<Integer, String>> itr1 = a3.entries();
        Iterator iter1 = itr1.iterator();

        while (iter1.hasNext()) {
            Entry<Integer, String> e = (Entry<Integer, String>) iter1.next();

            System.out.println(e.getValue());
        }

        ExtendedAVLTree.print(a3);

        AVLTree a4 = ExtendedAVLTree.clone(a3);
        ExtendedAVLTree.print(a4);
    }
}
