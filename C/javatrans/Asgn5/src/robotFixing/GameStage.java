package robotFixing;
import java.awt.Color;
import java.awt.Font;
import java.awt.Rectangle;
import java.util.Random;

import javax.swing.JLabel;
import javax.swing.JPanel;


public class GameStage extends JPanel{
	
	private static final long serialVersionUID = 1L;

	private int time = 2000;

	private Rectangle bounds;
	private ImgPlane plane;
	private JLabel roleTag;
	private JLabel tag;
	private JLabel stageTag;
	private Verifier verifier;
	private SelectArea selectArea;
	private PlayPlane playPlane;
	private TimeBar timeBar;
	
	private boolean winFlag;
	private boolean doneRobotSummonAnimationFlag;
	private boolean isTimeUPFlag;
	private boolean isWrongMatch;
	private boolean isHighlightDone;
	private boolean animationToggle;
	
	private int rows, cols;
	
	/** Constructor GameStage(Rectangle bounds, ImgPlane plane, int rows, int cols)
	 * 		Generate a GameStage, initialize all the instance.
	 * 
	 *  @param Rectangle bounds - the size of image chunk
	 *  @param ImgPlane plane - the selected image
	 *  @param int rows - number of rows
	 *  @param int cols - number of columns
	 * */
	public GameStage(Rectangle bounds, ImgPlane plane,int rows,int cols, int stageNumber, String role){
		this.winFlag = false;
		this.doneRobotSummonAnimationFlag = false;
		this.isTimeUPFlag = false;
		this.isWrongMatch = false;
		this.isHighlightDone = false;
		
		this.plane = plane;
		this.bounds = bounds;
		
		this.rows = rows;
		this.cols = cols;
		
		this.setLayout(null);
		this.setBounds(50,0,this.bounds.width,this.bounds.height);
		Random Rcolor = new Random();
		this.setBackground(new Color(252, 200+Rcolor.nextInt(40), 60+Rcolor.nextInt(100)));
		this.setVisible(true);
			
		this.initTag();
		this.initStageTag(stageNumber);
		this.initRoleTag(role);
		
		this.verifier = new Verifier(rows,cols);
		this.selectArea = new SelectArea(plane, rows,cols,this.bounds);
		this.playPlane = new PlayPlane(plane, rows, cols);
		this.timeBar = new TimeBar();
		
		/* x:left;   y:center;   width:chunk's width*rows;   height:chunk's height*cols */
		this.playPlane.setBounds((this.getWidth()/2-plane.getChunkWidth()*cols)/2,
									60, 
									plane.getChunkWidth()*(cols), 
									plane.getChunkHeight()*(rows));
		
		if (role.equals("puzzler")) {
			this.selectArea.setInitialBlur();
			this.selectArea.isSelectable(false);
		}
		else {
			this.playPlane.setBlur();
		}
		
		this.add(roleTag);
		this.add(tag);
		this.add(stageTag);
		this.add(selectArea);
		this.add(playPlane);
		this.add(timeBar);
	}
	
	private void initRoleTag(String role)
	{
		this.roleTag = new JLabel(role);
		this.roleTag.setHorizontalAlignment(roleTag.getWidth() / 2);
		this.roleTag.setBackground(Color.WHITE);//why it's useless???? because you have to setBounds!!!
		this.roleTag.setFont(new Font("Courier", Font.PLAIN, 20));
		this.roleTag.setOpaque(true);
		
		this.roleTag.setBounds(this.getWidth()/2-370,10,150,25);
	}

	private void initTag(){
		this.tag = new JLabel(plane.getTag());
		this.tag.setHorizontalAlignment(tag.getWidth() / 2);
		this.tag.setBackground(Color.WHITE);//why it's useless???? because you have to setBounds!!!
		this.tag.setFont(new Font("Courier", Font.PLAIN, 20));
		this.tag.setOpaque(true);
		this.tag.setBounds(this.getWidth()/4*3-70,10,150,25);
	}
	
	private void initStageTag(int stageNumber){
		this.stageTag = new JLabel("No."+ stageNumber);
		this.stageTag.setBackground(Color.gray);//why it's useless???? because you have to setBounds!!!
		this.stageTag.setOpaque(true);
		this.stageTag.setHorizontalAlignment(stageTag.getWidth() / 2);
		this.stageTag.setBounds(this.getWidth()-70,0,40,20);
	}

	public boolean hasLose(){
		if(this.isTimeUp() || this.isWrongMatch) return true;
		return false;
	}
	
	public boolean hasWon() {
		return this.winFlag;
	}
	
	public boolean isPlayPlaneDone(){
		return playPlane.hasDonePlay();
	}
	
