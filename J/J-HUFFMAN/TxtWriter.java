import java.io.*;

public class TxtWriter
{
	public static void main(String[] args)
		throws Exception
	{
		FileWriter fw = new FileWriter("1.txt");

		String s = "abracadabra!";

		for (int i = 0; i < s.length(); i++)
			fw.write(s.charAt(i));

		fw.flush();

		fw.close();
	}
}
