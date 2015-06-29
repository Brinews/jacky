import javax.swing.*;
import javax.swing.event.*;
import java.awt.*;
import java.awt.event.*;

import java.rmi.Remote;

class LoginPanel extends JPanel {

	JTextField jtfName;
	JPasswordField jtfPwd;
	JTextField jtfType;

	LinkService frame = null;
	IRmiService rmiService = null;

	public LoginPanel(IRmiService rmiSer, LinkService f) {
		setLayout(null);
		setSize(800, 480);

		rmiService = rmiSer;
		frame = f;

		ActionListener command = new LoginCommandAction();

		addLabel("用户名:", 260, 220, 80, 22);
		addLabel("密  码:", 260, 246, 80, 22);
		addLabel("类  型:", 260, 272, 80, 22);

		jtfName = new JTextField("", 30);
		jtfPwd = new JPasswordField("", 30);
		jtfType = new JTextField("", 30);

		jtfName.setBounds(345, 220, 80, 22);
		jtfPwd.setBounds(345, 246, 80, 22);
		jtfType.setBounds(345, 272, 80, 22);

		add(jtfName);
		add(jtfPwd);
		add(jtfType);

		addButton("登陆", command, 280, 298, 80, 22); 

		setVisible(true);
	}

	private void addLabel(String txt, int x, int y, int w, int h) {
		JLabel label = new JLabel(txt);
		label.setBounds(x, y, w, h);
		add(label);
	}

	private void addButton(String label, ActionListener listener,
			int x, int y, int w, int h) {
		JButton button = new JButton(label);
		button.setBounds(x, y, w, h);

		button.addActionListener(listener);
		add(button);
	}

	class LoginCommandAction implements ActionListener {
		public void actionPerformed (ActionEvent event) {
			String input = event.getActionCommand();

			String name = jtfName.getText();
			String pwd = jtfPwd.getText();
			String type = jtfType.getText();


			if (input.equals("登陆")) {
				try {

					boolean ret = rmiService.checkLogin(name, pwd, type);

					if (ret == true && type.equals("1")) {
						/* seller */
						frame.changePanel(1);
					} else if (ret == true && type.equals("2")) {
						/* buyer */
						frame.changePanel(2);
					}
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}
	}
}
