package core;

import java.util.AbstractList;
import java.util.Arrays;

/**
 * Generic Vector Class
 */
public class Vector extends AbstractList<Double> {

    private double[] data;

    public Vector(int capacity) {
        data = new double[capacity];
    }

    public Vector(double... elements) {
        this.data = elements;
    }

    @Override
    public Double get(int index) {
        return data[index];
    }

    @Override
    public Double set(int index, Double element) {
        data[index] = element;
        return element;
    }

    /**
     * Returns the size of the vector
     */
    @Override
    public int size() {
        return data.length;
    }

    /**
     * Increases the size of the vector
     */
    public void increaseSize(int increment) {
        data = Arrays.copyOf(data, data.length+increment);
    }

    /**
     * Increments an element in the vector by 'value'
     */
    public void increment(int index, int value) {
        data[index] += value;
    }

    /**
     * Adds a vector to this vector
     */
    public void add(Vector other) {
        for(int i = 0; i < data.length; i++) {
            data[i] += other.data[i];
        }
    }

    /**
     * Multiplies a vector by another vector, and returns the result
     */
    public static Vector multiply(Vector a, Vector b) {
        if(a.size() != b.size()) throw new UnsupportedOperationException("Vectors need to be equal size");
        Vector c = new Vector(a.size());
        c.data = Arrays.copyOf(a.data, a.size());
        for(int i = 0; i < a.size(); i++) {
            c.set(i, a.get(i) * b.get(i));
        }
        return c;
    }

    /**
     * Multiplies a vector by a scalar value and returns a new vector
     */
    public static Vector multiply(Vector a, int scalar) {
        Vector c = new Vector(a.size());
        c.data = Arrays.copyOf(a.data, a.size());
        for(int i = 0; i < a.size(); i++) {
            c.set(i, a.get(i) * scalar);
        }
        return c;
    }

    /**
     * Sum up a vector
     */
    public double sum() {
        double total = 0;
        for(int i = 0; i < data.length; i++) {
            total += data[i];
        }
        return total;
    }

    /**
     * Puts all the values of the vector the power of 'exponent'
     * [2]                [8 ]
     * [4]  => pow(3)  => [64]
     * [1]                [1]
     */
    public void pow(int exponent) {
        for(int i = 0; i < data.length; i++) {
            data[i] = Math.pow(data[i], exponent);
        }
    }

    /**
     * Clones a vector
     */
    @Override
    public Vector clone() {
        Vector cloned = new Vector(0);
        cloned.data = Arrays.copyOf(data, data.length);
        return cloned;
    }

    /**
     * Checks if a double array is equal to this vector
     */
    public boolean equals(double[] other) {
        for(int i = 0; i < data.length; i++) {
            if(Math.abs(data[i]-other[i]) > 0.001) {
                return false;
            }
        }
        return true;
    }
}
