import java.util.Scanner;
import java.util.ArrayList;
import java.util.List;
import java.lang.Integer;

/** Ascii Art Paint program
 *
 * @author xxxx
 * @version CSE11 Winter 2016
 *
 * Main control of ascii art, maintain the shape list
 */
public class AsciiArt 
{
	private AsciiGrid grid = null;
	private List<AsciiShape> shapeList = new ArrayList<AsciiShape>();
	private String errMessage;
	public static final char DEFSYMB = '#';

	/** Constructor 
	 * Construct a Art with a grid and no shapes
	 */
	public AsciiArt()
	{
		grid = new AsciiGrid(20, 40);
		shapeList.clear();
	}

	/** Parse args to int
	 * @param args string array
	 * @param num number of to parse
	 * @return int[] integer after parsed
	 */
	private int[] convertArgs(String[] args, int num)
	{
		int[] ret = new int[num];

		for (int i = 1; i <= num; i++) {
			try {
				ret[i-1] = Integer.parseInt(args[i]);
			} catch (Exception e) {
				ret[i-1] = -1;
			}
		}

		return ret;
	}


	/** Add Grid 
	 * @param args contains the height and width of grid
	 */
	private boolean addGrid(String [] args)
	{
		errMessage = "Invalid grid parameters";

		if (args.length != 3)
			return false;

		int [] parms = convertArgs(args, 2);

		if (parms[0] < 0 || parms[1] < 0)
			return false;

		grid = new AsciiGrid(parms[0],parms[1]);

		return true;
	}

	/** Print grid
	 */
	public boolean printGrid(String[] args)
	{
		errMessage = "Invalid print parameters";

		if (args.length > 1) return false;

		System.out.print(grid.toString());
		return true;
	}

	/** Add Oval 
	 * @param args number of rows in the shape and width of the shape
	 */
	private boolean addOval(String [] args)
	{
		errMessage = "Invalid shape parameters";

		if (args.length != 3)
			return false;

		int [] parms = convertArgs(args, 2);

		if (parms[0] < 0 || parms[1] < 0)
			return false;

		AsciiShape shape = new Oval(parms[0], parms[1], DEFSYMB);
		shapeList.add(shape);

		return true;
	}

	/** Add Rectangle
	 * @param args number of rows in the shape and width of the shape
	 */
	private boolean addRectangle(String [] args)
	{
		errMessage = "Invalid shape parameters";

		if (args.length != 3)
			return false;

		int [] parms = convertArgs(args, 2);

		if (parms[0] < 0 || parms[1] < 0)
			return false;

		AsciiShape shape = new Rectangle(parms[0],parms[1], DEFSYMB);
		shapeList.add(shape);

		return true;
	}

	/** Add Triangle
	 * @param args number of rows in the shape and width of the shape
	 */
	private boolean addTriangle(String [] args)
	{
		errMessage = "Invalid shape parameters";

		if (args.length != 3)
			return false;

		int [] parms = convertArgs(args, 2);

		if (parms[0] < 0 || parms[1] < 0)
			return false;

		AsciiShape shape = new Triangle(parms[0],parms[1], DEFSYMB);
		shapeList.add(shape);

		return true;
	}

	/**
	 * List all shapes
	 */
	public boolean list(String[] args)
	{
		errMessage = "Invalid list parameters";

		if (args.length > 1) return false;

		for (int i = 0; i < shapeList.size(); i++) {
			System.out.println(i+":");
			System.out.format("%s\n", shapeList.get(i).toString());
		}

		return true;
	}

	/** Place shape in grid
	 * @param args idx pos-x pos-y
	 */
	private boolean placeShape(String [] args)
	{
		errMessage = "Invalid place parameters";

		if (args.length != 4)
			return false;

		int [] parms = convertArgs(args, 3);

		if (parms[0] < 0 || parms[1] < 0 || parms[2] < 0)
			return false;

		if (parms[0] >= shapeList.size()) return false;

		int []gsize = grid.getSize();

		if (parms[1] > gsize[0] || parms[2] > gsize[1]) return false;

		grid.placeShape(shapeList.get(parms[0]), parms[1], parms[2]);

		return true;
	}

	/** clear shape in grid
	 * @param args idx pos-x pos-y
	 */
	private boolean clearShape(String [] args)
	{
		errMessage = "Invalid clear parameters";

		if (args.length != 4)
			return false;

		int [] parms = convertArgs(args, 3);

		if (parms[0] < 0 || parms[1] < 0 || parms[2] < 0)
			return false;

		if (parms[0] >= shapeList.size()) return false;

		int []gsize = grid.getSize();

		if (parms[1] >= gsize[0] || parms[2] >= gsize[1]) return false;

		grid.clearShape(shapeList.get(parms[0]), parms[1], parms[2]);

		return true;
	}

	/** set symbol
	 * @param args idx symbol
	 */
	private boolean setSymbol(String [] args)
	{
		errMessage = "Invalid symbol parameters";

		if (args.length != 3)
			return false;

		int [] parms = convertArgs(args, 1);

		if (parms[0] < 0) return false;
		if (parms[0] >= shapeList.size()) return false;

		shapeList.get(parms[0]).setSymbol(args[2].charAt(0));

		return true;
	}

	/**
	 * return errMessage
	 */
	public String getErrMessage()
	{
		return errMessage;
	}

	/** Test Main
	 * @param args parameters
	 */
	public static void main (String [] argvs) 
	{
		Scanner scnr = new Scanner (System.in);
		String line;

		AsciiArt art = new AsciiArt();

		while (true) {
			System.out.print("> ");
			line = scnr.nextLine();

			/*
			 * parse command
			 */
			String []args = line.trim().split("\\s+");

			if (args.length < 1) continue;

			String cmd = args[0].toLowerCase();
			boolean ret = true;

			if (cmd.equals("grid")) {
				ret = art.addGrid(args);
			} else if (cmd.equals("print")) {
				ret = art.printGrid(args);
			} else if (cmd.equals("oval")) {
				ret = art.addOval(args);
			} else if (cmd.equals("rectangle")) {
				ret = art.addRectangle(args);
			} else if (cmd.equals("triangle")) {
				ret = art.addTriangle(args);
			} else if (cmd.equals("list")) {
				ret = art.list(args);
			} else if (cmd.equals("place")) {
				ret = art.placeShape(args);
			} else if (cmd.equals("clear")) {
				ret = art.clearShape(args);
			} else if (cmd.equals("symbol")) {
				ret = art.setSymbol(args);
			} else if (cmd.equals("exit")) {
				break;
			}

			if (ret == false) {
				System.out.println("BAD INPUT: "+art.getErrMessage());
			} else {
				System.out.println("OK");
			}
		}
	}
}
