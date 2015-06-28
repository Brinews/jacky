import java.util.Vector;
import java.io.*;

public class CompressedSuffixTrie {

	/** You need to define your data structures for the compressed trie */

	class Node {}
	class LeafNode extends Node {
		int beginIndex;
		LeafNode more;
	}
	class InternalNode extends Node {
		Edge[] child = new Edge[27];
	}
	class Edge {
		int beginIndex;
		int endIndex;
		Node node;
	}

	InternalNode root = null;
	String string;

	/**
	 * @param string to check string
	 * @return String src+'$'
	 */
	public static String convert (String string) {
		string = string.toLowerCase();

		for (int i = 0; i < string.length(); i++) {
			char c = string.charAt(i);
			if (c != '$' && (c < 'a' || c > 'z'))
				throw new IllegalArgumentException("Illegal character");
		}
		if (string.charAt(string.length()-1) != '$') string = string + '$';
		return string;
	}

	/**
	 * @param ch 
	 * @return the corresponding int.
	 */
	public static int index (char ch) {
		return (ch == '$')? 0 : ch - 'a' + 1;
	}

	/** 
	 * Constructor 
	 * @param f source file
	 */
	public CompressedSuffixTrie( String  f ) {
		// Create a compressed suffix trie from file f
		String inputString = "";

		try {
			BufferedReader br = new BufferedReader(new FileReader(f));
			StringBuffer sb = new StringBuffer();
			String temp = br.readLine();

			while (temp != null) {
				sb.append(temp);
				temp = br.readLine();
			}

			inputString = sb.toString();

		} catch (Exception e) {
			e.printStackTrace();
		}

		//System.out.println(inputString);

		// check illegal
		string = convert(inputString);
		root = new InternalNode();

		for (int i = 0; i < string.length(); i++) {
			LeafNode leaf = insert(i, root);
			leaf.beginIndex = i;
		}
	}

	/**
	 * Insert string starting at loc into the tree rooted at node.
	 * @param loc the location with string at which to start.
	 * @param node the tree node at which the insertion starts.
	 * @return the leaf node where the inserted string ends.
	 */
	private LeafNode insert (int loc, InternalNode node) {
		LeafNode leaf;
		// First '$' after loc
		int endLoc = 1 + string.indexOf('$', loc);    

		int subscript = index(string.charAt(loc));
		Edge edge = node.child[subscript];

		if (edge == null) {
			// Brand new edge
			edge = new Edge();
			edge.beginIndex = loc;
			edge.endIndex = endLoc;
			edge.node = new LeafNode();
			node.child[subscript] = edge;
			return (LeafNode) edge.node;
		}

		String edgeString = string.substring(edge.beginIndex, edge.endIndex);
		if (string.substring(loc, endLoc).equals(edgeString)) {
			// Shared leaf node
			leaf = new LeafNode();
			leaf.more = (LeafNode) edge.node;
			edge.node = leaf;
			return leaf;
		}

		// Edge has to be 'broken' at first disagreement
		int i, j;
		for (i = edge.beginIndex, j = loc;
				(i < edge.endIndex) && (string.charAt(i) == string.charAt(j)); 
				i++, j++) {}
		if (i >= edge.endIndex) {
			// Recursively insert the remaining string in the edge's node.
			return insert(j, (InternalNode) edge.node);
		}

		InternalNode newNode = new InternalNode();

		Edge newEdge = new Edge();
		newEdge.beginIndex = i;
		newEdge.endIndex = edge.endIndex;
		newEdge.node = edge.node;
		edge.endIndex = i;
		edge.node = newNode;

		newNode.child[index(string.charAt(i))] = newEdge;

		leaf = insert(j, newNode);

		return leaf;
	}

	/**
	 * Locate all longest prefixes of a query string.
	 * @param query a String
	 * @param root an InternalNode at which to start
	 * @param prefixOK true iff prefix match is OK.
	 * @return an array of indices showing all the places where query starts.
	 */
	private int[] locate (String query, Node node, boolean prefixOK) {
		if (query.length() == 0) 
			return collect(node);

		if (node instanceof LeafNode) {
			if (prefixOK) return collect(node);
			return new int[0];
		}

		Edge edge = ((InternalNode) node).child[index(query.charAt(0))];
		if (edge == null) {
			if (prefixOK) return collect(node);
			return new int[0];
		}

		int queryLength = query.length();
		int i, q;
		for (i = edge.beginIndex, q = 0;
				(q < queryLength) && (i < edge.endIndex) && 
				(query.charAt(q) == string.charAt(i)); i++, q++) {}

		if (q >= queryLength) 
			return collect(edge.node);
		else if (i >= edge.endIndex) 
			return locate(query.substring(q), edge.node, prefixOK);
		else if (prefixOK) 
			return collect(edge.node);
		else 
			return new int[0];
	}

	/**
	 * Collect all the beginnings in all descendent leaf nodes.
	 * @param node Node to start at.
	 */
	private int[] collect (Node node) {
		Vector v = new Vector();
		collect(node, v);
		Object[] elements = v.toArray();
		int[] result = new int[elements.length];
		for (int i = 0; i < result.length; i++) 
			result[i] = ((Integer) elements[i]).intValue();
		return result;
	}

