import javax.swing.*;
import javax.swing.border.*;
import java.awt.*;
import java.awt.event.*;
import java.util.List;
import java.util.ArrayList;

import java.rmi.*;

public class BuyerPanel extends JPanel {
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

	JTextField jtfKey = null;
	JTextField jtfPrice = null;
	JButton jtfFilter = null;

	public BuyerPanel(IRmiService rmiSer) {

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
		addButton("����б�", setstate);

		topPanel.add(new JLabel("ģ������:"));
		topPanel.add(new JLabel("����"));
		jtfKey = new JTextField("", 10);
		topPanel.add(jtfKey);

		topPanel.add(new JLabel("(��)�۸�"));
		jtfPrice = new JTextField("", 10);
		topPanel.add(jtfPrice);
		topPanel.add(new JLabel("(>x��<x)"));


		jtfFilter = new JButton("ɸѡ");
		topPanel.add(jtfFilter);
		jtfFilter.addActionListener(new FilterAction());
		
		//addButton("���ά��", setstate);

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

	public void hideViewPanel(int f) {
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

			if (input.equals("����б�"))
				panelID = 1;
			if (input.equals("���ά��"))
				panelID = 2;

			/***
			 *repaint
			 */
			hideViewPanel(panelID);
		}
	}

	private class FilterAction implements ActionListener {
		public void actionPerformed(ActionEvent event) {
			String key = jtfKey.getText();
			String price = jtfPrice.getText();
			mlp.setFilter(key, price);
		}
	}
}
