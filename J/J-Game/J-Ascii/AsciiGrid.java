/** define a 2D array of chars as a way to make ascii art.
 * can place and clear an arbitrary 2D array of chars in the grid
 * if asked-for array fits.
 * @author YOUR NAME HERE
 * @version CSE11 Spring 2015
 */
public class AsciiGrid
{
	private int row, col;
	private static final char  EMPTY = ' ';
	private static final char  NEWLINE = '\n';
	private static final char  HORI = '=';
	private static final char  VETI = '|';

	private Character [][]board;


	/** Constructor 
	 */
	public AsciiGrid()
	{
		row = 25;
		col = 40;

		board = new Character[row][col];

		for (int i = 0; i < row; i++)
			for (int j = 0; j < col; j++)
				board[i][j] = new Character(EMPTY);
	}
	/** Constructor 
	 * @param row number of rows in the ascii grid 
	 * @param col number of columns in the ascii grid 
	 */
	public AsciiGrid(int row, int col)
	{
		this.row = row;
		this.col = col;

		board = new Character[row][col];

		for (int i = 0; i < row; i++)
			for (int j = 0; j < col; j++)
				board[i][j] = new Character(EMPTY);
	}

	/** return a row x col array of the current char array  
	 * This should be a full/deep copy, not a reference to internal
	 * storage
	 * @return array of chars 
	 */
	public char [][] getChars()
	{
		return (char [][]) null;
	}


	/** 
	 * place the 2D shape in the grid at location (r,c) 
	 *
	 * @param shape AsciiShape object 
	 * @param r row in the grid where to place the first row of the shape
	 * @param c column in the grid where to place the first column of the shape
	 * @return true, if grid was updated, false otherwise 
	 *
	 * Implementation note: don't attempt to set anything outside of the grid.
	 * Start at coordinates (r,c). The loop through the rows and columns of
	 * the Shape (see getShape()) and adding r to the row coord, c to the
	 * column coord  of each element to 'translate the shape' to have it's
	 * upper left corner at (r,c).  Only 
	 * set of the grid element if the getShape()[i][j] element is non-null and
	 * is translated to be within the boundaries of the grid.  
	 */
	public boolean placeShape(AsciiShape shape,int r, int c)
	{
		boolean ret = false;

		Character[][] shapeArray = shape.getShape();

		for (int i = 0; i < shapeArray.length; i++) {
			for (int j = 0; j < shapeArray[i].length; j++) {
				if (r+i < row && c+j < col) {
					board[r+i][c+j] = shapeArray[i][j];
					ret = true;
				}
			}
		}

		return ret;
	}
	
	/** 
	 * clear the elements in the grid  defined by the 2D shape 
	 * starting at grid at location (r,c). 
	 * Clear is the inverse of place.  Instead of placing the chars 
	 * defined by the shape, place the EMPTY char (effectively clearing).
	 * Null Characters defined in the shape do not affect the grid (either
	 * placing or clearing). 
	 *
	 * @param shape AsciiShape object  
	 * @param r row in the grid where to start the clearing 
	 * @param c column in the grid where to start the clearing 
	 * @return true, if grid was updated, false otherwise 
	 *
	 * Implementation note: placeShape and clearShape are nearly identical.
	 * The ONLY difference is what you see the grid element to be (the symbol
	 * of the shape OR a ' '.  Define a helper.
	 */
	public boolean clearShape(AsciiShape shape,int r, int c)
	{	
		boolean ret = false;

		Character[][] shapeArray = shape.getShape();

		for (int i = 0; i < shapeArray.length; i++) {
			for (int j = 0; j < shapeArray[i].length; j++) {
				if (r+i < row && c+j < col) {
					board[r+i][c+j] = new Character(EMPTY);
					ret = true;
				}
			}
		}

		return ret;
	}
	
	/** Return the width and height of the grid 
	 *  @return array where index=0 is nrows, index=1 ncolumns 
	 */
	public int [] getSize()
	{
		int[] ret = new int[2];

		ret[0] = row;
		ret[1] = col;

		return ret;
	}

	/** create a nice, printable representation of the grid and
	 * filled coordinates
	 *
	 * the grid should be framed. A row of "=' (length = width of grid + 2)
	 * should be used to frame the top and bottom of the grid. The '|' should
	 * be used to frame the left and right side of each row of the grid. e.g 
	 * 1x1  empty grid      2 x 2 empty grid
	 * ===                  ====
	 * | |                  |  |
	 * ===                  |  |
	 *                      ====
	 */
	@Override
	public String toString()
	{
		String output = "";
		for (int i = 0; i < col+2; i++) output += HORI;
		output += NEWLINE;

		for (int i = 0; i < row; i++) {
			output += VETI;
			for (int j = 0; j < col; j++) {
				output += (board[i][j] != null ? (char)board[i][j] : EMPTY);
			}
			output += VETI;
			output += NEWLINE;
		}

		for (int i = 0; i < col+2; i++) output += HORI;
		output += NEWLINE;

		return output;
	}
	
}
// vim: ts=4:sw=4:tw=78
