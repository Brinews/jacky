package game;

/**
 * The class "GameStage" is an object of an individual stage, several stages will
 * establish a game.
 * All of the method in this class is related to generate UI we use
 * 
 * */

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Vector;

import javax.imageio.ImageIO;
import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.border.Border;
import javax.swing.border.EtchedBorder;
import javax.swing.border.LineBorder;

import model.AbstractSentence;

public class GameStage extends JPanel implements KeyListener {
	/*
	 * While "Keyboard" is no more than a recommend gaming method, others are available
	 * and you can implement yours.
	 * */
	private List<String> chineseSpans = null;
	private List<String> determinedSpans = new ArrayList<String>();
	private Rectangle bounds;
	private Wheel wheelLabel;
	private JLabel engLabel = new JLabel();
	private JPanel wordsPanel = new JPanel();
	private JPanel determinedWordsPanel = new JPanel();
	
	private List<JLabel> srcChiLabel = new ArrayList<JLabel>();
	
	private int level = 0;
	private int cursorIndex = 0;
	private int stageScore = 0;
	private AbstractSentence sentence;
	
	/*We provided some basic things, you can add any other things here.*/
	public GameStage(Rectangle bounds, AbstractSentence sentence, int level) {
		
		this.level = level;
		this.bounds = bounds;
		this.sentence = sentence;
		
		this.setLayout(null);
		
		this.setFocusable(true);
		this.setLocation(0, 40);
		this.setBounds(0, 40, bounds.width, bounds.height);
		
		this.setBackground(Color.gray);
		//this.setBorder(new LineBorder(Color.red));
		
		this.initWordspans();
		this.initWheel();
		
		//addKeyListener(this);
	}

	private void initWheel() {
		this.wheelLabel = new Wheel(this.bounds);
		
		this.wheelLabel.setBounds(320, 150, 500, 500);
		this.wheelLabel.setFocusable(false);
		
		this.add(wheelLabel);
	}
	
	private JLabel addToWordsPanel(String span)
	{
		JLabel lab = new JLabel();
		
		int len = span.length();
		lab.setSize(len*10, 45);
		
		lab.setText(span);
		lab.setOpaque(true);
		lab.setBackground(Color.yellow);
		lab.setForeground(Color.black);

		return lab;
	}

	private void initWordspans() {

		this.initDeterminedWordsPanel();

		this.wordsPanel.setLayout(new FlowLayout(FlowLayout.LEADING, 10, 10));
		this.wordsPanel.setAlignmentX(Component.LEFT_ALIGNMENT);
		this.wordsPanel.setBackground(Color.gray);
		this.wordsPanel.setBounds(0, bounds.height-100, bounds.width, 40);

		this.chineseSpans = this.sentence.getRandomizedChineseSpansWithLevel(this.level);
		for (String span: this.chineseSpans) {
			/*
			 * Something is missing!!
			 * 
			 * Add your code to establish remaining method that put Chinese 
			 * spans on your play board
			 * 
			 * */
			JLabel lab = addToWordsPanel(span);

			this.srcChiLabel.add(lab);
			this.wordsPanel.add(lab);
		}

		this.add(wordsPanel);
		
		this.engLabel.setFocusable(false);
		this.determinedWordsPanel.setFocusable(false);
		this.wordsPanel.setFocusable(true);
		//this.wordsPanel.addKeyListener(this);
		this.addKeyListener(this);
		
		this.selectSpan(this.cursorIndex);
	}

	/*Initiate determinedWordsPanel, you can add something here if needed.*/
	private void initDeterminedWordsPanel() {
		this.engLabel.setAlignmentX(Component.LEFT_ALIGNMENT);
		this.engLabel.setText(this.sentence.getEngSentence());
		this.engLabel.setFont(new Font("Comic Sans MS", Font.PLAIN, 16));
		this.engLabel.setForeground(Color.white);
		
		this.determinedWordsPanel.setAlignmentX(Component.LEFT_ALIGNMENT);
		this.determinedWordsPanel.setLayout(new FlowLayout(FlowLayout.LEADING, 10, 10));
		this.determinedWordsPanel.setOpaque(false);
		
		this.engLabel.setBounds(0, 0, 1000, 20);
		this.add(engLabel);
		this.determinedWordsPanel.setBounds(0, 30, 1000, 30);
		//this.determinedWordsPanel.setBorder(new LineBorder(Color.green));
		this.add(determinedWordsPanel);
	}
	
	/*You can add something here if needed.*/
	private void restart() {
		this.determinedWordsPanel.removeAll();
		this.wordsPanel.removeAll();
		this.determinedSpans.clear();
		this.cursorIndex = 0;
		
		this.srcChiLabel.clear();

		for (String span: this.chineseSpans) {
			/*
			 * Something is missing!!
			 * 
			 * The process here may be close to something you have done in initialization 
			 * 
			 * */
			JLabel lab = addToWordsPanel(span);
			
			this.srcChiLabel.add(lab);
			this.wordsPanel.add(lab);
		}
		
		this.wordsPanel.revalidate();
		this.determinedWordsPanel.revalidate();
		this.selectSpan(this.cursorIndex);
	}
	
	
	/**
	 * void selectSpan(int spanIndex)
	 * 		An display that represent the player is now focusing on specific span 
	 * */
	private void selectSpan(int spanIndex) {
		/*implement your procedures here*/
		
		JLabel lab = this.srcChiLabel.get(spanIndex);
		
		if (lab != null) {
			lab.setBackground(Color.orange);
		}
	}
	
