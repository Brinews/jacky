import java.util.Scanner;
public class GridTest
{
	public static void main (String [] args)
	{
		Scanner scnr = new Scanner (System.in);
		System.out.println("Enter height, width");
		int h = scnr.nextInt();
		int w = scnr.nextInt();
		System.out.println("=== Grid with Centered Oval === ");
		AsciiGrid grid = new AsciiGrid(h,w);
		AsciiShape shape = new Oval(h/2,w/2,'#');
		grid.placeShape(shape,h/4,w/4);
		System.out.println(grid);	

		System.out.println("=== Grid with Partially Overlapped Oval === ");
		grid = new AsciiGrid(h,w);
		grid.placeShape(shape,h/4,3*w/4);
		System.out.println(grid);	
	}
}