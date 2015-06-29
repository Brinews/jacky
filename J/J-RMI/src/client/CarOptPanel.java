import javax.swing.*;
import javax.swing.event.*;
import java.awt.*;
import java.awt.event.*;
import java.sql.*;

import java.rmi.Remote;

class CarOptPanel extends JPanel
{
	private JButton mesg;
	private JPanel panel;
	private JPanel btnPanel;

	JTextField jtfNo;
	JTextField jtfName;
	JTextField jtfPics;
	JTextField jtfPrice;

	IRmiService rmiService = null;

	public CarOptPanel(IRmiService iRmi)
	{
		rmiService = iRmi;

		setLayout(new BorderLayout());
		setSize(800, 320);

		mesg = new JButton("  ");
		mesg.setEnabled(false);
		add(mesg, BorderLayout.NORTH);

		DocumentListener listener = new KeyFieldListener();

		panel = new JPanel(new GridLayout(4, 2));

		addLabel("广告编号..:");
		jtfNo = new JTextField("", 30);
		panel.add(jtfNo);
		jtfNo.getDocument().addDocumentListener(listener);

		addLabel("广告名称..:");
		jtfName = new JTextField("", 30);
		panel.add(jtfName);

		addLabel("广告图片...:");
		jtfPics = new JTextField("", 8);
		panel.add(jtfPics);

		addLabel("广告价格...:");
		jtfPrice = new JTextField("", 10);
		panel.add(jtfPrice);

		add(panel, BorderLayout.CENTER);

		ActionListener command = new MemCommandAction();

		btnPanel = new JPanel();
		addButton("[+] 添加", command);
		addButton("[-] 删除", command);
		addButton("[u] 更新", command);
		
		add(btnPanel, BorderLayout.SOUTH);
	}

	private void addLabel(String txt) {
		JLabel label = new JLabel(txt);
		panel.add(label);
	}

	private void addButton(String label, ActionListener listener) {
		JButton button = new JButton(label);
		button.addActionListener(listener);
		btnPanel.add(button);
	}

	public void updateField() {
		String adId = jtfNo.getText();
		if (!"".equals(adId)) {

			int id = Integer.parseInt(adId);

			try {
				AdsRecord ar = rmiService.selADs(id);

				if (null != ar) {
					jtfName.setText(ar.adName);
					jtfPics.setText(ar.adPics);
					jtfPrice.setText(""+ar.adPrice);
				}

			} catch(Exception e) {
				e.printStackTrace();
			}
		}
	}

	private class KeyFieldListener implements DocumentListener {
		public void insertUpdate(DocumentEvent event) { updateField(); }
		public void removeUpdate(DocumentEvent event) { updateField(); }
		public void changedUpdate(DocumentEvent event) {}
	}

	private class MemCommandAction implements ActionListener {
		public void actionPerformed(ActionEvent event) {
			String input = event.getActionCommand();

			String ret = input + " 编号[";

			try {
				int id = Integer.parseInt(jtfNo.getText());
				int price = Integer.parseInt(jtfPrice.getText());

				if (input.equals("[+] 添加")) 
					id = rmiService.addADs(jtfName.getText(),
							jtfPics.getText(), price); 

				if (input.equals("[-] 删除")) 
					rmiService.delADs(id);

				if (input.equals("[u] 更新")) 
					rmiService.updADs(id, jtfName.getText(),
							jtfPics.getText(), price);

				ret += id + "] 成功.";

			} catch (Exception e) {
				e.printStackTrace();
			}

			mesg.setText(ret);
		}
	}
}
