package model;

import java.util.ArrayList;
import java.util.List;

public class SentenceMT extends AbstractSentence {

	List<String> shufList = new ArrayList<String>();
	int hammingDist = 0;
	
	public SentenceMT(List<SentenceElement> lst)
	{
		this.elementList = lst;
	}

	public List<String> getRandomizedChineseSpans()
	{
		int index = 0;

		List<String> orderedChi = getChineseSpans();

		List<Integer> idx = randomIndexes();

		shufList.clear();

		/* shuffle chinese spans */
		for (Integer i: idx) {
			shufList.add(orderedChi.get(i));
		}

		hammingDist = 0;

		/* compute hamming distance */
		for (String srcStr: orderedChi) {
			String dstStr = shufList.get(index++);

			hammingDist += HammingUtil.getDistance(srcStr, dstStr);

			System.out.println(hammingDist + ": "+srcStr + ": "+ dstStr);
		}

		return shufList;
	}

	public List<String> getRandomizedChineseSpansWithLevel(int level)
	{
		while (hammingDist <= level)
		{
			getRandomizedChineseSpans();
		}

		return shufList;
	}

	public int score(List<String> candidate)
	{
		return -2;
	}
	
	public String getName(){
		return "MT";
	}
}
