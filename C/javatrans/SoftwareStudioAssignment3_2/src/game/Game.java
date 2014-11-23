package game;

import java.awt.Color;
import java.awt.Rectangle;
import java.util.LinkedList;
import java.util.Queue;

import javax.swing.JFrame;

import model.Model;
import model.AbstractSentence;

import java.awt.BorderLayout;

public class Game extends JFrame implements Runnable {
	
	private final static int STAGE_AMOUNT = 10;
	private Rectangle bounds = new Rectangle(1000,700);
	private int maxLife = 3;
	private Topbar topbar = new Topbar(new Rectangle(bounds.width, 40), maxLife);
	private Thread gameThread = null;
	private final static int DELAY = 20;
	private Model model = new Model();
	private GameStage currentStage;
	/* Create a Queue called "stages" to put game stage in */
	

	private LinkedList<GameStage> stages = new LinkedList<GameStage>();
	private int stagePassed;
	
	/*Initialize everything you need in this constructor.*/
	public Game() {
		super();
		this.setVisible(false);
		this.setBounds(this.bounds);
		this.setLayout(null);
		this.setResizable(false);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		this.setLayout(new BorderLayout(0,0));
		getContentPane().add(topbar, BorderLayout.NORTH);
		
		stagePassed = 0;
		this.produceStages(STAGE_AMOUNT);
		this.nextStage();
		
		this.currentStage.requestFocus();
	}
	
	public void start() {
		if (this.gameThread == null) {
            this.gameThread = new Thread(this);
        }
		this.gameThread.start();
		this.setVisible(true);
	}
	
	private void nextStage() {
		if (this.stages.size() <= 0) 
		{
			currentStage = null;
			return;
		}
		this.currentStage = this.stages.poll();
		this.currentStage.requestFocus();
		
		System.out.println("TYPE:"+currentStage.getSentence().getName());
	}
	
	/**
	 * void produceStages(int num)
	 * 		Generate the stages before we start gaming.
	 * 
	 * @input num: number of stages that user wants to generate
	 * 
	 * */
	private void produceStages(int num) {
		/*implement your procedures here*/
		
		stages.clear();
		
		for (int i = 0; i < num ; i++) {
			AbstractSentence as = model.RandomSentence();
			if (as != null)
			{
				GameStage sg = new GameStage(new Rectangle(bounds.width, bounds.height-40), as, 10);
				stages.add(sg);
				
				this.add(sg, BorderLayout.CENTER);
			}
		}
	}
	
	@Override
	public void run() {
		long t = System.currentTimeMillis();
		while (Thread.currentThread() == this.gameThread) {
            try {
            	/*
    			 * Something is missing!!
    			 * 
    			 * Add your code to finish remaining methods that control the game process, such as
    			 * checking whether the player pass the stage or not, win the game or not etc.
    			 * 
    			 * */
            	//System.out.println(currentStage.getSrcChi());
            	
            	if (currentStage != null) {
            		
            		currentStage.requestFocus();

            		if (currentStage.getStageScore() != 0)
            			System.out.print("NOW SCORE:" + currentStage.getStageScore());
            		
            		if (currentStage.getStageScore() == 10) {
            			// passed one PC stage
            			
            			System.out.println("success.");
            			
            			this.topbar.AddScore(10);
            			this.currentStage.openDoor();
            			
            			this.currentStage.setVisible(false);
            			this.nextStage();

            			stagePassed++;
            		}
            		else if (currentStage.getStageScore() == -2){
            			// passed one MT stage
            			this.currentStage.openDoor();
            			
            			this.currentStage.setVisible(false);
            			this.nextStage();
            			
            			stagePassed++;
            		}
            		else if (currentStage.getStageScore() == -1) {
            			// failed one PC stage
            			this.topbar.SubLives();
            			this.currentStage.setStageScore(0);
            			maxLife--;
            		}
            		
            		//judge result
            		if (maxLife <= 0) {
                		//lost
            			if (currentStage != null){
                			
                			for (GameStage gs : stages){
                				gs.setVisible(false);
                			}
                			this.currentStage.setVisible(false);
                			this.currentStage = null;
                		}
                		
                		this.getContentPane().add(new LostScreen(), BorderLayout.CENTER);
                	}
                	
                	if (stagePassed >= 10) {
                		//win
                		System.out.println("Success");
                		if (currentStage != null){
                			
                			for (GameStage gs : stages){
                				gs.setVisible(false);
                			}
                			this.currentStage.setVisible(false);
                			this.currentStage = null;
                		}
                		
                		WinScreen ws = new WinScreen();
                		ws.setVisible(true);
                		this.setBackground(Color.black);
                		this.getContentPane().add(ws, BorderLayout.CENTER);
                	}
            	}

            	this.repaint();
            	t += DELAY;
            	Thread.sleep(Math.max(0, t - System.currentTimeMillis()));
            }
            catch (InterruptedException e) {
            	break;
            } catch (IllegalArgumentException e) {
            	e.printStackTrace();
            }
		}
	}

}
