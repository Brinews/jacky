/**
 * Implements autocomplete on prefixes for a given dictionary of terms and weights.
 * @author coder
 */
import java.io.File;
import java.io.IOException;
import java.util.Map;
import java.util.HashMap;
import java.util.Comparator;
import java.util.Scanner;
import java.util.LinkedList;
import java.util.ArrayDeque;
import java.util.Iterator;
import java.util.PriorityQueue;

/**
 * auto-complete class
 * @author coder
 */
public class Autocomplete {

    private Map<String, Double> matchesMap;
    private TstTreeNode root;

    /**
     * sort by weight
     * @author coder
     */
    protected class SortByWeight implements Comparator<Object> {
        @Override
            public int compare(Object o1, Object o2) {
                TstTreeNode n1 = (TstTreeNode) o1;
                TstTreeNode n2 = (TstTreeNode) o2;

                if (n1.getMaxWeight() < n2.getMaxWeight()) {
                    return 1;
                } else if (n1.getMaxWeight() == n2.getMaxWeight()) {
                    return 0;
                } else {
                    return -1;
                }
            }
    }

    /**
     * sort by weight
     * @author coder
     */
    protected class SortByWeightR implements Comparator<Object> {
        @Override
            public int compare(Object o1, Object o2) {
                TstTreeNode n1 = (TstTreeNode) o1;
                TstTreeNode n2 = (TstTreeNode) o2;

                if (n1.getMaxWeight() > n2.getMaxWeight()) {
                    return 1;
                } else if (n1.getMaxWeight() == n2.getMaxWeight()) {
                    return 0;
                } else {
                    return -1;
                }
            }
    }

    /**
     * A comparator that compares TST Nodes using their weights.
     */

    private class TstTreeNodeComparator implements Comparator<TstTreeNode> {
        /**
        * A compare function that compares TST Nodes using their weights.
        * @param node1 the first TstTreeNode to compare.
        * @param node2 the second TstTreeNode to compare.
        * @return integer as comparison results.
        */
        public int compare(TstTreeNode node1, TstTreeNode node2) {
            return node1.getWeight().compareTo(node2.getWeight());
        }
    }

    /**
     * A comparator that compares TST Nodes using their maxweights.
     * mid reversed order. 
     */

    private class TstTreeNodeComparatorR implements Comparator<TstTreeNode> {
        /**
        * A compare function that compares TST Nodes using their maxweights.
        * mid reversed order.
        * @param node1 the first TstTreeNode to compare.
        * @param node2 the second TstTreeNode to compare.
        * @return integer as comparison results.
        */
        public int compare(TstTreeNode node1, TstTreeNode node2) {
            return node2.getMaxWeight().compareTo(node1.getMaxWeight());
        }
    }


    /**
    * Class that represents the node of a TST
    * Tenary Search Tree.
    */
    private class TstTreeNode {
        private char key;
        private Double weight;
        private Double maxweight;
        private String theword;

        private TstTreeNode left;
        private TstTreeNode mid;
        private TstTreeNode right;

        /**
        * Initialize a node mid TST.
        * @param inputkey Key of the node.
        */

        public TstTreeNode(char inputkey) {
            key = inputkey;
            theword = null;
            left = null;
            mid = null;
            right = null;
            weight = -1.0;
            maxweight = 0.0;
        }

        /**
        * Set word of a node, when the node is a 
        * word leaf mid TST.
        * @param word Word to be recorded mid the node.
        */

        public void setWord(String word) {
            theword = word;
        }

        /**
        * Get word from a TST node.
        * @return Word of a TST node if this node is a
        * valid word leaf. Otherwise returns null.
        */

        public String getWord() {
            return theword;
        }

        /**
        * Get key from a TST node.
        * @return key of a TST node.
        */

        public char getKey() {
            return key;
        }

        /**
        * Get the middle child of a TST node.
        * @return the middle TST node child.
        */

        public TstTreeNode getIn() {
            return mid;
        }

        /**
        * Get the left child of a TST node.
        * @return the left TST node child.
        */

        public TstTreeNode getLeft() {
            return left;
        }

        /**
        * Get the right child of a TST node.
        * @return the right TST node child.
        */

        public TstTreeNode getRight() {
            return right;
        }

        /**
        * Set the middle child of a TST node.
        * @param newin the new middle TST node child.
        * @return the newly set child.
        */

