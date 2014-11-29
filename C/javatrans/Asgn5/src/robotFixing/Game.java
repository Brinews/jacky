package robotFixing;

import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.Rectangle;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;

import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;

public class Game extends JFrame implements ActionListener,Runnable {

	private static final long serialVersionUID = 1L;
	
	private Rectangle bounds = new Rectangle(1300,800);
	private Rectangle stageBounds = new Rectangle(1250,800);
	private int rows = 4;
	private int cols = 4;
	private int lives = 3;
	private int round = 0;
	private int stageNumber =0;
	
	private Thread gameThread = null;
	private final static int DELAY = 20;
	
	private Model model;
	private GameStage currentStage;
	private Queue<GameStage> stages = new LinkedList<GameStage>();
	private JLabel gameOver;
	private JPanel lifePanel;
	private List<JLabel> lifeLabels = new ArrayList<JLabel>();
	
	private PrintWriter wtr = null;
	private BufferedReader rdr = null;
	private String role = "tagger";
	private int currentImageNo = -1; /* record current image */
	private boolean puzzlerBegin = false;
	private boolean selectedSendOver = false;
	private boolean targetSendOver = false;
	private boolean resultSendOver = false;
	
	private int[] selectedChunks = null;

	public Game() throws IOException {
		super();
		this.setVisible(false);
		this.setBounds(this.bounds);
		this.setLayout(null);
		this.setResizable(false);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		lifePanel = new JPanel();
		initLifePanel();
		
		this.model = new Model("res/assignment4TestData/assignment4TestData.txt");
		
		this.addNewStage();
	}
	
	public Game(String role, PrintWriter wtr, BufferedReader rdr) throws IOException {
		
		super();
		this.setVisible(false);
		this.setBounds(this.bounds);
		this.setLayout(null);
		this.setResizable(false);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		lifePanel = new JPanel();
		initLifePanel();
		
		this.model = new Model("res/assignment4TestData/assignment4TestData.txt");
		
		this.role = role;
		this.wtr = wtr;
		this.rdr = rdr;
		
		this.addNewStage();
	}
	
	private void initLifePanel(){
		this.lifePanel.setLayout(new FlowLayout(FlowLayout.CENTER,0,5));
		this.lifePanel.setBounds(0,0,50,800);
		this.lifePanel.setVisible(true);
		this.lifePanel.setBackground(Color.orange);
		this.lifePanel.setOpaque(true);
		this.lifePanel.removeAll();
		
		for(int i=0;i<this.lives;++i){
			JLabel robotLife = new JLabel(new ImageIcon("res/robot_head-128.png"));
			robotLife.setBounds(0, i*50, 50, 50);
			lifeLabels.add(robotLife);
			lifePanel.add(robotLife);
		}
		
		this.add(lifePanel);
	}
	
	public void start() {
		if (this.gameThread == null) {
            this.gameThread = new Thread(this);
        }
		this.gameThread.start();
		this.setVisible(true);
	}
	
	public void syncGameStage(int imageId, int rows, int cols)
	{
		System.out.println("sync image id=" + imageId +" "+ rows+" " + cols);
		
		this.rows = rows;
		this.cols = cols;
		
		currentImageNo = imageId;
		GameStage stage = new GameStage(stageBounds, 
				model.getRandomPlane(this.rows, this.cols, this.currentImageNo),
				rows, cols,++stageNumber, role);
		this.currentImageNo = model.getImageID();
		stage.setVisible(true);
		stages.add(stage);
		this.nextStage();
		
		puzzlerBegin = true;
	}
	
	public void syncSelectedChunks(int[] selected)
	{
		selectedChunks = new int[selected.length];
		
		for (int i = 0; i< selected.length; i++)
		{
			selectedChunks[i] = selected[i];
			
			//System.out.print(selected[i] + " ");
		}
		//System.out.print("\n");
		
		
		this.currentStage.setSelectAreaChunk(selectedChunks);
		this.currentStage.setSelcetAreaDone(true);
		this.currentStage.setPlayPlaneAble(true);
		
		this.currentStage.setSelectAreaChunk(selectedChunks);
		this.currentStage.setSelectAreaChunk(selectedChunks);
		this.currentStage.setSelectAreaChunk(selectedChunks);
		this.currentStage.setSelectAreaChunk(selectedChunks);
	}
	
	public void syncGameTargetPattern(int[] targets)
	{
		this.currentStage.setPaneTargetPattern(targets);
		this.currentStage.setPaneTargetPattern(targets);
		this.currentStage.setPaneTargetPattern(targets);
	}
	
	public void syncGameResult(boolean flag)
	{
		this.currentStage.setWinFlag(flag);
		this.currentStage.setWinFlag(flag);
		this.currentStage.setWinFlag(flag);
		
		this.currentStage.robotSuccessfulAnimation(round);
		this.currentStage.robotSuccessfulAnimation(round);
		this.currentStage.robotSuccessfulAnimation(round);
		this.currentStage.robotSuccessfulAnimation(round);
	}
	
