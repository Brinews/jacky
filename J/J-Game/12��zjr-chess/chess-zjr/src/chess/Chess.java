package chess;

import java.awt.BorderLayout;
import java.awt.GridLayout;



import javax.swing.*;

import java.awt.*;


public class Chess
{
	public static void main(String args[])
	{
		EventQueue.invokeLater(new Runnable() 
		{
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				JFrame frame = new JFrame("Camelot GAME");
				frame.setSize(840,746);
				frame.setLayout(null);
				
				MouseListener listener = new MouseListener();
				
				JTextArea traceText = new JTextArea();
				traceText.setEditable(false);
				traceText.setLineWrap(true);
				traceText.setText("The move record.\n");
				
				JLabel whitePlayer = new JLabel("WHITE player", 0);
				JLabel blackPlayer = new JLabel("BLACK player", 0);
				
				JTextArea hintText = new JTextArea();
				hintText.setEditable(false);
				hintText.setLineWrap(true);
				hintText.setText("The hint message.\n");
				
				JPanel playerPanel = new JPanel();
				playerPanel.setLayout(new GridLayout(1, 2));
				playerPanel.add(whitePlayer);
				playerPanel.add(blackPlayer);
				
				JPanel tracePanel = new JPanel();
				tracePanel.setLayout(new GridLayout(1, 2));
				tracePanel.add(new JScrollPane(traceText));
				tracePanel.add(new JScrollPane(hintText));
				
				feedBack feedback = new feedBack(whitePlayer, blackPlayer, traceText, hintText);

				ChessBoard board = new ChessBoard(listener);
				ChessBoard blackOutsideBoard = new ChessBoard();
				ChessBoard whiteOutsideBoard = new ChessBoard();
				
				ChessPlay play = new  ChessPlay(board, blackOutsideBoard, whiteOutsideBoard, feedback);
				play.initialise();
				listener.setPlay(play);
				
				JPanel buttonPanel = new JPanel();
				buttonPanel.setLayout(new GridLayout(1, 2));
				JButton btnRestart = new JButton("Restart");
				btnRestart.addMouseListener(listener);
				JButton btnQuit = new JButton("Quit");
				btnQuit.addMouseListener(listener);
				buttonPanel.add(btnRestart, BorderLayout.CENTER);
				buttonPanel.add(btnQuit, BorderLayout.CENTER);
				
				listener.setRestartButton(btnRestart);
				listener.setQuitButton(btnQuit);
				
				playerPanel.setBounds(0, 0, 800, 50);
				tracePanel.setBounds(0, 50, 800, 150);
				board.setBounds(0, 200, 400, 406);
				blackOutsideBoard.setBounds(400, 200, 400, 100);
				whiteOutsideBoard.setBounds(400, 400, 400, 100);
				buttonPanel.setBounds(0, 620, 800, 50);
				
				frame.add(playerPanel);
				frame.add(tracePanel);
				frame.add(board);
				
				frame.add(blackOutsideBoard);
				frame.add(whiteOutsideBoard);
				
				frame.add(buttonPanel);
				frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
				frame.setVisible(true);
			}
		});
	}
}