package chess;

import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import javax.swing.JButton;


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
		
		BoardGrid grid = (BoardGrid)event.getSource();
		
		play.handleClick(grid);

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
	
	private ChessPlay play;
	private JButton btnRestart;
	private JButton btnQuit;
}