	private void collect (Node node, Vector v) {
		if (node instanceof LeafNode) {
			LeafNode leaf = (LeafNode) node;
			do {
				v.add(new Integer(leaf.beginIndex));
				//System.out.println(leaf.beginIndex);

				leaf = leaf.more;
			} while (leaf != null);
		}
		else {
			InternalNode iNode = (InternalNode) node;
			for (int i = 0; i < 27; i++) {
				if (iNode.child[i] != null) collect(iNode.child[i].node, v);
			}
		}
		return;
	}

	/**
	 * Find all occurrences of a query string.
	 * @return int[] an array of integer 
	 */
	public int[] findAll (String query) {
		return locate(query, root, false);
	}

	/** 
	 * Method for finding the first occurrence of a pattern s 
	 * in the DNA sequence 
	 * @param s pattern
	 * @return int first find location, -1 for no found
	 * Time Cost: O(m), m is patten's length
	 */ 
	public int findString( String s ) { 

		int[] ret = findAll(s.toLowerCase());

		if (ret.length == 0) return -1;

		int minPos = string.length();
		for (int i = 0; i < ret.length; i++) {
			if (ret[i] < minPos) 
				minPos = ret[i];
		}

		return minPos;
	}

	/**
	 * find LCS 
	 * @param src string one
	 * @param dst string two
	 * Time Cost: O(mn) 
	 *   two loop of i,j 
	 */
	public static String findLCS(String src, String dst) {
		int len1 = src.length();
		int len2 = dst.length();

		int[][] lcs = new int[len1+1][len2+1];
		int[][] dir = new int[len1+1][len2+1];

		for (int i = 0; i <= len1; i++) lcs[i][0] = 0;
		for (int i = 0; i <= len2; i++) lcs[0][i] = 0;

		for (int i = 1; i <= len1; i++) {
			for (int j = 1; j <= len2; j++) {
				if (src.charAt(i-1) == dst.charAt(j-1)) {
					lcs[i][j] = lcs[i-1][j-1] + 1;
					dir[i][j] = 3;
				} else {
					if (lcs[i-1][j] >= lcs[i][j-1]) {
						lcs[i][j] = lcs[i-1][j];
						dir[i][j] = 2;
					} else {
						lcs[i][j] = lcs[i][j-1];
						dir[i][j] = 1;
					}
				}
			}
		}

		StringBuffer ret = new StringBuffer();

		int i = len1, j = len2;
		int k = lcs[len1][len2] - 1;

		while (k >= 0 && i > 0 && j > 0) {
			if (dir[i][j] == 3) {
				ret.append(src.charAt(i-1));
				i--;
				j--;
			} else if (dir[i][j] == 2) {
				i--;
			} else if (dir[i][j] == 1) {
				j--;
			} else break;
		}

		return ret.reverse().toString();
	}

	/** Method for computing the degree of similarity of 
	 * two DNA sequences stored in the text files f1 and f2 
	 *
	 * Time Cost = O(mn)
	 *   call findLCS(s1,s2)
	 */ 
	public static float similarityAnalyser(String f1, String f2, String f3) {
		//System.out.println(findLCS("ABCBDAB", "BDCABA"));
		String src = "", dst = "";

		try {
			BufferedReader br1 = new BufferedReader(new FileReader(f1));
			BufferedReader br2 = new BufferedReader(new FileReader(f2));
			StringBuffer sb1 = new StringBuffer();
			StringBuffer sb2 = new StringBuffer();

			/* read f1 */
			String temp = br1.readLine();
			while (temp != null) {
				sb1.append(temp);
				temp = br1.readLine();
			}

			src = sb1.toString();

			/* read f2 */
			temp = br2.readLine();
			while (temp != null) {
				sb2.append(temp);
				temp = br2.readLine();
			}

			dst = sb2.toString();

		} catch (Exception e) {
			e.printStackTrace();
		}

		String lcs = findLCS(src, dst);
		//System.out.println(lcs);

		/* write f3 */
		try {
			BufferedWriter wr = new BufferedWriter(new FileWriter(f3));
			wr.write(lcs, 0, lcs.length());

			wr.flush();
			wr.close();
		} catch (Exception e) {
			e.printStackTrace();
		}

		float simi = 0;
		int max = src.length();
		if (dst.length() > max) max = dst.length();

		simi = lcs.length();

		return simi/max;
	}

	public static void main(String args[]) throws Exception{
		/** Construct a trie named trie1
		*/
		CompressedSuffixTrie trie1 = new CompressedSuffixTrie("file1");
		System.out.println("ACTTCGTAAG is at: " + trie1.findString("ACTTCGTAAG"));

		System.out.println("AAAACAACTTCG is at: " + trie1.findString("AAAACAACTTCG")); 
		System.out.println("ACTTCGTAAGGTT : " + trie1.findString("ACTTCGTAAGGTT")); 
		System.out.println("TA is at: " + trie1.findString("TA"));
		System.out.println(CompressedSuffixTrie.similarityAnalyser("file2", "file3", "file4"));
	}
}

