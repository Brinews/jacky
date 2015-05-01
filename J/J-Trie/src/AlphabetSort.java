/**
 * sort by trie, 2015-04
 * @author coder
 */
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.List;
import java.util.Iterator;

/**
 * Sort by alphabet
 * @author name
 */
public class AlphabetSort {

	/**
	 * function to judge line
	 * @param line input string
	 * @return valid or not
	 */
	public static boolean existNonLetter(String line) {
		boolean ret = false;

		if (null == line || line.equals("")) {
			ret = true;
		}

		char[] w = line.toCharArray();

		for (int i = 0; i < w.length; i++) {
			if (!((w[i] >= 'A' && w[i] <= 'Z')
						|| (w[i] >= 'a' && w[i] <= 'z'))) {
				ret = true;
				break;
			}
		}

		return ret;
	}

	/**
	 * main function
	 * @param args parameters
	 */
	public static void main(String[] args) {

		try {

			BufferedReader reader = 
				new BufferedReader(new InputStreamReader(System.in));

			String line;
			
			line = reader.readLine();

            if (line == null) {
                throw new IllegalArgumentException();
            }
			
			// build a trie with dict
			Trie trie = new Trie(line);

			while ((line = reader.readLine()) != null) {

                /*
				if (existNonLetter(line)) {
					throw new IllegalArgumentException();
				}
                */

				if (trie.find(line, true)) {
					throw new IllegalArgumentException();
				}

				trie.insert(line);
			}

            if (!trie.hasWords()) {
                throw new IllegalArgumentException();
            }

			List<String> list = trie.listAllWords();
			Iterator iter = list.listIterator();

			while (iter.hasNext()) {
				String s = (String) iter.next();
				System.out.println(s);
			}

			reader.close();

		} catch (IOException e) {
			e.printStackTrace();
		}

	}
}
