package tokeniser;

/**
 * Generic Interface for a String tokeniser
 * This splits a long string of text, into an array of the words
 */
public interface Tokenizer {

    public String[] parse(String text);

}
