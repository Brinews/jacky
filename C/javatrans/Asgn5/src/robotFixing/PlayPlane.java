package robotFixing;

import java.awt.Point;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;

import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JPanel;

public class PlayPlane extends JPanel implements MouseMotionListener, MouseListener {
	private static final long serialVersionUID = 1L;
	
	private ImgPlane plane;
	private int rows;
	private int cols;
	
	private JLabel chunkLabels[][]; //not initiated yet
	private Point xyPoint;
	private Point indexPoint;
	private Point enteredIndexPoint;
	private boolean donePlay;
	private boolean ableToMove;
	
	public PlayPlane(ImgPlane plane, int rows, int cols){
		this.plane = plane;
		this.rows = rows;
		this.cols = cols;
		this.donePlay = false;
		this.setAbleToMove(false);
		
		this.initPlayPlane();
		this.addMouseListener(this);
		this.addMouseMotionListener(this);
	}
	
	private void initPlayPlane(){
		this.setLayout(null);
		this.setEnabled(false);
		
		plane.randomImgPosition();
		chunkLabels = new JLabel[rows][cols];
		
		for(int i=0;i<rows;++i){
			for(int j=0;j<cols;++j){
				JLabel chunkLabel = new JLabel(new ImageIcon(plane.getImgChunk().get(plane.getImgPosition()[i][j])));
				chunkLabels[i][j] = chunkLabel;
				this.add(chunkLabels[i][j]);
				chunkLabels[i][j].setBounds(j*(plane.getChunkWidth()+1), i*(plane.getChunkHeight()+1), plane.getChunkWidth(), plane.getChunkHeight());
			}
		}
	}
	
	public void rearrangePlane(){
		this.removeAll();
		for(int i=0;i<rows;++i){
			for(int j=0;j<cols;++j){
				JLabel chunkLabel = new JLabel(new ImageIcon(plane.getImgChunk().get(plane.getImgPosition()[i][j])));
				chunkLabels[i][j] = chunkLabel;
				this.add(chunkLabels[i][j]);
				chunkLabels[i][j].setBounds(j*(plane.getChunkWidth()+1), i*(plane.getChunkHeight()+1), plane.getChunkWidth(), plane.getChunkHeight());
			}
		}
	}
	
	private Point xy2Index(Point xyPoint){
		Point indexPoint = new Point(xyPoint.y/(plane.getChunkHeight()+1),xyPoint.x/(plane.getChunkWidth()+1));
		return indexPoint;
	}
	
	private Point index2Xy(Point indexPoint){
		Point xyPoint = new Point(indexPoint.y*(plane.getChunkWidth()+1),indexPoint.x*(plane.getChunkHeight()+1));
		return xyPoint;
	}
	
	private void resetExchangedLabelPosition(Point p1, Point p2){
		chunkLabels[p1.x][p1.y].setIcon(new ImageIcon(plane.getImgChunk().get(plane.getImgPosition()[p1.x][p1.y])));
		chunkLabels[p2.x][p2.y].setIcon(new ImageIcon(plane.getImgChunk().get(plane.getImgPosition()[p2.x][p2.y])));
		chunkLabels[p1.x][p1.y].repaint();
		chunkLabels[p2.x][p2.y].repaint();
	}
	
	public boolean hasDonePlay(){
		return donePlay;
	}

