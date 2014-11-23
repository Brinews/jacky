package model;

/**
 * This is the file representing the abstract class of SentencePC and SentenceMP
 * Basic methods are implemented here
 * ArrayList is recommend as the data structure for SentencePC and SentenceMP 
 * 
 * */

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

public abstract class AbstractSentence {
	
	public List<SentenceElement> elementList = new ArrayList<SentenceElement>();
	
	public String getEngSentence(){
		StringBuilder sb = new StringBuilder();
		for (SentenceElement se : this.elementList) {
		    sb.append(se.getEng());
		}
		return sb.toString();
	}
	
	public List<String> getEnglishSpans() {
		List<String> list = new ArrayList<String>();
		for (SentenceElement se: this.elementList) {
			list.add(se.engField.trim());
		}
		return list;
	}
	
	public List<String> getChineseSpans() {
		List<SentenceElement> sortedList = new ArrayList<SentenceElement>(this.elementList);
		Collections.copy(sortedList, this.elementList);
		Collections.sort(sortedList, new Comparator<SentenceElement>() {

			@Override
			public int compare(SentenceElement se1, SentenceElement se2) {
				return se1.valueField - se2.valueField;
			}
			
		});
		List<String> orderedChineseSpans = new ArrayList<String>();
		for (SentenceElement se: sortedList) {
			orderedChineseSpans.add(se.chiField.trim());
		}
		return orderedChineseSpans;
	}
	
	protected List<Integer> randomIndexes() {
		List<Integer> indexes = new ArrayList<Integer>();
		for (int i=0; i<this.elementList.size(); i++) {
			indexes.add(i);
		}
		Collections.shuffle(indexes);
		return indexes;
	}
	
	/**
	 * List<String> getRandomizedChineseSpans()
	 * 		This is the basic randomize function that generate a random list, is called by 
	 * 		getRandomizedChineseSpansWithLevel 
	 * 
	 * @param - none
	 * @return - randomized list
	 * 
	 * */
	public abstract List<String> getRandomizedChineseSpans();
	
	/**
	 * List<String> getRandomizedChineseSpansWithLevel(int level)
	 * 		This function will call getRandomizedChineseSpans to generate a random list. You can
	 * 		implement whatever algorithm you want, even the simplest one
	 * 
	 * 		If you are trying to implement Hamming distance as a method to generate a ideal random
	 * 		list, here is some hint about using the class we provide:
	 * 		
	 * 		This method will comparing the randomized list with original input. If the randomized
	 * 		list and original input has a Hamming distance between user defined number, it will 
	 * 		return the randomized list. If no, it will call getRandomizedChineseSpans to generate
	 * 		a new one. The larger the input "level" is, the bigger the Hamming distance will be.
	 * 
	 * 		SentencePC and SentenceMP can use different range of Hamming distance range
	 * 
	 * @param - level: User defined number to distinguish your Hamming distance
	 * @return - randomized list with specific Hamming distance
	 * 
	 * */
	public abstract List<String> getRandomizedChineseSpansWithLevel(int level);
	
	/**
	 * int score(List<String> candidate)
	 * 		This function returns the score of this sentence which can be simply fixed score 
	 * 		or depend on the order the player gave.
	 * @param candidate: The list of Chinese spans with the order the player answered.
	 * @return the score of this sentence.
	 *
	 **/
	public abstract int score(List<String> candidate);
	
	public abstract String getName();
	
}