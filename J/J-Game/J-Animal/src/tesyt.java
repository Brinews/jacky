import java.io.*;

public class tesyt {
	
	
	   public static void main(String args[]){
	      String Str1 = new String("abcdefghijklmn");
	      char[] Str2 = new char[7];

	      try{
	         Str1.getChars(0, 1, Str2, 0);
	         System.out.print("Copied Value = " );
	         System.out.println(Str2 );

	      }catch( Exception ex){
	         System.out.println("Raised exception...");
	      }
	   }
	}


