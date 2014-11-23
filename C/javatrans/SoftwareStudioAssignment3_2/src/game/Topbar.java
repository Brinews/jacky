package game;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Font;
import java.awt.GridLayout;
import java.awt.Rectangle;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.util.Stack;

import javax.imageio.ImageIO;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JPanel;

public class Topbar extends JPanel{

	private Stack<JLabel> lives = new Stack<JLabel>();
	private int score = 0;
	
	private BufferedImage img;
	private JLabel scoreLabel = null;
	
	public Topbar(Rectangle bounds, int lifecount) {
		/*TODO: Initialize the topbar*/
		
		this.setBounds(0, 0, bounds.width, bounds.height);
		this.setBackground(Color.black);
		this.setForeground(Color.white);
		
		this.setFocusable(false);
		
		//this.setLayout(new BorderLayout());
		
		try {
			img = ImageIO.read(this.getClass().getResource("/res/heart.png"));
		}
		catch (IOException e){
			e.printStackTrace();
		}
		
		for (int i = 0; i < lifecount; i++)
		{
			JLabel lab = new JLabel();
			ImageIcon ii = new ImageIcon();
			ii.setImage(img);
			lab.setIcon(ii);
			
			lives.add(lab);
			
			//lab.setBounds(i*35, 0, 35, 35);
			this.add(lab);
		}
		
		scoreLabel = new JLabel("Score:0");
		scoreLabel.setForeground(Color.white);
		
		//scoreLabel.setBounds(500, 0, 100, 40);
		this.add(scoreLabel);
	}
	
	/*
	 * implement all labels and methods you need to do about "score" and "life"
	 */
	
	public void AddScore(int sc)
	{
		score += sc;
		
		System.out.println(score+":::");
		
		this.scoreLabel.setText("Score:"+score);
		this.revalidate();
	}
	
	public void SubLives()
	{
		if (lives.size() > 0)
		{
			JLabel lab = lives.pop();
			this.remove(lab);
			this.revalidate();
		}
	}
}
