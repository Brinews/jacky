/**
 * Boggle game http://en.wikipedia.org/wiki/Boggle
 * @author coder
 */
import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;
import java.util.Scanner;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Random;

/**
 * Boggle class
 * @author coder
 */
public class Boggle {

    /**
     * variables
     */
    private char[][] board;
    private int maxLen = 1000;

    /**
     * constructor
     * @param M height
     * @param N width
     */
    private void newBoard(int M, int N) {
        board = new char[M][N];
    }

    /**
     * board init
     * @param i row
     * @param j col
     * @param c char
     */
    private void setBoard(int i, int j, char c) {
        board[i][j] = c;
    }

    /**
     * board
     * @return char[][]
     */
    public char[][] getBoard() {
        return board;
    }

    /**
     * set max len
     * @param l length
     */
    public void setMaxLen(int l) {
        maxLen = l;
    }

    /**
     * get max len
     * @return length
     */
    public int getMaxLen() {
        return maxLen;
    }

    /**
     * main function
     * @param args arguments
     */
    public static void main(String[] args) {

        Boggle b = new Boggle();

        int M = 4, N = 4;
        boolean r = false;
        int K = 1;
        String dictName = "dict";

        for (int i = 0; i < args.length; i++) {
            if (args[i].equals("-r")) {
                r = true;
            }

            if (args[i].equals("-m")) {
                M = Integer.parseInt(args[++i]);
            }

            if (args[i].equals("-n")) {
                N = Integer.parseInt(args[++i]);
            }

            if (args[i].equals("-d")) {
                dictName = args[++i];
            }

            if (args[i].equals("-k")) {
                K = Integer.parseInt(args[++i]);
            }
        }

        if (N < 0 || M < 0 || K < 0) {
            throw new IllegalArgumentException();
        }

        Scanner in = new Scanner(System.in);

        b.newBoard(M, N);

        if (r == false ) {
            for (int i = 0; i < M; i++) {
                String line = in.nextLine();
                if (line.length() != N) {
                    throw new IllegalArgumentException();
                }

                for (int j = 0; j < N; j++) {
                    //b.board[i][j] = line.charAt(j);
                    b.setBoard(i, j, line.charAt(j));
                }
            }
        } else {
            Random rand = new Random();

            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    char c = (char) ('a' + rand.nextInt(27));
                    b.setBoard(i, j, c);
                }
            }
        }

        Scanner fin = null;

        try {
            fin = new Scanner(new File(dictName));
        } catch (FileNotFoundException e) {
            //e.printStackTrace();
            throw new IllegalArgumentException();
        }

        HashSet<String> dict = new HashSet<String>();

        int max = -1000;

        while (fin.hasNextLine()) {
            String line = fin.nextLine();
            if (line.length() > max) {
                max = line.length();
            }

            dict.add(line);
        }

        b.setMaxLen(max);

        HashMap<Integer, ArrayList<String>> ret = b.boggleWithTrie(b.getBoard(), dict, max);

        Iterator itr = ret.values().iterator();

        ArrayList all = new ArrayList();

        while (itr.hasNext()) {
            ArrayList<String> list = (ArrayList<String>) itr.next();

            for (int p = 0; p < list.size(); p++) {
                all.add(list.get(p));
            }
        }

        int i = 0;
        while (i < K && i < all.size()) {
            System.out.println(all.get(all.size()-i-1));
            i++;
        }
    }

    /**
     * test case 2
     */
    public void testSuite2() {
        HashSet<String> dict = new HashSet<String>();
        Collections.addAll(dict, "cat", "cate", "apple", "pear", "haha", "great");
        char[][] board = { { 'a', 'b', 'c', 'd' }, { 'v', 'r', 'a', 'f' },
            { 'j', 't', 'e', 'p' }, { 'k', 'g', 'r', 'o' } };
        long t1 = System.currentTimeMillis();
        System.out.println(boggleWithTrie(board, dict, 8));
        System.out.println("Time cost of testSuite2: " + (System.currentTimeMillis() - t1)
                / 1000.0 + " secs");
    }

    /**
     * test case 1
     */
    public void testSuite1() {
        HashSet<String> dict = new HashSet<String>();
        Collections.addAll(dict, "cat", "cate", "apple", "pear", "haha", "great");
        char[][] board = { { 'a', 'b', 'c', 'd' }, { 'v', 'r', 'a', 'f' },
            { 'j', 't', 'e', 'p' }, { 'k', 'g', 'r', 'o' } };
        long t1 = System.currentTimeMillis();
        System.out.println(boggle(board, dict));
        System.out.println("Time cost of testSuite1: " + (System.currentTimeMillis() - t1)
                / 1000.0 + " secs");
    }

    /**
     * boggle with trie
     * @param board char[M][N]
     * @param dict trie set
     * @param max max length, used to optimization
     * @return hashmap
     */
    public HashMap<Integer, ArrayList<String>> boggleWithTrie(
            char[][] board, HashSet<String> dict, int max) {
        // Pre-process dict.
        TrieDict trie = new TrieDict(dict);
        // 
        int i, j, n = board.length;
        boolean[][] visited;
        HashMap<Integer, ArrayList<String>> words = new HashMap<Integer, ArrayList<String>>();
        for (i = 0; i < n; ++i) {
            for (j = 0; j < n; ++j) {
                visited = new boolean[n][n];
                nextCharWithTrie(board, visited, "", words, i, j, trie, dict, max);
            }
        }
        return words;
            }

    /**
     * get next char
     * @param board char[M][N]
     * @param visited flags to set
     * @param cur now string
     * @param words words list
     * @param i position x
     * @param j position y
     * @param trie trie tree
     * @param dict dict set
     * @param max max length
     */
    public void nextCharWithTrie(char[][] board, boolean[][] visited,
            String cur, HashMap<Integer, ArrayList<String>> words, int i,
            int j, TrieDict trie, HashSet<String> dict, int max) {
        if (i < 0 || j < 0 || i >= board.length || j >= board.length
                || visited[i][j] || cur.length() > max || trie.getIndexes(cur) == null) {
            return;
                }
        cur = cur + board[i][j];
        if (cur.length() >= 1 && dict.contains(cur)) {
            ArrayList<String> arr = words.get(cur.length());
            if (arr == null) {
                arr = new ArrayList<String>();
            }
            if (!arr.contains(cur)) {
                arr.add(cur);
                words.put(cur.length(), arr);
            }
        }
        visited[i][j] = true;
        int Xshift, Yshift, x, y;
        for (Xshift = -1; Xshift <= 1; ++Xshift) {
            x = i - Xshift;
            for (Yshift = 1; Yshift >= -1; --Yshift) {
                y = j - Yshift;
                nextCharWithTrie(board, visited, cur, words, x, y, trie, dict, max);
            }
        }
        visited[i][j] = false;
    }

    /**
     * no trie used
     * @param board char[M][N]
     * @param dict dict set
     * @return hashmap
     */
    public HashMap<Integer, ArrayList<String>> boggle(char[][] board,
            HashSet<String> dict) {
        // General solution without trie. Exponential time complexity.
        int i, j, n = board.length;
        boolean[][] visited;
        HashMap<Integer, ArrayList<String>> words = new HashMap<Integer, ArrayList<String>>();
        for (i = 0; i < n; ++i) {
            for (j = 0; j < n; ++j) {
                visited = new boolean[n][n];
                nextChar(board, visited, "", words, i, j, dict);
            }
        }
        return words;
    }

    /**
     * next char 
     * @param board char[M][N]
     * @param visited flags
     * @param cur string now
     * @param words word list
     * @param i pos-x
     * @param j pox-y
     * @param dict dict set
     */
    public void nextChar(char[][] board, boolean[][] visited,
            String cur, HashMap<Integer, ArrayList<String>> words, int i,
            int j, HashSet<String> dict) {
        if (i < 0 || j < 0 || i >= board.length || j >= board.length
                || visited[i][j] || cur.length() > 8) {
            return;
                }
        cur = cur + board[i][j];
        if (cur.length() >= 3 && dict.contains(cur)) {
            ArrayList<String> arr = words.get(cur.length());
            if (arr == null) {
                arr = new ArrayList<String>();
            }
            arr.add(cur);
            words.put(cur.length(), arr);
        }
        visited[i][j] = true;
        int Xshift, Yshift, x, y;
        for (Xshift = -1; Xshift <= 1; ++Xshift) {
            x = i - Xshift;
            for (Yshift = 1; Yshift >= -1; --Yshift) {
                y = j - Yshift;
                nextChar(board, visited, cur, words, x, y, dict);
            }
        }
        visited[i][j] = false;
    }

    /**
     * dict class
     * @author coder
     */
    class TrieDict {
        TrieNodeDict root;

        /**
         * constructor
         * @param dict hashset
         */
        TrieDict(HashSet<String> dict) {
            this.root = new TrieNodeDict();
            int i;
            for (String str : dict) {
                for (i = 0; i < str.length(); ++i) {
                    root.insert(str.substring(i), i);
                } 
            }
        }

        /**
         * get index
         * @param s string
         * @return arraylist
         */
        public ArrayList<Integer> getIndexes(String s) {
            return root.getIndexes(s);
        }
    }


    /**
     * dict class node
     * @author coder
     */
    class TrieNodeDict {
        // Store the all indexes of a specific char.
        ArrayList<Integer> indexes; 
        HashMap<Character, TrieNodeDict> kids;
        char val;

        /**
         * constructor
         */
        TrieNodeDict() {
            this.indexes = new ArrayList<Integer>();
            this.kids = new HashMap<Character, TrieNodeDict>();
        }

        /**
         * insert function
         * @param s string
         * @param index index
         */
        public void insert(String s, int index) {
            this.indexes.add(index);
            if (s != null && s.length() > 0) {
                this.val = s.charAt(0);
                TrieNodeDict child = this.kids.get(this.val);
                if (child == null) {
                    child = new TrieNodeDict();
                    kids.put(this.val, child);
                }
                child.insert(s.substring(1), index);
            }
        }

        /**
         * get index
         * @param s string
         * @return arraylist
         */
        public ArrayList<Integer> getIndexes(String s) {
            if (s == null || s.length() == 0) {
                return this.indexes;
            } else {
                char first = s.charAt(0);
                if (this.kids.containsKey(first)) {
                    return this.kids.get(first).getIndexes(s.substring(1));
                }
            }
            return null;
        }
    }
}
