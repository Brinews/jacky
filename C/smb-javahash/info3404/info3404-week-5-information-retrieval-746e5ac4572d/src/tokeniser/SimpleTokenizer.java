package tokeniser;

import core.Constants;
import stemmer.PorterStemmer;

import java.util.ArrayList;

/**
 * Simple tokeniser to convert a text string into an array of tokens.
 * - Remove all punctuation and numbers
 * - Return only the words that remain (delimited by whitespace)
 * - Ignore stop words
 */
public class SimpleTokenizer implements Tokenizer {
    @Override
    public String[] parse(String text) {
        ArrayList<String> words = new ArrayList<String>();
        /** Token the text **/
		text = text.toLowerCase();

		StringBuilder sb = new StringBuilder();

		for (int i = 0; i < text.length(); i++) {
			char c = text.charAt(i);
			if ((c >= 'a' && c <= 'z')) sb.append(c);
			else if (c == '\'') continue;
			else sb.append(' ');
		}

		String onlywords = sb.toString().trim();
		String[] wordsList = onlywords.split("\\s+");

		PorterStemmer ps = new PorterStemmer();

		for (int i = 0; i < wordsList.length; i++) {

			int found = 0;
			for (int j = 0; j < Constants.STOP_WORDS.size(); j++) {
				if (wordsList[i].equals(Constants.STOP_WORDS.get(j))) {
					found = 1;
					break;
				}
			}

			if (found == 0) {
				wordsList[i] = ps.stem(wordsList[i]);
				words.add(wordsList[i]);
			}
		}

        return words.toArray(new String[words.size()]);
    }
}
