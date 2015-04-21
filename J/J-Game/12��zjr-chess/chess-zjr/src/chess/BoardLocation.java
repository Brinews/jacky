
package chess;

public class BoardLocation
{
	private int x;
	private int y;
	
	public BoardLocation(int x, int y)
	{
		this.x = x;
		this.y = y;
	}
	
	public int getX()
	{
		return x;
	}
	
	public int getY()
	{
		return y;
	}
	
	public boolean equals(BoardLocation otherLocation)
	{
		return this.x == otherLocation.getX() && this.y == otherLocation.getY();
	}
	
	public String toString()
	{
		return "" + (char)('A' + x) + "" + (y + 1) + "";
	}
	
	public static String getNullToString()
	{
		return "--";
	}
	
	public static BoardLocation getNewBoardLocationFromTableCoordinates(int x, int y)
	{
		return new BoardLocation(x, Resources.BOARD_WIDTH - (y + 1));
	}
}
