package game;

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.geom.AffineTransform;
import java.awt.image.BufferedImage;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.JLabel;

public class Wheel extends JLabel {

	private BufferedImage image;
	private int angle = 0;
	
	public Wheel(Rectangle bounds) {
		
		this.setFocusable(false);
		
		try {
			this.image = ImageIO.read(this.getClass().getResource("/res/wheel.png"));
			int anchorX = (bounds.width - this.image.getWidth()) / 2;
			int anchorY = (bounds.height - this.image.getHeight()) / 2;
			this.setBounds(anchorX, anchorY, this.image.getWidth(),
					this.image.getHeight());
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	@Override
	public void paintComponent(Graphics g) {
		Graphics2D g2d = (Graphics2D) g;
		AffineTransform at = new AffineTransform();
		at.rotate(Math.toRadians(this.angle), image.getWidth()/2, image.getHeight()/2);
		g2d.drawImage(image, at, this);
	}
	
	public void rotateClockwise() {
		this.angle += 10;
	}
	
	public void rotateCounterClockwise() {
		this.angle -= 10;
	}
	

}
