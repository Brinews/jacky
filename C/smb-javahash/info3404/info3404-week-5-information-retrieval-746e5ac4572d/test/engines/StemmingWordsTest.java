package engines;

import core.Constants;
import core.DTMatrix;
import org.junit.Before;
import org.junit.Test;

import java.util.Map;

import static org.junit.Assert.assertEquals;

/**
 * Tests the use of the word stemmer
 */
public class StemmingWordsTest {

    private DTMatrix matrix;
    private Engine engine;

    @Before
    public void setUp() throws Exception {
        matrix = new DTMatrix(Constants.TOKENIZER);
        matrix.addDocument("t1", "Early to bed and early to rise makes a man healthy, wealthy and wise.");
        matrix.addDocument("t2", "Always take a banana to a party, bananas are good!");
        matrix.addDocument("t3", "A doctors office provides health advice and bananas to patients");
        matrix.addDocument("t4", "A fool thinks himself to be wise, but a wise man knows himself to be a fool.");
        engine = new IRBooleanWithCosineEngine(matrix);
    }

    @Test
    public void testWordStemming() throws Exception {
        Map<String, Double> result = engine.execute("health wise");
        assertEquals("Make sure your stemming both the document AND query", 0.072, result.get("t1"), 0.001);
        assertEquals("Make sure your stemming both the document AND query", 0.390, result.get("t3"), 0.001);
        assertEquals("Make sure your stemming both the document AND query", 0.178, result.get("t4"), 0.001);

        result = engine.execute("banana health");
        assertEquals("Make sure your stemming both the document AND query", 0.258, result.get("t2"), 0.001);
        assertEquals("Make sure your stemming both the document AND query", 0.487, result.get("t3"), 0.001);
    }
}
