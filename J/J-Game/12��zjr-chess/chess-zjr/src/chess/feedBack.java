package chess;

//import java.awt.GridLayout;

import java.awt.Color;

import javax.swing.BorderFactory;
import javax.swing.JLabel;
import javax.swing.JTextArea;
import chess.Resources.Player;

public class feedBack
{
	public feedBack(JLabel white, JLabel black, JTextArea trace, JTextArea hint)
	{
		whitePlayer = white;
		blackPlayer = black;
		traceText = trace;
		hintText = hint;
		step = 0;
	}
	
	public void clear()
	{
		step = 0;
		traceText.setText("The move record.\n");
	}
	
	public void trace(Player player, Piece piece, BoardLocation origin, BoardLocation destination)
	{
		if (player == Player.WHITE)
		{
			step ++;
			traceText.append(step + ". " + piece.getCharacter() + " " + origin.toString() + "-" + destination.toString() + "\t");
		}
		else
		{
			traceText.append(piece.getCharacter() + " " + origin.toString() + "-" + destination.toString() + "\n");
		}
	}
	
	public void Hint(String str)
	{
		hintText.append(str + "\n");
	}
	
	public void showTurn(Player player)
	{
		if (player == Player.WHITE)
		{
			whitePlayer.setBorder(BorderFactory.createLineBorder(Color.RED, 5));
			blackPlayer.setBorder(null);
			hintText.append("WHITE player turn.\n");
		}
		else if (player == Player.BLACK)
		{
			whitePlayer.setBorder(null);
			blackPlayer.setBorder(BorderFactory.createLineBorder(Color.RED, 5));
			hintText.append("BLACK player turn.\n");
		}
		else
		{
			whitePlayer.setBorder(null);
			blackPlayer.setBorder(null);
		}
	}
	
	private int step = 0;
	private JLabel whitePlayer;
	private JLabel blackPlayer;
	private JTextArea traceText;
	private JTextArea hintText;
}