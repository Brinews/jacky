package game;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.awt.geom.AffineTransform;

import javax.imageio.ImageIO;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.border.LineBorder;

public class WinScreen extends JPanel {

	private BufferedImage image;
	private Image img;
	
	public WinScreen(){
		this.setLayout(null);
		this.setVisible(true);
		try {
			this.image = ImageIO.read(this.getClass().getResource("/res/treasure.jpg"));
			this.setBounds(0, 0, 1000, 700);
			//this.setBounds(100, 200, this.image.getWidth(),
			//		this.image.getHeight());
			
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	@Override
	public void paintComponent(Graphics g) {
		Graphics2D g2d = (Graphics2D) g;
		g2d.drawImage(image, 0, 0, 1000, 700, null);
	}
}

class LostScreen extends JPanel {
	private BufferedImage image;
	public LostScreen(){
		this.setLayout(null);
		this.setVisible(true);
		try {
			this.image = ImageIO.read(this.getClass().getResource("/res/gameover.gif"));
			this.setBounds(0, 0, 1000, 700);
			
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	@Override
	public void paintComponent(Graphics g) {
		Graphics2D g2d = (Graphics2D) g;
		//g2d.drawImage(image, new AffineTransform(), this);
		g2d.drawImage(image, 0, 0, 1000, 700, null);
	}
}