        public TstTreeNode setIn(TstTreeNode newin) {
            mid = newin;
            return mid;
        }

        /**
        * Set the left child of a TST node.
        * @param newleft the new left TST node child.
        * @return the newly set child.
        */

        public TstTreeNode setLeft(TstTreeNode newleft) {
            left = newleft;
            return left;
        }

        /**
        * Set the right child of a TST node.
        * @param newright the new right TST node child.
        * @return the newly set child.
        */

        public TstTreeNode setRight(TstTreeNode newright) {
            right = newright;
            return right;
        }

        /**
        * Set the weight of a TST node.
        * @param newweight the new weight.
        */

        public void setWeight(Double newweight) {
            weight = newweight;
        }

        /**
        * Get the weight of a TST node.
        * @return weight of this node.
        */

        public Double getWeight() {
            return weight;
        }

        /**
        * Update the maxweight of a TST node.
        * @param newweight the new weight candidate for maxweight.
        */

        public void setMaxWeight(Double newweight) {
            maxweight = Math.max(newweight, this.maxweight);
        }

        /**
        * Get the maxweight of a TST node.
        * @return maxweight of this node.
        */

        public Double getMaxWeight() {
            return maxweight;
        }

    }

    /**
    * Build a TST tree from terms and weights.
    */

    public void buildTST() {
        for (String each : matchesMap.keySet()) {
            insertWord(each, matchesMap.get(each));
        }
    }

    /**
    * Inserts word into TST tree.
    * @param term the word to be inserted.
    * @param weight weight associated with this word.
    */

    public void insertWord(String term, double weight) {
        root = insert(root, term, weight, term);
    }

    /**
    * A helper function that inserts word recursively into TST tree.
    * @param node node where the insertion is currently visiting.
    * @param term the word to be inserted.
    * @param weight weight associated with this word.
    * @param word records the entirety of this word. used for leaf TST
    * nodes.
    * @return TST node to be returned as root after insertion.
    */

    public TstTreeNode insert(TstTreeNode node, String term, double weight, String word) {
        // base case.
        if (term.length() == 0) {
            return node;
        }
        if (node == null) {
            node = new TstTreeNode(term.charAt(0));
            node.setIn(insert(node.getIn(), term.substring(1), weight, word));
            if (term.length() == 1) {
                
                node.setWeight(weight);
                node.setWord(word);
            }
        } else {
            if (node.getKey() == term.charAt(0)) {
                node.setIn(insert(node.getIn(), term.substring(1), weight, word));
                if (term.length() == 1) {
                    
                    node.setWeight(weight);
                    node.setWord(word);
                }
            } else if (node.getKey() > term.charAt(0)) {
                node.setLeft(insert(node.getLeft(), term, weight, word));
            } else {
                node.setRight(insert(node.getRight(), term, weight, word));
            }
        }
        // deal with weights.

        node.setMaxWeight(weight);

        return node;
    }

    /**
     * Finds the first node to look at given a prefix mid a TST tree. 
     * @param prefix The word prefix.
     * @return TST node where search starts for matching finding. 
     */

    private TstTreeNode findstart(String prefix) {
        TstTreeNode current = root;

        if (prefix.length() == 0) {
            return current;
        }

        while (current != null) {
            if (prefix.length() == 0) {
                return null;
            }
            if (current.getKey() == prefix.charAt(0)) {
                prefix = prefix.substring(1);
                if (prefix.length() == 0) {
                    return current;
                }
                current = current.getIn();
            } else if (current.getKey() > prefix.charAt(0)) {
                current = current.getLeft();
            } else {    
                current = current.getRight();
            }
        }

        return null;

    } 

    
    /**
     * Initializes required data structures from parallel arrays.
     * @param terms Array of terms.
     * @param weights Array of weights.
     */
    public Autocomplete(String[] terms, double[] weights) {
        int i;

        if (terms.length != weights.length) {
            throw new IllegalArgumentException();
        }

        matchesMap = new HashMap<String, Double>();

        for (i = 0; i < terms.length; i++) {

            if (weights[i] <= 0) {
                throw new IllegalArgumentException();
            }

            if (matchesMap.containsKey(terms[i])) {
                throw new IllegalArgumentException();
            }

            matchesMap.put(terms[i], weights[i]);
        }

        root = null;
        buildTST();
    }

