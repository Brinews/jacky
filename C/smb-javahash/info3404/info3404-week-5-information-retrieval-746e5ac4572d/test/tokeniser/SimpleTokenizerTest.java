package tokeniser;

import org.junit.Before;
import org.junit.Test;

import static org.junit.Assert.*;

public class SimpleTokenizerTest {

    private Tokenizer tokenizer;

    @Before
    public void setUp() throws Exception {
        tokenizer = new SimpleTokenizer();
    }

    @Test
    public void testTokenizeString() throws Exception {
        String[] tokens = tokenizer.parse("the quick brown fox jumped over the lazy dog");
        String[] expected = new String[] {"quick", "brown", "fox", "jumped", "lazy", "dog"};
        assertArrayEquals("Failing to split the text-string into tokens properly", expected, tokens);
    }

    @Test
    public void testTokenizeStringCapitals() throws Exception {
        String[] tokens = tokenizer.parse("THE quick Brown fOx jumped over the laZy DOg");
        String[] expected = new String[] {"quick", "brown", "fox", "jumped", "lazy", "dog"};
        assertArrayEquals("Failing to convert the tokens to lowercase", expected, tokens);
    }

    @Test
    public void testTokenizeStringPunctuation() throws Exception {
        String[] tokens = tokenizer.parse("1337 The quick brown fox's jumped over the lazy dog.");
        String[] expected = new String[] {"quick", "brown", "foxs", "jumped", "lazy", "dog"};
        assertArrayEquals("Failing to split the text-string into tokens properly", expected, tokens);
    }
}