	public int getCurrentImageNo()
	{
		return currentImageNo;
	}
	
	private void addNewStage() {
		
		if (this.role.equals("tagger"))
		{
			currentImageNo = -1;
			
			GameStage stage = new GameStage(stageBounds, 
					model.getRandomPlane(this.rows, this.cols, this.currentImageNo), 
					rows, cols,++stageNumber, role);

			this.currentImageNo = model.getImageID();
			stage.setVisible(true);
			stages.add(stage);
			this.nextStage();
			
			if (wtr != null) {
				wtr.println("IMAGE-SYNC " + currentImageNo + " " + rows + " " + cols);
				wtr.flush();
			}
		}
		
		puzzlerBegin = false;
		selectedSendOver = false;
		targetSendOver = false;
		resultSendOver = false;
	}
	
	private void nextStage() {
		this.currentStage = this.stages.poll();
		this.initLifePanel();
		this.add(currentStage);
		this.currentStage.requestFocus();
	}
	
	private void gameOver(){
		this.gameOver = new JLabel(new ImageIcon("res/game_over.png"));
		this.gameOver.setBounds(this.bounds);
		this.gameOver.setBackground(Color.black);
		this.gameOver.setOpaque(true);
		this.setVisible(true);
		this.add(gameOver);
		this.repaint();
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		
	}

	@Override
	public void run() {
		long t = System.currentTimeMillis();
		while (Thread.currentThread() == this.gameThread) {
			if (role.equals("puzzler") && puzzlerBegin == false) continue;
            try { 
            	if (this.lives > 0) {
            		
            		/* tagger is ok, send position info to puzzler */
            		if (this.role.equals("tagger") && this.currentStage.isSelectAreaDone()
            				&& !selectedSendOver) {
            			
            			selectedSendOver = true;
            			
            			String selStr = "SELECT-SYNC ";
            			int[] selected = this.currentStage.getSelectedChunk();
            			for (int i = 0; i < selected.length; i++)
            			{
            				selStr = selStr + selected[i] + " ";
            			}
            			
            			wtr.println(selStr);
            			wtr.flush();
            		}
            		
            		/* tagger send target info to puzzler */
            		if (this.role.equals("tagger") && this.currentStage.isSelectAreaDone() &&
            				!targetSendOver) {
            			
            			targetSendOver = true;
            			String tarStr = "TARGET-SYNC ";
            			int[][] target = this.currentStage.getSelectedTarget();
            			
            			for (int i = 0; i < target.length; i++) {
            				for (int j = 0; j < target[i].length; j++) {
            					tarStr = tarStr + target[i][j] + " ";
            				}
            			}
            			
            			wtr.println(tarStr);
            			wtr.flush();
            		}
            		
            		if (this.role.equals("puzzler"))
            		{
            			if (this.currentStage.isPlayPlaneDone()) {
            				this.currentStage.submitStage();
            				
            				/* send winFlag to tagger */
            				if (!resultSendOver) {
            					String state = "RESULT-SYNC ";
            					if (this.currentStage.hasLose())
            						state += "0";
            					else state += "1";
            					
            					wtr.println(state);
            					wtr.flush();
            					
            					resultSendOver = true;
            				}
            			}
            		}
            		
            		if(this.currentStage.isSelectAreaDone()) {
                		//this.currentStage.setPlayPlaneAble(true);
                		//if(this.currentStage.isPlayPlaneDone()) this.currentStage.submitStage();
                    	
                    	if(this.currentStage.hasLose()) {
                    		--this.lives;
                    		while (!this.currentStage.hasDoneRobotSummonAnimation() && round < 20) {
                    			this.currentStage.robotSummonAnimationFrame();
                    			Thread.currentThread().sleep(900/(++this.round));
                    		} 
                    		this.round=0;
        					this.remove(this.currentStage);
        					if (this.rows > 3) {this.rows--;}
        					if (this.cols > 3) {this.cols--;}
        					
        					this.addNewStage();
                    	} else if(currentStage.hasWon()){
                    		while (!this.currentStage.hasDoneRobotSummonAnimation() && round < 20) {
                    			this.currentStage.robotSummonAnimationFrame();
                    			Thread.currentThread().sleep(900/(++this.round));
                    		} 
                    		this.round=0;
                    		this.remove(this.currentStage);
                    		if(this.rows<8){++this.rows;}
                    		if(this.cols<8){++this.cols;}
                    		
                    		this.addNewStage();
                    		
                    	} else if(!this.currentStage.isTimeUp()){
                    		this.currentStage.timeRunning();
                    	} //else;
                	}
            	} else {
            		this.remove(this.currentStage);
            		this.remove(this.lifePanel);
            		this.gameOver();
            		break;
            	}
            	 	
                this.repaint();
                t += DELAY;
                Thread.sleep(Math.max(0, t - System.currentTimeMillis()));
            } catch (InterruptedException e) {
            	break;
			} catch (IllegalArgumentException e) {
				e.printStackTrace();
			}
        }
	}
}
