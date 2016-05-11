import java.lang.Math;
/** Defines Shapes where a "pixel" is a printable character.
 *
 * @author Philip Papadopoulos
 * @version CSE11 Winter 2016
 *
 * A set of classes for creating AsciiShapes
 *        Parent Class: AsciiShape
 *        Child Classes: Square, Triangle, Oval 
 */
public abstract class AsciiShape 
{
	protected Character [][] shape; 
	public static final char DEFAULTSYMBOL = '*';
	protected int H, W;   // Height and Width, for code convenience
	private static final char NEWLINE = '\n';
	private static final char  SPACE= ' ';
	

	/** Constructor 
	 * Construct a shape with no width or height 
	 */
	public AsciiShape()
	{
		shape = new Character [0][0];
		H = W = 0;
	}

	/** Constructor 
	 * @param height number of rows in the shape
	 * @param width  max width of the shape
	 */
	public AsciiShape(int height, int width)
	{
		H = (height > 0 ? height : 0);
		W = (width > 0 ? width : 0);
		shape = new Character[height][width];
	};

	/** Constructor 
	 * @param height number of rows in the shape
	 * @param width  max width of the shape
	 * @param symbol symbol to use for printing the shape
	 */
	public AsciiShape(int height, int width, char symbol)
	{
		this(height,width);
	};



	/** return a 2D Character array of the shape 
	 * This is a full/deep copy, not a reference to internal
	 * storage.
	 *
	 * 
	 * @return rectangular array of Character. Non-null array elements define
	 * what part of the array a specific shape occupies.
	 */
	public Character [][] getShape()
	{
		Character [][] rarray = new Character[H][W];
		for (int i = 0; i < H; i++)
			for(int j = 0; j < W; j++)
				if (shape[i][j] != (Character) null)
					rarray[i][j] = new Character(shape[i][j]);
		return rarray;
	}

	/** return a Height of the shape 
	 * @return height of shape 
	 */
	public int getHeight()
	{
		return H;
	}

	/** return a width of the shape 
	 * @return width of shape 
	 */
	public int getWidth()
	{
		return W;
	}

	/**
	 * set the symbol used by the shape
	 * @param symbol the symbol to use
	 */
	public abstract void setSymbol(char symbol);

	/**
	 * String representation of the shape
	 */
	@Override
	public String toString()
	{
		int i,j;
		String output = "";
		for (i = 0; i < H; i++) 
		{
			for (j = 0; j < W; j++)
				output += (shape[i][j] != null ? (char)shape[i][j] : SPACE); 
			if (i < (H-1))
				output += NEWLINE;
		}
		return output;
	}
}

/* ############  Child Classes ########### */
/* These can be declared in this file, because they are not
 * public classes. 
 *   	Oval, Rectangle, Triangle
 * 
 * Any java class in the same directory as these can utilize these classes
 */

class Oval extends AsciiShape
{
	/** Constructor 
	 * @param height number of rows in an Oval 
	 * @param width  max width of an Oval 
	 * @param symbol symbol to use for printing the shape
	 *
	 * The shape is an oval (ellipse). 
	 */
	public Oval(int height, int width, char symbol)
	{
		super(height, width);
		setSymbol(symbol);
	}

	/** Constructor 
	 * @param height number of rows in an Oval 
	 * @param width  max width of an Oval 
	 *
	 * uses the DEFAULTSYMBOL as the symbol
	 */
	public Oval(int height, int width) {
		this(height,width,DEFAULTSYMBOL);
	}

	/** Set the non-null Characters in this shape
	 *  @param symbol the symbol itself
	 *
	 *  This sets the symbol an oval shape
	 */
	public void setSymbol(char symbol)
	{
		// Pixelate an oval rough algorithm, compute if approximately 
		// more than 1/2 the pixel would be inside the ellipse 
		double r2 = 0;
	 	double [] focus1 = {0,0} , focus2 = {0,0};
		if (H > W) 
		{ // Taller than wide case
			double F = Math.sqrt((H*H - W*W)/4.0);
			focus1[0] = focus2[0] = (W-1)/2.0; 
			focus1[1] = (H-1)/2.0 + F;
			focus2[1] = (H-1)/2.0 - F;
			r2 = focus1[1]+0.5 + focus2[1]+0.5;
		}
		else
		{  // Wider than tall case
			double F = Math.sqrt((W*W - H*H)/4.0);
			focus1[1] = focus2[1] = (H-1)/2.0;
			focus1[0] = (W-1)/2.0 + F; 
			focus2[0] = (W-1)/2.0 - F;
			r2 = focus1[0]+0.5 + focus2[0]+0.5;
		}
		// System.out.format("Foci: (%f,%f)  (%f,%f)\n", focus1[0],focus1[1],
		//		focus2[0],focus2[1]);
		// System.out.format("dist sum: %f\n",r2);
		double EPS = 0.0;
		int i, j;

		// compute distance from point to foci
		for (i = 0; i < H; i++)
		{
			for( j = 0; j < W; j++)
			{
				double dist; 
				dist = Math.sqrt(Math.pow(i-focus1[1],2) + 
							Math.pow(j-focus1[0],2));
				dist += Math.sqrt(Math.pow(i-focus2[1],2) + 
							Math.pow(j-focus2[0],2));
				// System.out.format("(%d,%d): %f || ",i,j,dist);
				if (dist - r2 <= EPS)
					shape[i][j] = new Character(symbol);
			}
			// System.out.println();
		}
	}
}
class Rectangle extends AsciiShape
{
	/** Constructor 
	 * @param height height of the rectangle
	 * @param width  width of the  rectangle
	 * @param symbol symbol to use for printing the shape
	 */
	public Rectangle(int height, int width, char symbol)
	{
		super(height, width);
		setSymbol(symbol);
	}

	/** Constructor with the default symbol 
	 * @param height height of the rectangle
	 * @param width  width of the  rectangle
	 *
	 * uses the DEFAULTSYMBOL as the symbol
	 */
	public Rectangle(int height, int width) 
	{
		// invoke the constructor with the explicit symbol
		this(height,width,DEFAULTSYMBOL);
	}

	/** Set the non-null Characters in this shape
	 *  @param symbol the symbol itself
	 */
	public void setSymbol(char symbol)
	{
		for (int i = 0; i < H; i++)
			for( int j = 0; j < W; j++)
				shape[i][j] = new Character(symbol);
	}
}

class Triangle extends AsciiShape
{
	/** Constructor create a Right Triangle shape 
	 * @param height height  of the Triangle
	 * @param width  width of the Triangle
	 * @param symbol symbol to use for printing the Triangle
	 */
	public Triangle(int height, int width, char symbol)
	{
		super(height,width);	
		setSymbol(symbol);
	}

	/** Constructor 
	 * @param height height of the Triangle 
	 * @param width  width of the Triangle 
	 *
	 * uses the DEFAULTSYMBOL as the symbol
	 */
	public Triangle(int height, int width) 
	{
		// invoke the constructor with the explicit symbol
		this(height,width,DEFAULTSYMBOL);
	}

	/** Shape-Specific setSymbol method
	 * 
	 */
	
	public void setSymbol(char symbol)
	{
		if ( H == 0 || W == 0) return;
		// Triangles. First row is length 1, last row is width
		shape[0][0] = new Character(symbol);
		for (int i = 1; i < H-1; i++)
		{
			int wid =(i * W)/H + 1; 
			for (int j = 0; j < wid; j++)
				shape[i][j] = new Character(symbol); 
		}
		for (int j = 0; j < W; j++)
				shape[H-1][j] = new Character(symbol);
	}

}
// vim: ts=4:sw=4:tw=78