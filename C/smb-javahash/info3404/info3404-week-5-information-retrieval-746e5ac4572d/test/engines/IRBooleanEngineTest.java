package engines;

import core.Constants;
import core.DTMatrix;
import org.junit.Before;
import org.junit.Test;
import tokeniser.SimpleTokenizer;

import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import static org.junit.Assert.*;

public class IRBooleanEngineTest {

    private DTMatrix matrix;
    private Engine engine;

    @Before
    public void setUp() throws Exception {
        matrix = new DTMatrix(Constants.TOKENIZER);
        matrix.addDocument("t1", "what a fool, he thinks himself to be wise");
        matrix.addDocument("t2", "a wise man knows himself to be a fool");
        matrix.addDocument("t3", "a truly wise man knows he knows nothing");
        matrix.addDocument("t4", "a wise man does first, what a fool does last");
        engine = new IRBooleanEngine(matrix);
    }

    @Test
    public void testExecute() throws Exception {
        Map<String, Double> result = engine.execute("is a man a fool?");
        assertEquals("Returning too many elements, only return those with result > 0", 4, result.size());
        assertEquals("The result calculation is wrong", 0.015, (double)result.get("t1"), 0.001);
        assertEquals("The result calculation is wrong", 0.031, (double)result.get("t2"), 0.001);
        assertEquals("The result calculation is wrong", 0.031, (double)result.get("t2"), 0.001);
        assertEquals("The result calculation is wrong", 0.031, (double)result.get("t4"), 0.001);
    }


    @Test
    public void testCalculateStatistics() throws Exception {
        Map<String, Double > result = new HashMap<String, Double>();
        result.put("t1", 0.5);
        result.put("t2", 0.5);
        List<String> expected = Arrays.asList("t1", "t3");
        assertEquals("Recall Algorithm is incorrect", 0.5, engine.calculateRecall(expected, result), 0.001);
        assertEquals("Precision Algorithm is incorrect", 0.5, engine.calculatePrecision(expected, result), 0.001);
        assertEquals("Fallout Algorithm is incorrect", 0.5, engine.calculateFallout(expected, result), 0.001);
    }

    @Test
    public void testCalculateStatisticsComplex() throws Exception {
        // Add more elements
        matrix.addDocument("t5", "a wise man does first, what a fool does last");
        matrix.addDocument("t6", "a wise man does first, what a fool does last");
        matrix.addDocument("t7", "a wise man does first, what a fool does last");
        engine = new IRBooleanEngine(matrix);

        Map<String, Double > result = new HashMap<String, Double>();
        result.put("t1", 0.5);
        result.put("t2", 0.5);
        result.put("t5", 0.2);
        result.put("t6", 0.1);
        result.put("t7", 0.1);

        List<String> expected = Arrays.asList("t1", "t3", "t6");
        assertEquals("Recall Algorithm is incorrect", 0.666, engine.calculateRecall(expected, result), 0.001);
        assertEquals("Precision Algorithm is incorrect", 0.4, engine.calculatePrecision(expected, result), 0.001);
        assertEquals("Fallout Algorithm is incorrect", 0.75, engine.calculateFallout(expected, result), 0.001);
    }
}
