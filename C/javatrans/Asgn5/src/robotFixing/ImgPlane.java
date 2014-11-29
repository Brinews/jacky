/** 
 *  Class ImgPlane
 * 
 * 		A class that perform image loading, splitting and other imgPlane control method.
 *  	Initially, after loading the image, it will be split in chunks and the original position
 *  of each chunks will be saved. Other methods will see imgPlane as a library which provide 
 *  basic image information.
 *  
 *		�o��class�N�x�s����Image��������k�A�]�t�Ϥ�Ū��B�N�Ϥ�����p���A�C�Ӥp���|�̷Ө��Ӫ���m�Q�s���A�ȩ̀��x�s�b���
 *	���c��(�ѥ��ӥk�A�ѤW�ܤU)�CImgPlane�x�s����T�N�|�Q��L��class�i�@�B�եΡC
 * */

package robotFixing;

import java.awt.Graphics;
import java.awt.Point;
import java.awt.image.BufferedImage;
import java.awt.image.RescaleOp;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import javax.imageio.ImageIO;

public class ImgPlane {
	private int[][] imgPosition; 
	// numbering the chunk of image, from left to right, Top down, start from 0.
	// imgPosition[i][j] = the index of the chunk which put at the location [i][j].
	
	private List<BufferedImage> imgChunks = new ArrayList<BufferedImage>(); // instance of image
	private String tag;
	private int rows;
	private int cols;
	private int[][] targetPattern = null; 
	// catch the information about which chunk is select, 
	
	/** ImgPlane (String pathName, int rows, int cols)
	 * 		Constructor for initializing image plane, original position of image is also
	 * 		initialized here
	 * 
	 *  @param - pathName: the inputed image path
	 *  @param - rows
	 *  @patam - cols 
	 * 
	 * */
	public ImgPlane(String pathName,String tag, int rows, int cols) {
		this.tag = new String(tag);
		this.rows = rows;
		this.cols = cols;
		imgPosition = new int[rows][cols];
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				imgPosition[i][j] = i * rows + j;
			}
		}
		this.imgChunks = loadAndChunkImage(pathName);
	}
	
	/** BufferedImage[] loadAndChunkImage(String pathName)
	 * 		It load the image and cut it in a chunk by inputed numbers of rows and columns
	 * 
	 * @param - pathName: input image
	 * 
	 * */
	private List<BufferedImage> loadAndChunkImage(String pathName) {
		File file = new File(pathName);
		FileInputStream fis = null;
		try {
			fis = new FileInputStream(file);
		} catch (FileNotFoundException ex) {
			System.out.println("Image file not found");
			ex.printStackTrace();
		}
		
		BufferedImage image = null;
		try {
			image = ImageIO.read(fis);
			
		} catch (IOException ex) {
			System.out.println("Image file loading fail");
			ex.printStackTrace();
		}
		
		int chunkWidth = image.getWidth() / this.cols;
		int chunkHeight = image.getHeight() / this.rows;
		List<BufferedImage> imgChunks = new ArrayList<BufferedImage>();

		for (int r = 0; r < this.rows; r++) {
			for (int c = 0; c < this.cols; c++) {
				imgChunks.add(image.getSubimage(c*chunkWidth, r*chunkHeight, chunkWidth, chunkHeight));
			}
		}
		return imgChunks;
	}
	
	public void randomImgPosition() {
		int shuffleTime = (int)((Math.random())* 500);
		Point p1 = new Point();
		Point p2 = new Point();
		
		for(int i = 0; i < shuffleTime; i++) {
			p1.x = (int)((Math.random())* this.rows);
			p1.y = (int)((Math.random())* this.cols);
			p2.x = (int)((Math.random())* this.rows);
			p2.y = (int)((Math.random())* this.cols);
			this.exchangeImgPosition(p1, p2);
		}
	}
	
	public void setTargetPattern(int[][] target) {
		this.targetPattern = target; 
	}
	
	public void exchangeImgPosition(Point p1, Point p2){ //Point(row No., cols No.)
		int temp = imgPosition[p1.x][p1.y];
		imgPosition[p1.x][p1.y] = imgPosition[p2.x][p2.y];
		imgPosition[p2.x][p2.y] = temp;
	}
	
	public BufferedImage adaptImg(BufferedImage img, float scaleFactor){
		BufferedImage bi = new BufferedImage(img.getWidth(), img.getHeight(), BufferedImage.TYPE_INT_RGB);
		Graphics g = bi.createGraphics();
		g.drawImage(img, 0, 0, null);
		RescaleOp op = new RescaleOp(scaleFactor, 0, null);
		bi = op.filter(bi, null);
		return bi;
	}
	
	public int[][] getImgPosition() {
		return this.imgPosition;
	}
	
	public List<BufferedImage> getImgChunk() {
		return this.imgChunks;
	}
	
	public int[][] getTargetPattern() {
		return this.targetPattern;
	}
	
	public String getTag(){
		return this.tag;
	}
	
	public int getChunkWidth(){
		return imgChunks.get(0).getWidth(); 
	}
	
	public int getChunkHeight(){
		return imgChunks.get(0).getHeight();
	}
	
	/*
	 * TA's treasure, something may be useful here
	 * */
	public void setImgPosition(int[][] input) { // set image Position from other player 
		this.imgPosition = input;
	}
}
