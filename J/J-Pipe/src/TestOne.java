public class TestOne {

	public static String readLine() throws java.io.IOException
    {
        StringBuffer s = new StringBuffer();
        char c;
        while ((c = (char)System.in.read())!= '\n')
        {
            if (c == '\b' && s.length() > 0)
                s.setLength(s.length() - 1);
            else
                s.append(c);
        }
        return s.toString();
    }

	public static void main(String[] args) {
		try {
			String line = TestOne.readLine();
			System.out.println("Your Input:" + line.trim());
		} catch (Exception e) {
		}
	}
}
