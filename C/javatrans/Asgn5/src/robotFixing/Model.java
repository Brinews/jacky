package robotFixing;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.BufferedInputStream;

public class Model {
	private int linesAmount;
	private String fileName;
	private int nowId;
	
	public Model(String fileName) throws IOException{
		linesAmount = countLines(fileName);
		this.fileName = new String(fileName);
	}
	
	/***
	 * @return now image id
	 */
	public int getImageID()
	{
		return nowId;
	}
	
	/** public ImgPlane getRandomPlane
	 * 		Randomly get a source name of a image from the file and call imgplane constructor 
	 * 		and store the information.
	 * 
	 *  @param rows - row number
	 *  @param cols - column numbers
	 *  @param givenId -if givenId>0 then load givenId image, else random
	 * */
	public ImgPlane getRandomPlane(int rows, int cols, int givenId){
		int i = 0;
		int rdm = (int)(Math.random() * (this.linesAmount + 1));
		
		/* 指定图像加载 */
		if (givenId > 0) rdm = givenId;
		nowId = rdm;
		
		String nameAndTag[] = new String[2];
		try {
			BufferedReader br =new BufferedReader(new FileReader(fileName));
			String temp = new String();
			do {
				i++;
				temp=br.readLine();
			} while(i <= rdm);
			nameAndTag = temp.split(" ", 2);
			
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		ImgPlane plane = new ImgPlane("res/assignment4TestData/"+nameAndTag[0],nameAndTag[1],rows,cols);
		return plane;
	}

	/**
	 * private static int countLines(String filename)
	 * 		return the number of lines in a file, which is by counting the number of carriage return
	 * 
	 * reference:
	 * 		http://stackoverflow.com/questions/453018/number-of-lines-in-a-file-in-java
	 **/
	public static int countLines(String filename) throws IOException {
	    InputStream is = new BufferedInputStream(new FileInputStream(filename));
	    try {
	        byte[] c = new byte[1024];
	        int count = 0;
	        int readChars = 0;
	        boolean empty = true;
	        while ((readChars = is.read(c)) != -1) {
	            empty = false;
	            for (int i = 0; i < readChars; ++i) {
	                if (c[i] == '\n') {
	                    ++count;
	                }
	            }
	        }
	        return (count == 0 && !empty) ? 1 : count;
	    } finally {
	        is.close();
	    }
	}
}
