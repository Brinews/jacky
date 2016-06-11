/** Build a Tetris coordinate (x, y)
 * @author x 
 * @date Spring 2016
 * @version 1
 * @email x@ucsd.edu
 */

public class Coord {

    private int row;
    private int col;
    
    /** Constructor
     * @param r row - coordinate
     * @param c column - coordinate
     */
    public Coord(int r, int c)
    {
        row = r;
        col = c;
    }

    /** Constructor
     * @param initial copy constructor from initial
     */
    public Coord(Coord initial) 
    {
        row = initial.row;
        col = initial.col;
    }

    /** getter
     * @return int row
     */
    public int getRow() 
    {
        return row;
    }

    /** getter
     * @return int col
     */
    public int getCol()
    {
        return col;
    }

    /** setter
     * @param r row
     */
    public void setRow(int r)
    {
        row = r;
    }

    /** setter
     * @param c col
     */
    public void setCol(int c)
    {
        col = c;
    }

    /** tostring
     * @return String litaral
     */
    public String toString()
    {
        return "(" + row + "," + col + ")";
    }
}
    
