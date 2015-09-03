package core;

import java.util.Arrays;
import java.util.Iterator;
import java.util.NoSuchElementException;

/**
 * Matrix class to handle resizing/expansion of the rows and columns
 */
public class Matrix {

    private Vector[] matrix;

    public Matrix() {
        matrix = new Vector[0];
    }

    /**
     * Sets a value in the matrix
     */
    public void setValue(int row, int column, double value) {
        matrix[row].set(column, value);
    }

    /**
     * Increments a value in the matrix
     */
    public void incrementValue(int row, int column) {
        double value = matrix[row].get(column);
        matrix[row].set(column, value + 1);
    }

    /**
     * Multiply matrix by a row vector
     */
    public void multiply(Vector other) {
        for(int i = 0; i < numberOfRows(); i ++) {
            Vector row = matrix[i];
            for(int j = 0; j < row.size(); j++) {
                double value = row.get(j);
                row.set(j, value * other.get(j));
            }
        }
    }

    /**
     * Returns the number of columns in the matrix
     */
    public int numberOfColumns() {
        if(matrix.length == 0 || matrix[0] == null) return 0;
        return matrix[0].size();
    };

    /**
     * Add a column to the matrix
     */
    public int addColumn() {
        int columns = numberOfColumns();
        for(int i = 0; i < matrix.length; i++) {
            matrix[i].increaseSize(1);
        }
        return columns;
    }

    /**
     * Returns a copy of the row values of the i-th column
     * [0, 0, 2]                      [0]
     * [2, 3, 1]   => getColumn(1) => [3]
     * [4, 0, 1]                      [0]
     */
    public Vector getColumn(int index) {
        Vector columns = new Vector(numberOfRows());
        for(int i = 0; i < columns.size(); i++) {
            double value = matrix[i].get(index);
            columns.set(i, value);
        }
        return columns;
    }

    /**
     * Returns the number of rows in the matrix
     */
    public int numberOfRows() {
        return matrix.length;
    }

    /**
     * Adds a row to the matrix
     * @return the index of the row that was added
     */
    public int addRow() {
        int size = numberOfRows();
        matrix = Arrays.copyOf(matrix, size + 1);
        matrix[size] = new Vector(numberOfColumns());
        return size;
    }

    /**
     * Returns the i-th row in the matrix (as a copy)
     * [0, 0, 2]
     * [2, 3, 1]   => getColumn(1) => [2, 3, 1]
     * [4, 0, 1]
     */
    public Vector getRow(int i) {
        if(i < 0 || i >= numberOfRows()) throw new NoSuchElementException();
        return matrix[i].clone();
    }

    /**
     * Sums up the columns in the matrix
     */
    public Vector sumOfColumns() {
        Vector sum = new Vector(numberOfColumns());
        for(Vector row : matrix) {
            sum.add(row);
        }
        return sum;
    }

    /**
     * Returns an iterator over the rows of the matrix
     */
    public Iterator<Vector> iterator() {
        return Arrays.asList(matrix).iterator();
    }

    @Override
    public Matrix clone() {
        Matrix cloned = new Matrix();
        cloned.matrix = new Vector[numberOfRows()];
        for(int i = 0; i < cloned.matrix.length; i++) {
            cloned.matrix[i] = this.matrix[i].clone();
        }
        return cloned;
    }

}