	/**
	 * void unselectSpan(int spanIndex)
	 * 		An display that represent the span which player isn't focus on  
	 * */
	private void unselectSpan(int spanIndex) {
		/*implement your procedures here*/
		
		JLabel lab = this.srcChiLabel.get(spanIndex);
		
		if (lab != null) {
			lab.setBackground(Color.yellow);
		}
	}
	
	private JLabel addToDeterWordsPanel(String span)
	{
		JLabel lab = new JLabel();
		lab.setAlignmentX(Component.CENTER_ALIGNMENT);
		lab.setText(span);
		lab.setBorder(new LineBorder(Color.gray));
		lab.setFont(new Font("", Font.HANGING_BASELINE, 13));
		
		lab.setOpaque(true);
		lab.setForeground(Color.lightGray);
		lab.setBackground(Color.gray);
		
		this.determinedWordsPanel.add(lab);
		
		return lab;
	}
	
	/**
	 *void confirmSpan(int spanIndex)
	 * 		The action to do when a span is being sent to the determined area.
	 **/
	private void confirmSpan(int spanIndex) {
		/*implement your procedures here*/
		
		JLabel lab = this.srcChiLabel.remove(spanIndex);
		this.wordsPanel.remove(spanIndex);
		
		String spanTxt = lab.getText();
		this.determinedSpans.add(spanTxt);
		addToDeterWordsPanel(spanTxt);
		
		this.determinedWordsPanel.revalidate();
		this.wordsPanel.revalidate();
	}
	
	/**
	 * void submit(int spanIndex)
	 * 		The action to do when a sentence is being submitted.
	 **/
	private void submit(int spanIndex) {
		/*implement your procedures here*/
		
		confirmSpan(spanIndex);
		
		if (this.srcChiLabel.size() == 0) {
			
			if (confirmSubmit() == 1)
			{
				this.stageScore += this.sentence.score(this.determinedSpans);
				
				if (this.stageScore == -1)
					this.restart();
				
				System.out.println("SCORE:" + this.stageScore);
			}
			else
			{
				this.restart();
			}
		}
	}
	
	/*
	 * It's a action that when a sentence has been submit,a window will pop out to ask the player
	 * to confirm if he/she really want to submit.
	 */
	private int confirmSubmit() {
		String[] options = {"Cancel", "Yes"};
		int n = JOptionPane.showOptionDialog(this,
										"Are you sure?",
										"Confirmation",
										JOptionPane.OK_CANCEL_OPTION,
										JOptionPane.QUESTION_MESSAGE,
										null,
										options,
										options[1]);
		return n;
	}
	
	/*
	 * Make the frame of "open door animation" 
	 * While animation consist of many static pictures, that we usually call it as frame.
	 * You may implement procedures that generates individual frames of moving door
	 * 
	 * If it's successfully called by run() in game.java, there must have desired animation  
	 */
	public void openDoor() {
	}
	
	public int getStageScore() {
		return this.stageScore;
	}

	@Override
	public void keyPressed(KeyEvent arg0) {
		// TODO Auto-generated method stub
		
		//System.out.println("P:"+arg0.getKeyCode());
		
		if(arg0.getKeyCode()== 37)
		{
			//left
			if (this.cursorIndex > 0)
			{
				this.unselectSpan(this.cursorIndex);
				this.cursorIndex--;
				this.selectSpan(this.cursorIndex);
				
				this.wheelLabel.rotateCounterClockwise();
				this.wheelLabel.repaint();
			}
		}
		
		if (arg0.getKeyCode() == 39)
		{
			//right
			if (this.cursorIndex < this.srcChiLabel.size()-1)
			{
				this.unselectSpan(this.cursorIndex);
				this.cursorIndex++;
				this.selectSpan(this.cursorIndex);
				
				this.wheelLabel.rotateClockwise();
				this.wheelLabel.repaint();
			}
		}
		
		if (arg0.getKeyCode() == 10)
		{
			this.submit(this.cursorIndex);
			if (this.srcChiLabel.size() > 0)
			{
				this.cursorIndex = 0;
				this.selectSpan(this.cursorIndex);
			}
		}
	}
	
	public void setStageScore(int sc){
		this.stageScore = sc;
	}
	
	public String getSrcChi()
	{
		String ret = "";
		for (JLabel lab : this.srcChiLabel)
			ret += lab.getText() + ":";
		return ret;
	}
	
	public AbstractSentence getSentence(){
		return sentence;
	}

	@Override
	public void keyReleased(KeyEvent arg0) {
		// TODO Auto-generated method stub
	}

	@Override
	public void keyTyped(KeyEvent arg0) {
		// TODO Auto-generated method stub
	}

}
