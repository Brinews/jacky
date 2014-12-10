package edu;

import java.awt.Canvas;
import java.awt.Color;
import java.awt.Graphics;

public class GaussCaves extends Canvas {
	
	private GaussPoints gsPoints = null;
	private Color[] myColor = new Color[3];
	
	private Color nowColor = Color.BLACK;
	
	private static int offsetX = 250;
	private static int offsetY = 250;
	private int fillWays = 0;

	public GaussCaves(GaussPoints gps){
		
		gsPoints = gps;
		
		myColor[0] = Color.RED;
		myColor[1] = Color.BLUE;
		myColor[2] = Color.GREEN;
		
		setSize(500, 500);
		setBackground(Color.white);
		setBounds(0, 0, 500, 500);
		
		setVisible(true);
	}
	
	/* for repaint */
	public void paint(Graphics g) {
		
		g.setColor(Color.BLACK);
		
		/* clear for repaint */
		g.clearRect(0, 0, 500, 500);
		
		//paint the X-Y axes
		g.drawLine(10, 250, 490, 250);
		g.drawLine(250, 10, 250, 490);
		
		g.setColor(Color.DARK_GRAY);
		g.drawString("X+250", 465, 250);
		g.drawString("Y+250", 250, 10);
		g.drawString("-250", 0, 250);
		g.drawString("-250", 250, 490);
		
		g.setColor(Color.BLUE);
		
		//paint the points
		double x, y;
		for (int i = 0; i < gsPoints.getSize(); i++) {
			x = gsPoints.getPointXAt(i);
			y = gsPoints.getPointYAt(i);
			
			x = offsetX + x;
			y = offsetY - y;
			
			// calculate relative position
			if (fillWays == 0) 
				g.fillOval((int)x, (int)y, 5, 5);
			else if (fillWays == 1) {
				g.fillRect((int)x, (int)y, 5, 5);
			}
			else if (fillWays == 2) {
				g.drawString("*", (int)x, (int)y);
			}
			else if (fillWays == 3) {
				g.drawString("#", (int)x, (int)y);
			}
			else if (fillWays == 4)
				g.drawString("o", (int)x, (int)y);
			                         
		}
		
		//paint the line
		double lx = gsPoints.getLineOffsetX();
		double ly = gsPoints.getLineOffsetY();
		
		g.setColor(nowColor);
		g.drawLine((int)(lx+offsetX), offsetY-0, 0+offsetX, (int)(offsetY-ly));
		
		//extend the line when x=-250 and x=250
		int xMin = -250, xMax = 250;
		int yMin = (int)gsPoints.getNewLineY(xMin);
		int yMax = (int)gsPoints.getNewLineY(xMax);
		
		g.drawLine((int)(xMin+offsetX), (int)(offsetY-yMin), (int)(lx+offsetX), offsetY-0);
		g.drawLine(0+offsetX, (int)(offsetY-ly), xMax+offsetX, (int)(offsetY-yMax));
	}

	public void update(Graphics g) {
		
		g.clearRect(0, 0, 500, 500);
		paint(g);
	}
	
	/* set the brush color */
	public void setColor(int i) {
		
		if (i < 0 || i > 2) return;
		
		nowColor = myColor[i];
	}
	
	/* set point shape */
	public void setFillWays(int w)
	{
		if (w < 0 || w > 4) return;
		
		fillWays = w;
	}
}
