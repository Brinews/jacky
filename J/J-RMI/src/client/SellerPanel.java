import javax.swing.*;
import javax.swing.border.*;
import java.awt.*;
import java.awt.event.*;
import java.util.List;
import java.util.ArrayList;

import java.rmi.*;

public class SellerPanel extends JPanel {
	IRmiService rmiService;

	/* For View Panel Select */
	public int panelID;
	
	/* top control panel */
	public JPanel topPanel;
	
	/* Main view panel */
	public JPanel viewPanel;

	/* For Message Display */
	public JButton display;

	/* view panel class */
	List<JPanel> panelList;

	CarLstPanel mlp;
	CarOptPanel map;

	public SellerPanel(IRmiService rmiSer) {

		rmiService = rmiSer;

		setLayout(null);
		setSize(800, 480);

		panelList = new ArrayList<JPanel>();

		panelID = 0;
		setLayout(new BorderLayout());

		display = new JButton("[...]");
		display.setEnabled(false);
		add(display, BorderLayout.SOUTH);

		ActionListener setstate = new StateAction();

		topPanel = new JPanel();
		viewPanel = new JPanel();

		/***
		 * top control buttons
		 */
		addButton("广告列表", setstate);
		addButton("广告维护", setstate);

		topPanel.setBorder(new TitledBorder(new EtchedBorder(), ""));
		add(topPanel, BorderLayout.NORTH);

		/***
		 * View Panels Init
		 */
		mlp = new CarLstPanel(rmiService);
		map = new CarOptPanel(rmiService);

		panelList.add(mlp);
		panelList.add(map);
		
		viewPanel.add(mlp);
		viewPanel.add(map);

		viewPanel.setBorder(new TitledBorder(new EtchedBorder(), ""));
		hideViewPanel(0);

		add(viewPanel, BorderLayout.CENTER);

		setVisible(true);
	}

	public void hideViewPanel(int f)
	{
		int i = 0;
		for(JPanel p:panelList)
		{
			i++;
			p.setVisible(false);
			if (i == f) {
				p.setVisible(true);
			}
		}
	}

	public void addButton(String label, ActionListener listener) {
		JButton button = new JButton(label);
		button.addActionListener(listener);
		
		topPanel.add(button);
	}

	public void paintComponent(Graphics g) {
		super.paintComponent(g);
	}

	private class StateAction implements ActionListener {
		public void actionPerformed(ActionEvent event) {
			String input = event.getActionCommand();

			if (input.equals("广告列表")) panelID = 1;
			if (input.equals("广告维护")) panelID = 2;

			/*
			 *repaint
			 */
			hideViewPanel(panelID);
		}
	}
}