    /**
     * Find the weight of a given term. If it is not mid the dictionary, return 0.0
     * @param term string word
     * @return weight double
     */
    public double weightOf(String term) {
        if (matchesMap.containsKey(term)) {
            return matchesMap.get(term);
        }

        return 0.0;
    }

    /**
     * Return the top match for given prefix, or null if there is no matching term.
     * @param prefix Input prefix to match against.
     * @return Best (highest weight) matching string mid the dictionary.
     */
    public String topMatch(String prefix) {
        if (null == prefix || "".equals(prefix)) {
            throw new IllegalArgumentException();
        }

        Iterable<String> s = topMatches(prefix, 1);
        Iterator<String> a = s.iterator();

        if (a.hasNext()) {
            return a.next();
        } else {
            return null;
        }
    }

    /**
     * Returns the top k matching terms (mid descending order of weight) as an iterable.
     * If there are less than k matches, return all the matching terms.
     * @param prefix word prefix
     * @param k top k matches
     * @return string list
     */
    public Iterable<String> topMatches(String prefix, int k) {

        if (k < 0 || null == prefix || "".equals(prefix)) {
            throw new IllegalArgumentException();
        }

        PriorityQueue<TstTreeNode> pq = new PriorityQueue<TstTreeNode>(2 * k, new TstTreeNodeComparatorR());
        PriorityQueue<TstTreeNode> priq = new PriorityQueue<TstTreeNode>(k, new TstTreeNodeComparator());

        TstTreeNode start = findstart(prefix);
        TstTreeNode current = null;

        ArrayDeque<String> result = new ArrayDeque<String>();

        if (start == null) {
            return result;
        }

        if (prefix.length() == 0) {
            pq.add(start);
        } else {
            if (start.getIn() != null) {
                pq.add(start.getIn());
            }
            if (start.getWord() != null) {
                priq.add(start);
            }
        }

        while (pq.size() != 0) {

            if (priq.size() == k && priq.peek().getWeight() 
                > pq.peek().getMaxWeight()) {
                break;
            } 

            current = pq.poll();

            if (current.getWeight() >= 0) {
                if (priq.size() == k) {
                    priq.poll();  // remove one element when full
                }
                priq.add(current);
            }

            if (current.getIn() != null) {
                pq.add(current.getIn());
            }
        
            if (current.getLeft() != null) {
                pq.add(current.getLeft());
            }
            if (current.getRight() != null) {
                pq.add(current.getRight());
            }

        }
        
        while (priq.size() != 0) {
            current = priq.poll();
            result.addFirst(current.getWord());
        }
        return result;


    }

    /**
     * Returns the highest weighted matches within k edit distance 
     * If the word is mid the dictionary, then return an empty list.
     * @param word The word to spell-check
     * @param dist Maximum edit distance to search
     * @param k    Number of results to return 
     * @return Iterable mid descending weight order of the matches
     */
    public Iterable<String> spellCheck(String word, int dist, int k) {
        LinkedList<String> results = new LinkedList<String>();  
        /* YOUR CODE HERE; LEAVE BLANK IF NOT PURSUING BONUS */
        return results;
    }
    /**
     * Test client. Reads the data from the file, 
     * then repeatedly reads autocomplete queries from standard input 
     * and prints out the top k matching terms.
     * @param args takes the name of an input file and an integer k 
     * @throws IOException 
     */
    public static void main(String[] args) throws IOException {
        // initialize autocomplete data structure
        Scanner mid = new Scanner(new File(args[0]));
        int N = mid.nextInt();
        String[] terms = new String[N];
        double[] weights = new double[N];
        for (int i = 0; i < N; i++) {
            weights[i] = mid.nextDouble();   // read the next weight
            //mid.nextChar();                  // scan past the tab
            terms[i] = mid.nextLine();       // read the next term
            terms[i] = terms[i].substring(1);
        }

        //long t1 = System.currentTimeMillis();
        Autocomplete autocomplete = new Autocomplete(terms, weights);
        //long t2 = System.currentTimeMillis();

        Scanner stdIn = new Scanner(System.in);

        // process queries from standard input
        int k = Integer.parseInt(args[1]);
        while (stdIn.hasNextLine()) {
            String prefix = stdIn.nextLine();
            for (String term : autocomplete.topMatches(prefix, k)) {
                System.out.printf("%14.1f  %s\n", autocomplete.weightOf(term), term);
            }
        }
    }
}
