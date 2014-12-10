package edu;

import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.border.TitledBorder;

public class GaussPanel extends JPanel {
	
	private static int PTSMAX = 10;
	
	/* prepare the fields to input points position */
	private JLabel[] ptsLabel = new JLabel[PTSMAX];
	private JTextField[] ptsX = new JTextField[PTSMAX];
	private JTextField[] ptsY = new JTextField[PTSMAX];
	
	private JComboBox colorOption = new JComboBox();
	private JComboBox shapeOption = new JComboBox();
	
	private JButton randControl = new JButton();
	JButton clrBtn = new JButton();
	
	private GaussCaves myCaves = null;
	private GaussPoints gsPoints = null;
	
	public GaussPanel(GaussPoints gps, GaussCaves gcs)
	{
		gsPoints = gps;
		myCaves = gcs;
		
		setBounds(500, 0, 180, 500);
		setLayout(new BorderLayout());
		setBorder(new TitledBorder(""));
		
		InputListener inputAction = new InputListener();
		
		/* rand button */
		
		randControl.setText("Rand Points");
		randControl.setBounds(500, 330, 160, 30);
		randControl.addActionListener(inputAction);
		randControl.setVisible(true);
		
		add(randControl, BorderLayout.NORTH);
		
		JPanel pointsPanel = new JPanel();
		pointsPanel.setLayout(new GridLayout(10, 3));
		
		for (int i = 0; i < PTSMAX; i++) {
			
			ptsLabel[i] = new JLabel();
			
			/* add Labels */
			ptsLabel[i].setText("Point " + i);
			ptsLabel[i].setVisible(true);
			
			pointsPanel.add(ptsLabel[i]);
			
			ptsX[i] = new JTextField();
			
			//add X points input 
			ptsX[i].setColumns(8);
			ptsX[i].setVisible(true);
			
			ptsX[i].addActionListener(inputAction);
			
			pointsPanel.add(ptsX[i]);
			
			ptsY[i] = new JTextField();
			
			//add Y points input 
			ptsY[i].setColumns(8);
			ptsY[i].setVisible(true);
			
			ptsY[i].addActionListener(inputAction);
			
			pointsPanel.add(ptsY[i]);
		}
		
		/* first init */
		gsPoints.generateRandPoints(200);
		fillPointInput();
		
		add(pointsPanel, BorderLayout.CENTER);
		
		JPanel selectPanel = new JPanel();
		selectPanel.setLayout(new BorderLayout());
		
		clrBtn.setText("Clear Points");
		clrBtn.addActionListener(inputAction);
		clrBtn.setVisible(true);
		
		/* add clear button */
		selectPanel.add(clrBtn, BorderLayout.NORTH);
		
		colorOption.addItem("RED");
		colorOption.addItem("BLUE");
		colorOption.addItem("GREEN");
		
		colorOption.setVisible(true);
		
		/* color setting */
		selectPanel.add(colorOption, BorderLayout.CENTER);
		
		/* shape selection */
		shapeOption.addItem("oval");
		shapeOption.addItem("rect");
		shapeOption.addItem("char *");
		shapeOption.addItem("char #");
		shapeOption.addItem("char o");
		
		shapeOption.setVisible(true);
		
		// add shape option 
		selectPanel.add(shapeOption, BorderLayout.SOUTH);
		
		add(selectPanel, BorderLayout.SOUTH);
		
		setVisible(true);
	}
	
	public void fillPointInput()
	{
		for (int i = 0; i < PTSMAX; i++) {
			ptsX[i].setText(""+gsPoints.getPointXAt(i));
			ptsY[i].setText(""+gsPoints.getPointYAt(i));
		}
	}
	
	class InputListener implements ActionListener {
		
		public void actionPerformed(ActionEvent event) {
			
			int flags = 0;
			
			if (event.getSource() == randControl) {
				flags = 1;
				gsPoints.generateRandPoints(490);
				fillPointInput();
			}
			
			if (event.getSource() == clrBtn) {
				flags = 1;
				gsPoints.resetPoints();
				fillPointInput();
			}
			
			for (int i = 0; i < PTSMAX; i++) {
				if (event.getSource() == ptsX[i])
				{
					try {
						double x = Double.parseDouble(ptsX[i].getText());
						gsPoints.setPointXAt(x, i);
						flags = 1;
					}
					catch (Exception e) {
						e.printStackTrace();
					}
				}
				
				if (event.getSource() == ptsY[i]) {
					try {
						double y = Double.parseDouble(ptsY[i].getText());
						gsPoints.setPointYAt(y, i);
						flags = 1;
					}
					catch(Exception e) {
						e.printStackTrace();
					}
				}
			}
			
			/* update caves */
			if (flags == 1) {
				int color = colorOption.getSelectedIndex();
				myCaves.setColor(color);
				
				myCaves.setFillWays(shapeOption.getSelectedIndex());
				
				myCaves.repaint();
			}
		}
	}
}