	public boolean hasDoneRobotSummonAnimation() {
		return this.doneRobotSummonAnimationFlag;
	}
	
	public boolean isTimeUp(){
		if(time<0) this.isTimeUPFlag=true;
		return this.isTimeUPFlag;
	}
	
	public void robotSummonAnimationFrame() {
		//the animation frame that makes left eye keep randomly changing its order.
		this.plane.randomImgPosition();
		playPlane.rearrangePlane();
		playPlane.validate();
	}

	/** public void timeRunning()
	 * 
	 * 	a method that managing the playing time of left part
	 *  @param none
	 * */
	public void timeRunning() {
		if(time%50==0) {
			this.timeBar.minusTime();
			this.repaint();
		}
		--time;
	}
	
	/*when user finished rolling the puzzle*/
	public void submitStage(){
		verifier.setAnswer(this.plane.getTargetPattern());//seems that it's not necessary to use TargetPattern, selectMatrix is ok!
		if(verifier.puzzleMatch(this.plane.getImgPosition())) this.winFlag=true;
		else this.isWrongMatch=true;
	}
	
	public boolean isSelectAreaDone() {
		this.setSelectAreaHighlightDone(true);
		return this.selectArea.isSelectDone();
	}
	
	public void setPlayPlaneAble(boolean set) {
		this.playPlane.setAbleToMove(set);
	}
	
	public int[] getSelectedChunk() {
		return this.selectArea.getSelectedChunks();
	}	
	public void setPlayPlaneHighlight (int[] input) {
		this.playPlane.setHighlightChunk(input);
	}

	public boolean getSelectAreaHighlightDone() {
		return this.isHighlightDone;
	}

	public void setSelectAreaHighlightDone(boolean set) {
		this.isHighlightDone = set;
	}
	
	/*
	 * TA's treasure, something may be useful
	 * */
	
	public void robotSuccessfulAnimation(int round) {
		if (this.animationToggle ) {
			this.playPlane.setHighlightChunk(this.getSelectedChunk());
			this.animationToggle = false;
		} else {
			this.playPlane.successEffect(this.getSelectedChunk());
			this.animationToggle = true;
		}
	}
	
	public void setSelcetAreaDone(boolean set) { // let gameStage control, set SelcetArea Done
		this.selectArea.setSelectDone(set);
	}
	
	public void setSelectAreaChunk(int[] input) { // let gameStage control set SelectArea, set its highlight
		this.selectArea.setSelectedChunks(input);
		this.selectArea.setHighlighted(input, 12, 6);
	}
	
	public void setSelectAreaInitialBlur () { // let gameStage control, set SelectArea Initially blurred
		this.selectArea.setInitialBlur();
	}
	
	public boolean getSelectAreaAbility() { // let gameStage control get if SelectArea is able to perform action
		return this.selectArea.getSelectable();
	}
	
	public void setSelectAreaAble (boolean set) { // let gameStage control set SelectArea is able to perform action or not 
		this.selectArea.isSelectable(set);
	}
	
	public void setPlayPlaneBlur() { // let gameStage control set PlayPlane blurred
		this.playPlane.setBlur();
	}
	
	public int[] getImgPlaneInfo() { // let gameStage get imgPlane information about whenever puzzle is done 
		int[] array = new int[this.rows * this.cols];  
		for(int i = 0; i < this.rows; i++) {
			for (int j = 0 ; j < this.cols; j++) {
				array[i * this.rows + j] = this.plane.getImgPosition()[i][j]; 
			}
		}
		return array;
	}
	
	public void setPlayPlanePosition(int[] input) { // let gameStage set something on PlayPlane
		int[][] array = new int[this.rows][this.cols];
		for (int i = 0; i < this.rows; i++) {
			for (int j = 0 ; j < this.cols; j++) {
				array[i][j] = input[i * this.rows + j]; 
			}
		}
		this.plane.setImgPosition(array);
	}
	
	public int[][] getSelectedTarget()
	{
		return this.plane.getTargetPattern();
	}
	
	public void setPaneTargetPattern(int[] target)
	{
		int row = (int) Math.sqrt(target.length);
		int[][] t= new int[row][row];
		
		for (int i = 0; i < row; i++){
			for (int j = 0; j < row; j++) {
				t[i][j] = target[i*row+j];
				
				System.out.print(t[i][j] + " ");
			}
			System.out.println("");
		}
		
		this.plane.setTargetPattern(t);
	}
	
	public void setWinFlag(boolean flag)
	{
		this.winFlag = flag;
		
		System.out.println("Win Result:" + flag);
	}
}