package robotFixing;

public class Verifier {
	private int[] rowAnsPattern;
	private int[] colAnsPattern;
	private int rows, cols;
	
	public Verifier(int rows, int cols) {
		this.rows = rows;
		this.cols = cols;
	}
	
	public void setAnswer(int[][] SelectPattern) {
		this.rowAnsPattern = this.transRowArray(SelectPattern);
		this.colAnsPattern = this.transColArray(SelectPattern);
	}
	
	private int[] transRowArray(int[][] answerMetrix) {
		int[] rowPattern = new int[this.rows * this.cols];
		
		for (int i = 0; i < this.rows; i++) {
			for (int j = 0; j < this.cols; j++) {
				rowPattern[i * this.cols + j] = answerMetrix[i][j];
			}
		}
		rowPattern = this.purifyArray(rowPattern);
		return rowPattern;
	}
	
	private int[] transColArray(int[][] answerMetrix) {
		int[] colPattern = new int[this.rows * this.cols];
		for (int j = 0; j < this.cols; j++) {
			for (int i = 0; i < this.rows; i++) {
				colPattern[j * this.rows + i] = answerMetrix[i][j];
			}
		}
		colPattern = this.purifyArray(colPattern);
		return colPattern;
	}
	
	private int[] purifyArray(int[] input) {
		int start = 0, end = this.rows * this.cols;
		while (input[start] < 0) start++;
		while (input[end - 1] < 0) end--;
		int[] purifiedArray = new int[end - start];
		
		for (int i = 0; i < end - start; i++) purifiedArray[i] = input[i + start];  
		return purifiedArray;
	}
	
	public boolean puzzleMatch(int[][] input) {
		return verify(transRowArray(input), this.rowAnsPattern) && 
			   verify(transColArray(input), this.colAnsPattern);
	}
	
	private boolean verify(int[] inputArray, int[] ansPattern) {
		boolean rowPass = false;
		int start = 0;
		
		while(inputArray[start] != ansPattern[0]) start++;
		if ((start + ansPattern.length) > inputArray.length) return rowPass;
		
		for (int j = 0; j < ansPattern.length; j++) {
			if (ansPattern[j] == -1) continue;
			else {
				if (inputArray[start + j] == ansPattern[j]) rowPass = true;
				else {
					rowPass = false;
					break;
				}
			}
		}
		return rowPass;
	}
}
