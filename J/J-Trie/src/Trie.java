/**
 * Prefix-Trie. Supports linear time find() and insert(). 
 * @author coder
 */
import java.util.ArrayList;
import java.util.Comparator;
import java.util.Iterator;
import java.util.List;
import java.util.HashMap;
import java.util.Map;
import java.util.SortedSet;
import java.util.TreeSet;

/**
 * Class for Trie
 *  a dict of character
 * @author coder
 */
public class Trie {
    /**
     * Field node of tree
     */
    private Vertex root;

    /**
     * The Dict string
     */
    private String dict = "";

    /**
     * Vertex Node:
     *  letter branch childs
     * @author coder
     */
    protected class Vertex {
        protected int words;
        protected int prefixes;
        protected double weight;
        //protected Vertex[] edges;
        protected HashMap<Character, Vertex> edges;

        /**
         * constructor: init the root node
         */
        Vertex() {
            words = 0;
            prefixes = 0;
            edges = new HashMap<Character, Vertex>();
        }
    }

    /**
     * Node of word
     * @author coder
     */
    public class Node {
        protected double weight;
        protected String word;

        /**
         * Node class
         * @param wd word
         * @param wht weight
         */
        Node(String wd, double wht) {
            weight = wht;
            word = wd;
        }
    }

    /**
     * Node of character
     * @author coder
     */
    protected class CharNode {
        protected Character ch;
        protected Vertex vt;

        /**
         * char node constructor
         * @param c char
         * @param v vertex
         */
        CharNode(Character c, Vertex v) {
            ch = c;
            vt = v;
        }
    }	


    /**
     * sort comparator
     * @author coder
     */
    protected class SortByChar implements Comparator<Object> {
        @Override
            public int compare(Object o1, Object o2) {
                CharNode n1 = (CharNode) o1;
                CharNode n2 = (CharNode) o2;

                char c1 = n1.ch.charValue();
                char c2 = n2.ch.charValue();

                if (dict.equals("")) {

                    if (n1.ch > n2.ch) {
                        return 1;
                    } else if (n1.ch == n2.ch) {
                        return 0;
                    } else {
                        return -1;
                    }
                } else {
                    int p1 = dict.length();
                    int p2 = p1;

                    for (int i = 0; i < dict.length(); i++) {
                        if (c1 == dict.charAt(i)) {
                            p1 = i;
                        }

                        if (c2 == dict.charAt(i)) {
                            p2 = i;
                        }
                    }

                    return p1 - p2;
                }
            }
    }

    /**
     * Trie constructor
     * @param ds dict sequence
     */
    public Trie(String ds) {
        root = new Vertex();
        dict = ds;
    }

    /**
     * Trie constructor
     */
    public Trie() {
        root = new Vertex();
        dict = "";
    }

    /** 
     * List all words in the Trie.
     * 
     * @return List<String>
     */
    public List<String> listAllWords() {

        List<String> words = new ArrayList<String>();
        //Vertex[] edges = root.edges;
        HashMap<Character, Vertex> edges = root.edges;

        SortedSet<CharNode> sscn = new TreeSet<CharNode>(new SortByChar());

        Iterator iter = edges.entrySet().iterator();
        while (iter.hasNext()) {
            Map.Entry<Character, Vertex> entry = (Map.Entry<Character, Vertex>) iter.next();

            Character key = entry.getKey();
            Vertex ver = entry.getValue();

            CharNode cn = new CharNode(key, ver);
            sscn.add(cn);
        }

        Iterator itr = sscn.iterator();
        while (itr.hasNext()) {
            CharNode cn = (CharNode) itr.next();

            String word = "" + (char) cn.ch;

            if (cn.vt.words != 0) {
                words.add(word);
            }

            depthFirstSearchWords(words, cn.vt, word);
        }

        return words;
    }

    /** 
     * Depth First Search words in the Trie and add them to the List.
     * 
     * @param words words list to return
     * @param vertex root node of trie
     * @param wordSegment segment of words
     */
    private void depthFirstSearchWords(List<String> words, Vertex vertex, 
            String wordSegment) {

        SortedSet<CharNode> sscn = new TreeSet<CharNode>(new SortByChar());

        HashMap<Character, Vertex> edges = vertex.edges;

        Iterator iter = edges.entrySet().iterator();
        while (iter.hasNext()) {

            Map.Entry<Character, Vertex> entry = (Map.Entry<Character, Vertex>) iter.next();

            Character key = entry.getKey();
            Vertex ver = entry.getValue();

            sscn.add(new CharNode(key, ver));
        }

        Iterator itr = sscn.iterator();
        while (itr.hasNext()) {
            CharNode cn = (CharNode) itr.next();

            String newWord = wordSegment + (char) cn.ch;
            if (cn.vt.words != 0) {
                words.add(newWord);
            }

            depthFirstSearchWords(words, cn.vt, newWord);
        }
    }

    /**
     * counter prefix of words
     * @param prefix word prefix
     * @return total words with prefix
     */
    public int countPrefixes(String prefix) {
        return countPrefixes(root, prefix);
    }

    /**
     * count prefix from root
     * @param vertex root node
     * @param prefixSegment word prefix
     * @return total count
     */
    private int countPrefixes(Vertex vertex, String prefixSegment) {
        //reach the last character of the word
        if (prefixSegment.length() == 0) { 
            return vertex.prefixes;
        }

        char c = prefixSegment.charAt(0);

        HashMap<Character, Vertex> edges = vertex.edges;

        if (edges.keySet().contains(c)) {
            return countPrefixes(edges.get(c), prefixSegment.substring(1));
        } else {
            return 0;
        }
    }

