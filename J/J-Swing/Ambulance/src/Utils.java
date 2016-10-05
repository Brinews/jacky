
public class Utils {
	public static String decodeString(String str) {
		if (str.charAt(0) == '\"' && str.charAt(str.length()-1) == '\"') {
			return str.substring(1, str.length()-1);
		}
		return str;
	}
	
	public static String encodeString(String str) {
		String ret = "\"";
		return ret + str + "\"";
	}
	
	public static boolean validLocation(String loc) {
		boolean ret = false;
		
		try {
			Integer p = Integer.parseInt(loc);
			if (p > 0 && p < 100) {
				ret = true;
			}
		} catch (Exception e) {
		}
		
		return ret;
	}
}
