import java.util.Scanner;
public class ShapeTest 
{
	public static void main (String [] args)
	{
		Scanner scnr = new Scanner (System.in);
		AsciiShape shape;
		System.out.print("Enter height and width: ");
		int h = scnr.nextInt();
		int w = scnr.nextInt();
		Character [][] array;
		shape = new Oval(h,w,'#');

		array=shape.getShape();
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				String entry = (array[i][j] != null ?
					array[i][j].toString() : "null");
				System.out.format("(%d,%d):%s|",i,j, entry);
			}
			System.out.println();
		}

		System.out.format("\n === toString of shape === \n%s\n",shape.toString());
	}
}