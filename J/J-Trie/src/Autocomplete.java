/**
 * Implements autocomplete on prefixes for a given dictionary of terms and weights.
 * @author coder
 */
import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.Comparator;
import java.util.Scanner;
import java.util.SortedSet;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Iterator;
import java.util.TreeSet;

/**
 * auto-complete class
 * @author coder
 */
public class Autocomplete {

    /**
     * Trie Tree
     */
    private Trie trie = new Trie("");
    private HashMap matchesMap = new HashMap();

    /**
     * sort by weight
     * @author coder
     */
    protected class SortByWeight implements Comparator<Object> {
        @Override
            public int compare(Object o1, Object o2) {
                Node n1 = (Node) o1;
                Node n2 = (Node) o2;

                if (n1.weight < n2.weight) {
                    return 1;
                } else if (n1.weight == n2.weight) {
                    return 0;
                } else {
                    return -1;
                }
            }
    }

    /**
     * Initializes required data structures from parallel arrays.
     * @param terms Array of terms.
     * @param weights Array of weights.
     */
    public Autocomplete(String[] terms, double[] weights) {
        int i;

        for (i = 0; i < terms.length; i++) {
            trie.addWord(terms[i], weights[i]);
        }
    }

    /**
     * Find the weight of a given term. If it is not in the dictionary, return 0.0
     * @param term string word
     * @return weight double
     */
    public double weightOf(String term) {
        //return trie.getWeight(term);
        return (Double) matchesMap.get(term);
    }

    /**
     * Return the top match for given prefix, or null if there is no matching term.
     * @param prefix Input prefix to match against.
     * @return Best (highest weight) matching string in the dictionary.
     */
    public String topMatch(String prefix) {
        Iterable<String> s = topMatches(prefix, 1);
        Iterator<String> a = s.iterator();

        if (a.hasNext()) {
            return a.next();
        } else {
            return null;
        }
    }

    /**
     * Returns the top k matching terms (in descending order of weight) as an iterable.
     * If there are less than k matches, return all the matching terms.
     * @param prefix word prefix
     * @param k top k matches
     * @return string list
     */
    public Iterable<String> topMatches(String prefix, int k) {
        SortedSet<Node> ssn = new TreeSet<Node>(new SortByWeight());

        List<Node> retList = trie.getPrefixList(prefix);

        Iterator<Node> itr = retList.iterator();
        while (itr.hasNext()) {
            ssn.add(itr.next());
        }

        int i = 0;

        Iterator<Node> it = ssn.iterator();
        List<String> ret = new ArrayList<String>();
        matchesMap.clear();

        while (it.hasNext() && i < k) {
            Node n = it.next();

            ret.add(n.word);
            matchesMap.put(n.word, n.weight);

            i++;
        }

        return ret;
    }

    /**
     * Returns the highest weighted matches within k edit distance 
     * If the word is in the dictionary, then return an empty list.
     * @param word The word to spell-check
     * @param dist Maximum edit distance to search
     * @param k    Number of results to return 
     * @return Iterable in descending weight order of the matches
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
        Scanner in = new Scanner(new File(args[0]));
        int N = in.nextInt();
        String[] terms = new String[N];
        double[] weights = new double[N];
        for (int i = 0; i < N; i++) {
            weights[i] = in.nextDouble();   // read the next weight
            //in.nextChar();                  // scan past the tab
            terms[i] = in.nextLine();       // read the next term
            terms[i] = terms[i].substring(1);
        }

        Autocomplete autocomplete = new Autocomplete(terms, weights);

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
