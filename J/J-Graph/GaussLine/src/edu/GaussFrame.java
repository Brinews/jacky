package edu;

import java.awt.Dimension;

import javax.swing.JFrame;
import javax.swing.JPanel;

public class GaussFrame {
	
	private GaussPanel controlPanel = null;
	private GaussCaves cavesPanel = null;
	private GaussPoints points = null;
	
	private int FRAMEWIDTH = 700;
	private int FRAMEHEIGHT = 550;
	
	private JPanel mainPanelInit()
	{
		/* main panel prepare */
		JPanel mpanel = new JPanel();
		mpanel.setLayout(null);
		mpanel.setBounds(0, 0, FRAMEWIDTH, FRAMEHEIGHT);
		
		points = new GaussPoints();
		
		/* add painting panel */
		cavesPanel = new GaussCaves(points);
		
		/* add control panel */
		controlPanel = new GaussPanel(points, cavesPanel);
		
		mpanel.add(controlPanel);
		mpanel.add(cavesPanel);
		
		mpanel.setVisible(true);
		
		return mpanel;
	}
	
	public GaussFrame() {
		JFrame mframe = new JFrame();
		
		mframe.setSize(new Dimension(FRAMEWIDTH, FRAMEHEIGHT));
		mframe.setResizable(false);
		
		mframe.setBounds(0, 0, FRAMEWIDTH, FRAMEHEIGHT);
		mframe.setLayout(null);
		
		mframe.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		mframe.add(mainPanelInit());
		
		mframe.setVisible(true);
	}

	public static void main(String[] args) {
		new GaussFrame();
	}
}
