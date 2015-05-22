package net.datastructures;

import java.util.Comparator;
import java.util.Iterator;
import javax.swing.JFrame;
import javax.swing.JPanel;

import java.awt.Color;
import java.awt.Graphics;
import java.util.ArrayList;

/**
 * ExtendedAVLTree class -- added functions:
 * 1. clone;
 * 2. merge;
 * 3. print
 * @author coder
 */
public class ExtendedAVLTree<K, V> extends AVLTree<K, V> 
    implements Dictionary<K, V> {
    /**
     * constructor
     * @param c comparator
     */
    public ExtendedAVLTree(Comparator<K> c) { 
        super(c); 
    }

    /**
     * constructor
     */
    public ExtendedAVLTree() { 
        super(); 
    }

    /**
     * @param node AVLNode
     * @return avlnode's clone
     */
    public static<K, V> AVLNode<K,V> cloneNode(AVLNode<K,V> node) {

        if (null == node) return new AVLNode(null, null, null, null);

        AVLNode<K,V> newNode = 
            new AVLNode(node.element(), node.getParent(), 
                    cloneNode((AVLNode<K,V>)(node.getLeft())), 
                    cloneNode((AVLNode<K,V>)(node.getRight())));

        ((AVLNode<K, V>)newNode)
            .setHeight(((AVLNode<K, V>)node).getHeight());

        return newNode;
    }

    /**
     * @param tree AVLTree
     * @return avltree's clone
     */
    public static<K, V> AVLTree<K, V> clone(AVLTree<K, V> tree) {
        if (tree == null) return null;

        // node copy ok
        AVLNode<K,V> node = cloneNode((AVLNode<K,V>)(tree.root()));

        // build tree
        AVLTree newTree = new AVLTree();
        newTree.root = node;

        return newTree;
    }

    /**
     * @param tree1,tree2 ALVTree
     * @return merged tree
     * Running time analysis:
     *  1. Traverse tree1 to list, O(n)
     *  2. Traverse tree2 to list, O(m)
     *  3. Merge two list, O(M+N)
     *  4. Taverse list to tree, O(log(M+N))
     */
    public static<K, V> AVLTree<K, V> merge(AVLTree<K, V> tree1,
            AVLTree<K, V> tree2) {
        AVLTree newTree = new AVLTree();

        Iterable<Entry<K,V>> itr = tree1.entries();
        Iterator iter = itr.iterator();

        Iterable<Entry<K,V>> itr2 = tree2.entries();
        Iterator iter2 = itr2.iterator();

        PositionList<Entry<K,V>> poslist 
            = new NodePositionList<Entry<K,V>>();

        while (iter.hasNext()) {
            Entry<K,V> e = (Entry<K, V>)iter.next();
            poslist.addLast(e);
        }

        while (iter2.hasNext()) {
            Entry<K,V> e = (Entry<K, V>)iter2.next();
            poslist.addLast(e);
        }

        Iterator it = poslist.iterator();

        while (it.hasNext()) {
            Entry<K, V> e = (Entry<K, V>)it.next();
            newTree.insert(e.getKey(), e.getValue());
        }

        return newTree;
    }

    /**
     * print AVL tree
     * @param tree AVLTree
     */
    public static<K, V> void print(AVLTree<K, V> tree) {
        TreeFrame tf = new TreeFrame();

        // traverse tree and paint
        printTree(tf, (AVLNode<K,V>)tree.root(), 0, 0);
    }

    static int py = 0;

    /**
     * printTree
     * @param tf TreeFrame
     * @param node AVLNode
     * @param x x-position
     * @param y y-position
     */
    public static<K, V> void printTree(TreeFrame tf, 
            AVLNode<K, V> node, int x, int y) {
        if (node != null && node.element() != null) {

            for (int i = 0; i < x; i++) {
                System.out.print("  ");
            }
            System.out.println(node.element().getKey());

            if (node.getLeft().element() == null 
                    && node.getRight().element() == null)
                tf.doshape((String)(node.element().getKey().toString()),
                        1, x*28, y*28);
            else
                tf.doshape((String)(node.element().getKey().toString()), 
                        0, x*28, y*28);
            //y++;

            if (node.getLeft() != null 
                    && node.getLeft().element() != null) {
                tf.doline(x*28+13, y*28+13, (x+1)*28, (y+1)*28);
            }
            if (node.getRight() != null 
                    && node.getRight().element() != null) {
                tf.doline(x*28+13, y*28+13, (x+1)*28, (y+2)*28);
            }

            printTree(tf, (AVLNode<K,V>)(node.getLeft()), x+1, y+1);
            printTree(tf, (AVLNode<K,V>)(node.getRight()), x+1, y+2);
        }
    }

    static class Panel extends JPanel {
        class point {
            int type;
            String s;
            int x, y;
            int x1, y1;
            public point(String str, int a, int b, int t) {
                x = a;
                y = b;
                s = str;
                type = t;
            }

            public void setEnd(int c, int d) {
                x1 = c;
                y1 = d;
            }
        }

        ArrayList<point> ap = new ArrayList<point>();

        public Panel() {
        }

        protected void paintComponent(Graphics g) {
            for (int i = 0; i < ap.size(); i++) {
                if (ap.get(i).type == 0) {
                    g.setColor(Color.white);
                    g.fillOval(ap.get(i).x, ap.get(i).y, 26, 26);
                }
                else if (ap.get(i).type == 1) {
                    g.setColor(Color.blue);
                    g.drawRect(ap.get(i).x,ap.get(i).y, 26, 26);
                }
                else {
                    g.setColor(Color.black);
                    g.drawLine(ap.get(i).x, ap.get(i).y, ap.get(i).x1,
                            ap.get(i).y1);
                }
                g.setColor(Color.black);
                g.drawString(ap.get(i).s, ap.get(i).x+3, ap.get(i).y+16);
            }
        }

        void doPaint(int x, int y, String s, int type) {
            ap.add(new point(s, x, y, type));
            repaint();
        }

        void doLine(int x, int y, int x1, int y1) {
            point p = new point("", x, y, 3);
            p.setEnd(x1, y1);

            ap.add(p);
            repaint();
        }
    }

    static class TreeFrame extends JFrame {
        Panel p;
        public TreeFrame(){
            setSize(600, 700);
            setDefaultCloseOperation(EXIT_ON_CLOSE);
            p = new Panel();
            this.getContentPane().add(p);
            setVisible(true);
        }

        public void doshape(String key, int t, int x, int y){
            p.doPaint(x, y, key, t);
        }

        public void doline(int x, int y, int x1, int y1) {
            p.doLine(x, y, x1, y1);
        }
    }
}
