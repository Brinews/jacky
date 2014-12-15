/***
 * Author: 2975653
 * Date: 2014-05-16
 * Function: Circle Queue
 */
public class Josephus
{
	private ListReferenceBased lrb = new ListReferenceBased();

	public Josephus()
	{
	}

	/****
	 * Use the list to contain Josepuse
	 */
	public void initJosephus(int num)
	{
		try {
			lrb.removeAll();

			for (int i = 1; i <= num; i++)
			{
				lrb.add(i-1, i);
			}
		}
		catch (Exception liobe)
		{
			System.out.println(liobe.getMessage());
		}
	}

	/****
	 * Begin simulation by remove iTh once a time
	 */
	public String soluteJosephus(int iTh)
	{
		if (iTh > lrb.size()) return "";

		int prevStart = -1;

		while (lrb.size() > 1)
		{
			int nowSize = lrb.size();
			int i = 0;
			while (i < iTh)
			{
				prevStart++;
				prevStart %= nowSize;

				i++;
			}
			
			try {

				String t = lrb.get(prevStart).toString();
				lrb.remove(prevStart);
				prevStart--;

				System.out.println("Remove person at position " + t);
			}
			catch (ListIndexOutOfBoundsException e)
			{
				e.printStackTrace();
			}
		}

		String ret = "";
		try {
			ret = lrb.get(0).toString();
		}
		catch (ListIndexOutOfBoundsException e)
		{
			e.printStackTrace();
		}

		return ret;
	}

	public static void main(String[] args)
	{
		Josephus jos = new Josephus();

		if (args.length < 2)
		{
			System.out.printf("Usage: Josephus <Total> <iTh>");
		}

		int num = 0, iTh = 0;
		try 
		{
			num = Integer.parseInt(args[0]);
			iTh = Integer.parseInt(args[1]);
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}

		jos.initJosephus(num);

		System.out.println("Josephus problem by 2975653:");
		System.out.println("Total number of people: " + args[0] + ",");
		System.out.println("Eliminate every " + args[1] + "th person.");
		System.out.println("Solution...");

		String r = jos.soluteJosephus(iTh);
		System.out.println("The safest position in the circle is " + r + "!");
	}
}
