package chess;

import java.awt.Color;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JLabel;

import chess.Resources.Player;


public class MouseListener extends MouseAdapter{
	public MouseListener()
	{

	}
	public void mouseClicked(MouseEvent event)
	{
		if (event.getSource() == btnRestart)
		{
			play.restart();
			return;
		}
		if (event.getSource() == btnQuit)
		{
			System.exit(0);
		}
		if (event.getSource() == whiteLabel) {
			whiteLabel.setBorder(BorderFactory.createLineBorder(Color.RED, 5));
			blackLabel.setBorder(BorderFactory.createLineBorder(Color.gray, 5));
			if (!play.inPlay())
				play.setPlayer(Player.WHITE);
			return;
		}
		if (event.getSource() == blackLabel) {
			blackLabel.setBorder(BorderFactory.createLineBorder(Color.RED, 5));
			whiteLabel.setBorder(BorderFactory.createLineBorder(Color.gray, 5));
			if (!play.inPlay()) {
				play.computerMove(Player.WHITE);
				play.setPlayer(Player.BLACK);
			}
			return;
		}
		
		BoardGrid grid = (BoardGrid)event.getSource();
		
		try {
			play.handleClick(grid);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}
	
	public void mousePressed(MouseEvent event)
    {
		
    }
	
	public void setPlay(ChessPlay aplay)
	{
		play = aplay;
	}
	
	public void setRestartButton(JButton restart)
	{
		btnRestart = restart;
	}
	
	public void setQuitButton(JButton quit)
	{
		btnQuit = quit;
	}
	
	public void setLabel(JLabel w, JLabel b){
		whiteLabel = w;
		blackLabel = b;
	}
	
	private ChessPlay play;
	private JButton btnRestart;
	private JButton btnQuit;
	private JLabel whiteLabel;
	private JLabel blackLabel;
}
