package core;

import org.junit.Before;
import org.junit.Test;
import tokeniser.SimpleTokenizer;

import java.util.Arrays;

import static org.junit.Assert.*;

public class DTMatrixTest {


    private DTMatrix matrix;

    @Before
    public void setUp() throws Exception {
        matrix = new DTMatrix(new SimpleTokenizer());
        matrix.addDocument("t1", "what a fool, he thinks himself to be wise");
        matrix.addDocument("t2", "a wise man knows himself to be a fool");
        matrix.addDocument("t3", "a truly wise man knows he knows nothing");
        matrix.addDocument("t4", "a wise man does first, what a fool does last");
    }

    @Test
    public void testAddDocument() throws Exception {
        String message = "Make sure you are adding the document name to the HashMap";
        assertEquals(message, "t1", matrix.getDocumentNameFromIndex(0));
        assertEquals(message, "t2", matrix.getDocumentNameFromIndex(1));
        assertEquals(message, "t3", matrix.getDocumentNameFromIndex(2));
        assertEquals(message, "t4", matrix.getDocumentNameFromIndex(3));
    }

    @Test
    public void testGetInverseDocumentFrequency() throws Exception {
        double[] expected = new double[]{0.125, 0.602, 0, 0.125};
        Vector idf = matrix.getInverseDocumentFrequency();
        assertEquals("You are either eliminating too few stop words or too many non-stop words", 4, idf.size());
        assertTrue("You are likely not calculating the IDF vector properly", idf.equals(expected));
    }

    @Test
    public void testGetDocumentFrequency() throws Exception {
        double[] expected = new double[]{3, 1, 4, 3};
        Vector frequency = matrix.getDocumentFrequency();
        assertEquals("You are either eliminating too few stop words or too many non-stop words", 4, frequency.size());
        assertTrue("You are likely not calculating the frequency count properly", frequency.equals(expected));
    }

    @Test
    public void testCreateQueryVector() throws Exception {
        Vector frequency = matrix.createQueryVector("what knows a wise man");
        assertEquals("The vector needs to be the same size as the document vectors", 4, frequency.size());
        double[] expected = new double[]{0.0, 0.0, 1.0, 1.0};
        assertTrue("The expected query vector is incorrect.", frequency.equals(expected));
    }
}