	@Override
	public void mouseDragged(MouseEvent e) {
		if (this.isAbleToMove()) {
			if(donePlay==false){
				chunkLabels[enteredIndexPoint.x][enteredIndexPoint.y].setBounds(e.getX()-plane.getChunkWidth()/2, e.getY()-plane.getChunkHeight()/2, plane.getChunkWidth(), plane.getChunkHeight());
				//right
				if(e.getX()>((enteredIndexPoint.y+1)*plane.getChunkWidth())){ 
					Point temp = new Point(enteredIndexPoint.x, enteredIndexPoint.y+1);
					if (temp.y < this.cols) {
						plane.exchangeImgPosition(enteredIndexPoint,temp);
						this.resetExchangedLabelPosition(enteredIndexPoint, temp);
						chunkLabels[enteredIndexPoint.x][enteredIndexPoint.y].setBounds(index2Xy(enteredIndexPoint).x, index2Xy(enteredIndexPoint).y, plane.getChunkWidth(), plane.getChunkHeight());
						enteredIndexPoint = new Point(temp.x,temp.y);
					}
				}
				//left
				else if(e.getX()<((enteredIndexPoint.y)*plane.getChunkWidth())){ 
					Point temp = new Point(enteredIndexPoint.x, enteredIndexPoint.y-1);
					if (temp.y >= 0 ) {
						plane.exchangeImgPosition(enteredIndexPoint,temp);
						this.resetExchangedLabelPosition(enteredIndexPoint, temp);
						chunkLabels[enteredIndexPoint.x][enteredIndexPoint.y].setBounds(index2Xy(enteredIndexPoint).x, index2Xy(enteredIndexPoint).y, plane.getChunkWidth(), plane.getChunkHeight());
						enteredIndexPoint = new Point(temp.x,temp.y);
					}
				}
				//up
				else if(e.getY()<((enteredIndexPoint.x)*plane.getChunkHeight())){ 
					Point temp = new Point(enteredIndexPoint.x-1, enteredIndexPoint.y);
					if (temp.x >= 0) {
						plane.exchangeImgPosition(enteredIndexPoint,temp);
						this.resetExchangedLabelPosition(enteredIndexPoint, temp);
						chunkLabels[enteredIndexPoint.x][enteredIndexPoint.y].setBounds(index2Xy(enteredIndexPoint).x, index2Xy(enteredIndexPoint).y, plane.getChunkWidth(), plane.getChunkHeight());
						enteredIndexPoint = new Point(temp.x,temp.y);
					}
				}
				//down
				else if(e.getY()>((enteredIndexPoint.x+1)*plane.getChunkHeight())){ 
					Point temp = new Point(enteredIndexPoint.x+1, enteredIndexPoint.y);
					if (temp.x < this.rows) {
						plane.exchangeImgPosition(enteredIndexPoint,temp);
						this.resetExchangedLabelPosition(enteredIndexPoint, temp);
						chunkLabels[enteredIndexPoint.x][enteredIndexPoint.y].setBounds(index2Xy(enteredIndexPoint).x, index2Xy(enteredIndexPoint).y, plane.getChunkWidth(), plane.getChunkHeight());
						enteredIndexPoint = new Point(temp.x,temp.y);
					}
				}else;
			}
		}	
	}

	@Override
	public void mouseMoved(MouseEvent e) {
		// TODO Auto-generated method stub
	}

	@Override
	public void mouseClicked(MouseEvent e) {
		// TODO Auto-generated method stub
	}

	@Override
	public void mousePressed(MouseEvent e) {
		if (this.isAbleToMove()) {
			if (donePlay == false) {
				xyPoint = e.getPoint();
				indexPoint = xy2Index(xyPoint);
				enteredIndexPoint = indexPoint;
				this.remove(chunkLabels[indexPoint.x][indexPoint.y]);
				this.add(chunkLabels[indexPoint.x][indexPoint.y]);
				chunkLabels[indexPoint.x][indexPoint.y].setIcon(new ImageIcon(this.plane.adaptImg(plane.getImgChunk().get(plane.getImgPosition()[indexPoint.x][indexPoint.y]), 0.3f)));
			}
		} 
	}

	@Override
	public void mouseReleased(MouseEvent e) {
		if (this.isAbleToMove()) {
			if(donePlay==false){
				chunkLabels[enteredIndexPoint.x][enteredIndexPoint.y].setBounds(index2Xy(enteredIndexPoint).x, index2Xy(enteredIndexPoint).y, plane.getChunkWidth(), plane.getChunkHeight());
				this.donePlay = true;
				this.setAbleToMove(false);
			}
		}
	}

	@Override
	public void mouseEntered(MouseEvent e) {
	}

	@Override
	public void mouseExited(MouseEvent e) {
	}

	public boolean isAbleToMove() {
		return ableToMove;
	}

	public void setAbleToMove(boolean ableToMove) {
		this.ableToMove = ableToMove;
	}
	
	public void setHighlightChunk(int[] input) {
		for (int i = 0; i < this.rows * this.cols; i++) {
			for (int j = 0; j < this.rows; j++) {
				for (int k = 0; k < this.cols; k++) {
					if (plane.getImgPosition()[j][k] == input[i]) {
						chunkLabels[j][k].setIcon(new ImageIcon(this.plane.adaptImg(plane.getImgChunk().get(plane.getImgPosition()[j][k]), 1.7f)));
					}
				}	
			}
		}
	}
	
	public void successEffect(int[] input) {
		for (int i = 0; i < this.rows * this.cols; i++) {
			for (int j = 0; j < this.rows; j++) {
				for (int k = 0; k < this.cols; k++) {
					if (plane.getImgPosition()[j][k] != input[i]) {
						chunkLabels[j][k].setIcon(new ImageIcon(this.plane.adaptImg(plane.getImgChunk().get(plane.getImgPosition()[j][k]), 0.1f)));
					}
				}	
			}
		}
	}
	
	/*
	 * TA's treasure, something may be useful
	 * */
	
	public void setBlur() { // set images in PlayPlane be blurred  
		for (int j = 0; j < this.rows; j++) {
			for (int k = 0; k < this.cols; k++) {
				chunkLabels[j][k].setIcon(new ImageIcon(this.plane.adaptImg(plane.getImgChunk().get(plane.getImgPosition()[j][k]), (float)Math.random() * 0.4f)));
			}	
		}
	}
}