    /**
     * Count words
     * @param word complete word
     * @return the count in the trie
     */
    public int countWords(String word) {
        return countWords(root, word);
    }

    /**
     * Count words
     * @param vertex root node
     * @param wordSegment word lefts
     * @return count
     */
    private int countWords(Vertex vertex, String wordSegment) {
        //reach the last character of the word
        if (wordSegment.length() == 0) { 
            return vertex.words;
        }

        char c = wordSegment.charAt(0);

        HashMap<Character, Vertex> edges = vertex.edges;

        if (edges.keySet().contains(c)) {
            return countWords(edges.get(c), wordSegment.substring(1));
        } else {
            return 0;
        }
    }

    /**
     * add word and weight
     * @param word only-letter word 
     * @param weight double value
     */
    public void addWord(String word, double weight) {
        addWord(root, word, weight);
    }

    /** *//**
     * Add a word to the Trie.
     * 
     * @param word The word to be added.
     */

    public void addWord(String word) {
        addWord(root, word, 0.0);
    }


    /** *//**
     * Add the word from the specified vertex.
     * @param vertex The specified vertex.
     * @param word The word to be added.
     * @param weight double value
     */

    private void addWord(Vertex vertex, String word, double weight) {
        if (word.length() == 0) { 
            //if all characters of the word has been added
            vertex.words++;
            vertex.weight = weight;
        } else {
            vertex.prefixes++;

            char c = word.charAt(0);

            HashMap<Character, Vertex> edges = vertex.edges;

            if (!edges.keySet().contains(c)) {
                Vertex v = new Vertex();

                edges.put(c, v);
            }

            addWord(edges.get(c), word.substring(1), weight);
        }
    }

    /**
     * find a word in trie
     * @param s word
     * @param isFullWord is full word or not
     * @return find or not
     */
    public boolean find(String s, boolean isFullWord) {

        char[] w = s.toCharArray();
        Vertex vertex = root;

        for (int i = 0; i < w.length; i++) {
            char c = w[i];

            HashMap<Character, Vertex> edges = vertex.edges;

            if (edges.keySet().contains(c)) {
                vertex = edges.get(c);
            } else {
                return false;
            }
        }

        if (vertex.words == 0 && !isFullWord) {
            return true;
        }
        if (vertex.words != 0 && isFullWord) {
            return true;
        }

        return false;
    }

    /**
     * get prefix list
     * @param prefix prefix of word
     * @return List<String> prefixList
     */
    public List<Node> getPrefixList(String prefix) {
        List<Node> list = new ArrayList<Node>();

        char[] w = prefix.toCharArray();
        Vertex vertex = root;

        for (int i = 0; i < w.length; i++) {
            char c = w[i];

            HashMap<Character, Vertex> edges = vertex.edges;

            if (edges.keySet().contains(c)) {
                vertex = edges.get(c);
            } else {
                return list;
            }
        }

        if (vertex.words != 0) {
            list.add(new Node(prefix, vertex.weight));
        }

        getPrefixWords(list, vertex, prefix);

        return list;
    }

    /**
     * get all words under node vertex
     * @param list result list
     * @param vertex root node
     * @param wordSeg the word to find
     */
    private void getPrefixWords(List<Node> list, Vertex vertex, 
            String wordSeg) {
        HashMap<Character, Vertex> edges = vertex.edges;

        Iterator iter = edges.entrySet().iterator();
        while (iter.hasNext()) {
            Map.Entry<Character, Vertex> entry = (Map.Entry<Character, Vertex>) iter.next();

            Character key = entry.getKey();
            Vertex ver = entry.getValue();

            String newWord = wordSeg + (char) key;

            if (ver.words != 0) {
                //find a word
                Node n = new Node(newWord, ver.weight);
                list.add(n);
            }

            getPrefixWords(list, ver, newWord);
        }
    }

    /**
     * get weight of node
     * @param term to search string
     * @return double weight, 0.0 if not found
     */
    public double getWeight(String term) {
        double ret = 0.0;

        char[] w = term.toCharArray();
        Vertex vertex = root;

        for (int i = 0; i < w.length; i++) {
            char c = w[i];

            HashMap<Character, Vertex> edges = vertex.edges;

            if (edges.keySet().contains(c)) {
                vertex = edges.get(c);
            } else {
                return ret;
            }
        }

        ret = vertex.weight;

        return ret;
    }

    /**
     * insert new word into trie
     * @param s word to insert
     */
    public void insert(String s) {
        if (null == s || "".equals(s)) {
            throw new IllegalArgumentException();
        }

        addWord(s);
    }

    /**
     * @return true for has words in trie
     */
    public boolean hasWords() {

        if (root.prefixes > 0) {
            return true;
        }

        return false;
    }

    /**
     * main function
     * @param args parameters from command
     */
    public static void main(String[] args) {
        Trie t = new Trie("");
        t.insert("hello");
        t.insert("hey");
        t.insert("goodbye");
        System.out.println(t.find("hell", false));
        System.out.println(t.find("hello", true));
        System.out.println(t.find("good", false));
        System.out.println(t.find("bye", false));
        System.out.println(t.find("heyy", false));
        System.out.println(t.find("hell", true));   
    }
}
