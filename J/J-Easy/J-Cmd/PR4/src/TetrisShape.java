/** Build a Tetris shape, having 4 abilities: create and roate
 * @author x 
 * @date Spring 2016
 * @version 1
 * @email x@ucsd.edu
 */
import java.util.ArrayList;
import java.util.Random;

public class TetrisShape {
    
    private final static int SIZE = 8;
    private final static int SHAPE = 7;
    private int shapesize;
    private int[] shape;
    private Coord[] cords;
    private Coord[] offsets;

    private static int[] oshape = {-1,-1,-1,0,0,0,0,-1}; /* four position */
    private static int[] ishape = {-2,0,-1,0,0,0,1,0};
    private static int[] sshape = {-1,0,-1,1,0,0,0,-1};
    private static int[] zshape = {-1,-1,-1,0,0,1,0,0};
    private static int[] lshape = {-2,0,-1,0,0,1,0,0};
    private static int[] jshape = {-2,0,-1,0,0,0,0,-1};
    private static int[] tshape = {-1,-1,-1,0,-1,1,0,0};

    private static int[][] shapes = {oshape, ishape, sshape,
                            zshape, lshape, jshape, tshape};

    private static Random r = new Random();

    /** Constructor
     * @param sp shape kind
     */
    public TetrisShape(int sk)
    {
        shape = new int[SIZE];

        for (int i = 0; i < SIZE; i++)
            shape[i] = shapes[sk][i];

        cords = new Coord[SIZE/2];

        for (int i = 0; i < SIZE/2; i++) {
            cords[i] = new Coord(shape[2*i], shape[2*i+1]);
        }

        offsets = new Coord[SIZE/2];
        for (int i = 0; i < SIZE/2; i++) {
            offsets[i] = new Coord(shape[2*i], shape[2*i+1]);
        }
    }

    /** create a random shape
     * @return TetrisShape get a shape randomly
     */
    public static TetrisShape randShape() 
    {
        return new TetrisShape(r.nextInt(SHAPE));
    }

    /** rotate clockwise
     * (r, c) to (-c, r)
     */
    public void rotateClockwise()
    {
        int[] temp = new int[SIZE];
        for (int i = 0; i < SIZE/2; i++) {
            temp[2*i] = -shape[2*i+1];
            temp[2*i+1] = shape[2*i];
        }

        for (int i = 0; i < SIZE; i++) 
            shape[i] = temp[i];

        for (int i = 0; i < SIZE/2; i++) {
            cords[i].setRow(shape[2*i]);
            cords[i].setCol(shape[2*i+1]);
        }
    }

    /** rotate counterclockwise
     * (r, c) to (c, -r)
     */
    public void rotateCounterClockwise()
    {
        int[] temp = new int[SIZE];
        for (int i = 0; i < SIZE/2; i++) {
            temp[2*i] = shape[2*i+1];
            temp[2*i+1] = -shape[2*i]; /* (r, c) -> (c, -r) */
        }

        for (int i = 0; i < SIZE; i++) 
            shape[i] = temp[i];

        for (int i = 0; i < SIZE/2; i++) {
            cords[i].setRow(shape[2*i]);
            cords[i].setCol(shape[2*i+1]);
        }
    }

    /** getter
     * @return Coord[] the array of coordinates as stored
     */
    public Coord[] getOrigCoords()
    {
        return cords;
    }

    /** setter
     * update offsetcoords by (x, y)
     * @param rowoffset offset of x
     * @param coloffset offset of y
     */
    public void updateOffsetCoords(int rowoffset, int coloffset)
    {
        for (int i = 0; i < SIZE/2; i++) {
            offsets[i].setRow(cords[i].getRow()+rowoffset);
            offsets[i].setCol(cords[i].getCol()+coloffset);
        }
    }

    /** getter
     * @return Coord[] the array of coordinates with a given offset
     */
    public Coord[] getOffsetCoords()
    {
        return offsets;
    }
}
    
