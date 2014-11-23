package model;

/**
 * This is the file doing pre-process for your input. You are ask to:
 * 1.Implement file input and load it into your data structure. 
 * 	ArrayList is recommend as the data structure for SentencePC and SentenceMP 
 * 2.A method that randomly choose a sentence in your data structure as the 
 * 	question 
 * 
 * */

import java.util.*;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class Model {
	
	/*
	 * Hints for parsing input:
	 * 
	 * Using "split" method in String class to distribute inputed line.
	 * Use regular expression as a split method    
	 * String str = new String(); 
	 * str.split("[^-0-9]"); 	  // Get only the digits in the string.
	 * str.split("[^A-Za-z ]");   // Get numbers letter with a space in the string. 
	 * str.split("[A-Za-z0-9-]"); // Get anything in the string but keep letter, digit and '-' away
	 */
	
	public List<AbstractSentence> senList = new ArrayList<AbstractSentence>();
	
	public Model()
	{
		try {
			List<List<SentenceElement>> mtlst = readDataFile("./bin/res/MTTestData_v3.txt");
			for (List<SentenceElement> slst: mtlst) {
				senList.add(new SentenceMT(slst));
			}
		} catch (IOException e){
			e.printStackTrace();
		}
		
		
		try {
			List<List<SentenceElement>> pclst = readDataFile("./bin/res/PCTestData.txt");
			for (List<SentenceElement> slst: pclst) {
				senList.add(new SentencePC(slst));
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public AbstractSentence RandomSentence()
	{
		if (senList.size() == 0) return null;
		
		int r = ((int)(Math.random()*10000))%senList.size();
		//int r = 0;
		//int r = senList.size()-1;
		
		return senList.remove(r);
	}

	/**
	 * private static String stringArrayToString(String[] str) For the return
	 * value of split is String[], turn String[] to String for better use
	 * 
	 * @input - String[] str - a String[]
	 * @return - String
	 */
	private static String stringArrayToString(String[] str) {
		String temp = null;
		for(String i: str) temp = i;
		return temp;
	}

	public List<List<SentenceElement> > readDataFile(String fileName) throws IOException {

		List<List<SentenceElement>> stcList = new ArrayList<List<SentenceElement>>();

		List<SentenceElement> eleList = new ArrayList<SentenceElement>();

		FileReader fr = new FileReader(fileName);
		BufferedReader br = new BufferedReader(fr);

		while (br.ready()) {
			String line = br.readLine();

			if (line.equals("-1")) {
				stcList.add(eleList);
				eleList = new ArrayList<SentenceElement>();
			}
			else {
				String value = stringArrayToString(line.split("[^-0-9]")); 	  // Get only the digits in the string.
				String eng = stringArrayToString(line.split("[^A-Za-z ]"));   // Get numbers letter with a space in the string. 
				String chi = stringArrayToString(line.split("[A-Za-z0-9-]")); // Get anything in the string but keep letter, digit and '-' away
				SentenceElement se = new SentenceElement(value, eng, chi);

				eleList.add(se);
			}
		}

		br.close();
		fr.close();

		return stcList;
	}
}