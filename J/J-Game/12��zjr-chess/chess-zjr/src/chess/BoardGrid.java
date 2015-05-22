package chess;

import java.awt.*;


import javax.swing.*;
import javax.swing.border.Border;

public class BoardGrid extends JPanel
{
	
	public BoardGrid(BoardLocation loc)
	{
		inBoard = true;
		location = loc;
		
		if (location.getX() + location.getY() <= 2)
			return;
		
		if (location.getY()-location.getX() >= 5) 
			return;
		
		if (location.getX()-location.getY() >= 11)
			return;
		
		if (location.getX() + location.getY() >= 18)
			return;
		
		if ((location.getX() + location.getY()) % 2 == 0)
			this.setBackground(new Color(255,200,142));
		else
			this.setBackground(new Color(187,94,0));
		
		this.setBorder(BorderFactory.createEtchedBorder());
	}
	
	// for captured pieces
	public BoardGrid(int pieceIndex)
	{
		inBoard = false;
		location = new BoardLocation(-1, pieceIndex);
		this.setVisible(false);
	}
	
	public void paintComponent(Graphics g){
	      super.paintComponent(g);
	      g.drawImage(image,0,0,50,29,null);
	      
	}     
	
	public void setImage(Image img)
	{
		image=img;
	}

	public void setLocation(BoardLocation loc)
	{
		location = loc;
		if ((location.getX() + location.getY()) % 2 == 1)
			this.setBackground(Color.LIGHT_GRAY);
		else
			this.setBackground(Color.ORANGE);
	}
	
	public void setPiece(Piece aPiece)
	{
		piece = aPiece;
		image = piece.getImage();
		this.setVisible(true);
		
		/*
		Resources.trace(piece.getPlayer() + piece.getName()
				+ "(" + location.getX() + "," + location.getY() + ")");
		 */
	}
	
	public void setEmpty()
	{
		image = null;
		piece = null;
		if (!inBoard)
			this.setVisible(false);
	}

	public void setSelected()
	{
		this.setBorder(BorderFactory.createLineBorder(Color.BLUE, 5));
	}
	
	public void setUnSelected()
	{
		this.setBorder(null);
	}
	
	public BoardLocation getlocation()
	{
		return location;
	}
	
	public Image getImage()
	{
		return image;
	}
	
	public Piece getPiece()
	{
		return piece;
	}
	
	private Image image;
	private BoardLocation location;
	private Piece piece;
	private boolean inBoard;
}
