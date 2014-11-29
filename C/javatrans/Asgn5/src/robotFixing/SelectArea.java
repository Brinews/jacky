package robotFixing;

import java.awt.Color;
import java.awt.Rectangle;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.List;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JPanel;

public class SelectArea extends JPanel implements ActionListener {
	private static final long serialVersionUID = 1L;
	
	private ImgPlane plane;
	private List<JButton> chunkButtons;
	private JPanel selectPlane;
	private JButton submit;
	
	private int[] selectedChunks;
	private int[][] selectedMatrix;
	private int rows;
	private int cols;
	private Rectangle bounds;
	
	private boolean selectDone;
	private boolean selectable;
	
	/** Constructor public SelectArea(ImgPlane plane, int rows, int cols)
	 * 		To construct the select area of the game.
	 *
	 *  @param ImgPlane plane - imgPlane of this round of game
	 *  @param int rows - numbers of row
	 *  @param int cols - numbers of columns   
	 * */
	public SelectArea(ImgPlane plane, int rows, int cols, Rectangle bounds){
		this.plane = plane;
		this.rows = rows;
		this.cols = cols;
		this.bounds = bounds;
		this.setSelectDone(false);
		
		this.setLayout(null);
		this.setOpaque(false);
		this.setBounds(0,0,this.bounds.width,this.bounds.height);
		
		this.selectPlane = new JPanel();
		this.submit = new JButton("<html><p align=center, color = red>PRESS TO<br>start calibrating</br></html>");

		this.initSelectPlane();
		this.initSubmitButton();
		this.initSelectedChunksAndMtx();
		
		this.add(selectPlane);
		this.add(submit);
	}
	
	/*render image chunks*/
	private void initSelectPlane(){
		this.selectPlane.setLayout(null);
		
		/* x:right;   y:center;   width:chunk's width*cols;   height:chunk's height*rows */
		this.selectPlane.setBounds(this.getWidth()/2+(this.getWidth()/2-plane.getChunkWidth()*cols)/2,
				60, plane.getImgChunk().get(0).getWidth() * (cols),
				plane.getImgChunk().get(0).getHeight() * (rows));
		this.selectPlane.setOpaque(true);
		
		chunkButtons = new ArrayList<JButton>();
		for(int i=0;i<cols*rows;++i){
			JButton chunkButton = new JButton(new ImageIcon(plane.getImgChunk().get(i)));
			chunkButton.setDisabledIcon(new ImageIcon(plane.getImgChunk().get(i)));
			this.chunkButtons.add(chunkButton);
			this.selectPlane.add(chunkButton);
			chunkButton.setBounds((i%cols)*(plane.getChunkWidth()+1), (i/cols)*(plane.getChunkHeight()+1), plane.getChunkWidth(), plane.getChunkHeight());
			chunkButton.addActionListener(this);
		}
	}

	private void initSubmitButton(){
		this.submit.setSize(150, 70);
		this.submit.setLocation((this.getWidth()-this.submit.getWidth())/2, 60+this.selectPlane.getHeight()+5);
		this.submit.setBackground(new Color(0,0,0));
		
		this.submit.setOpaque(true);
		this.submit.addActionListener(this);
	}	
	
	private void initSelectedChunksAndMtx(){
		selectedChunks = new int[rows*cols];
		for(int i=0;i<rows*cols;++i){
			selectedChunks[i] = -1;
		}
		selectedMatrix = new int[rows][cols];
	}
	
	private void selectChunk(int chunkIndex){
		chunkButtons.get(chunkIndex).setIcon(new ImageIcon(this.plane.adaptImg(plane.getImgChunk().get(chunkIndex), 1.7f)));
		chunkButtons.get(chunkIndex).setDisabledIcon(new ImageIcon(this.plane.adaptImg(plane.getImgChunk().get(chunkIndex), 1.7f)));
		selectedChunks[chunkIndex] = chunkIndex ;
	}
	
	private void unselectChunk(int chunkIndex){
		chunkButtons.get(chunkIndex).setIcon(new ImageIcon(plane.getImgChunk().get(chunkIndex)));
		chunkButtons.get(chunkIndex).setDisabledIcon(new ImageIcon(plane.getImgChunk().get(chunkIndex)));
		chunkButtons.get(chunkIndex).invalidate();
		selectedChunks[chunkIndex] = -1;
	}
	
	private void setChunksDisabled(){
		for(JButton i:chunkButtons){
			i.setEnabled(false);
		}
	}
	
	private void submit(){  //set all chunkButton at selectPlane locked
		int count=0;
		for(int i=0;i<rows;i++){
			for(int j=0;j<cols;j++){
				selectedMatrix[i][j] = selectedChunks[i*cols+j];
				if(selectedMatrix[i][j]!=-1) ++count;
			}
		}
		if(count!=0) {
			plane.setTargetPattern(selectedMatrix);
			setChunksDisabled();
		}
		this.setSelectDone(true);
	}
	
	@Override
	public void actionPerformed(ActionEvent e) {
		//after clicked, call selectChunk(), then record the selected chunks
		if(e.getSource() == submit) {
			for (int i = 0; i < this.rows*this.cols; i++) {
				if (selectedChunks[i] != -1)submit();
			}
		} else if(selectedChunks[ chunkButtons.indexOf(e.getSource()) ]!=-1) {
			unselectChunk( chunkButtons.indexOf(e.getSource()) );
		} else {
			selectChunk( chunkButtons.indexOf(e.getSource()) );
		}	
	}

	public boolean isSelectDone() {
		return selectDone;
	}

	public void setSelectDone(boolean selectDone) {
		this.selectDone = selectDone;
	}
	
	public int[] getSelectedChunks() {
		return this.selectedChunks;
	}
	
	/*
	 * TA's Treasure, something may be useful here
	 * */
	
	public void setSelectedChunks(int[] input) { // let SelectArea get inputs from other player 
		this.selectedChunks = input;
	}
	
	public boolean getSelectable() { // get if SelectArea is Selectable
		return this.selectable;
	}
	
	public void isSelectable(boolean set) { // set SelectArea be able to click or not
		this.selectable = set;
	}
	
	public void setHighlighted(int[] input, int selected, int unselected) { // highlight some chunks in SelectArea by input  
		for (int i = 0; i < input.length; i++) {
			if (input[i] != -1)chunkButtons.get(i).setIcon(new ImageIcon(this.plane.adaptImg(plane.getImgChunk().get(i), (0.1f * selected))));
			else chunkButtons.get(i).setIcon(new ImageIcon(this.plane.adaptImg(plane.getImgChunk().get(i), (0.1f * unselected))));
		}
	} 
	
	public void setInitialBlur () { // make SelectArea blurred
		for (int i = 0; i < this.rows * this.cols; i++) {
			chunkButtons.get(i).setIcon(new ImageIcon(this.plane.adaptImg(plane.getImgChunk().get(i), 0.2f)));
		}
	}
}
