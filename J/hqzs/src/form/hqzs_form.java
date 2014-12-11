import javax.swing.*;
import javax.swing.border.*;
import java.util.Date;
import javax.swing.border.TitledBorder;
import javax.swing.border.EtchedBorder;

import java.awt.*;
import java.awt.event.*;

import java.io.*;
import java.sql.ResultSet;

public class hqzs_form extends JPanel {

	JPanel controls;
	private Color mfColor = new Color(0, 100, 0);
	public Surface surf;

	public hqzs_form()
	{
		setLayout(new BorderLayout());
		setBorder(new TitledBorder(new EtchedBorder(), "行情走势分析"));
		//setBackground(Color.black);


		JButton pdfBtn = new JButton("Make PDF");
		JButton mailBtn = new JButton("Send Mail");

		ActionListener mkpdf_act = new ActionListener() {
			public void actionPerformed(ActionEvent e) {

				System.out.println("Test DB reading..\n");
				hqzs_dbcon dbcon = new hqzs_dbcon();

				ResultSet rs = dbcon.ReadDataSet("select * from employee");

				try {
					while (rs.next()) {
						System.out.println(rs.getString(1));
					}
				} catch (Exception ex) {
					System.out.println(ex.getMessage());
				}
				System.out.println("DB reading over\n");

				/* Pdf testing */
				System.out.println("Pdf Testing..");
				hqzs_pdf hp = new hqzs_pdf();
				hp.CreatePdfDocument();
				System.out.println("Create Complete."); 
			}
		};
		pdfBtn.addActionListener(mkpdf_act);

		ActionListener smail_act = new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				System.out.println("Sending Mail...");

				hqzs_mail hMail = new hqzs_mail();
				hMail.send("yl200443109@126.com", "yl200443109", "See You",
						"Long Time No See. 谢谢。");
				hMail.SendToAll();
				System.out.println("Send Complete.");
			}
		};
		mailBtn.addActionListener(smail_act);

		JPanel control = new JPanel();
		control.add(pdfBtn, BorderLayout.NORTH);
		control.add(mailBtn, BorderLayout.SOUTH);
		control.setBorder(new EtchedBorder());
		control.setSize(this.getWidth(), 30);

		add(control, BorderLayout.NORTH);

		add(surf = new Surface(), BorderLayout.CENTER);

		JPanel text = new JPanel();
		JTextArea jta = new JTextArea("使用注意:\n");
		jta.append("1.配置数据库db.conf  \n");
		jta.append("2.当前目录生成PDF报表文件  \n");
		jta.append("3.配置邮件地址列表文件email.lst  \n");
		jta.setEnabled(false);
		jta.setForeground(mfColor);
		jta.setSize(text.getWidth(), text.getHeight());
		text.setBorder(new EtchedBorder());
		text.add(jta, BorderLayout.CENTER);

		add(text, BorderLayout.SOUTH);
	}

	public class Surface extends JPanel implements Runnable {

		public Thread thread;

		public Surface()
		{
		}

		public void paint(Graphics g) {
			g.clearRect(0, 0, this.getWidth(), this.getHeight());
			g.setColor(Color.black);
			g.fillRect(0, 1, this.getWidth(), 25);
			g.setColor(Color.green);
			g.drawString(new Date().toString(), 5, 20);
		}

		public void start() {
			thread = new Thread(this);
			thread.setPriority(Thread.MIN_PRIORITY);
			thread.setName("Time");
			thread.start();
		}

		public synchronized void stop() {
			thread = null;
			notify();
		}

		public void run() {
			Thread me = Thread.currentThread();

			while (thread == me) {
				repaint();

				try {
					thread.sleep(1000);
				} catch (InterruptedException e) {
					break;
				}
			}

			thread = null;
		}

	}

	public static void main(String[] args)
	{
		final hqzs_form hf = new hqzs_form();
		WindowListener wl = new WindowAdapter() {
			public void windowClosing(WindowEvent e) {
				System.exit(0); }
			public void windowIconified(WindowEvent e) {
				hf.surf.stop(); }
			public void windowDeiconified(WindowEvent e) {
				hf.surf.start(); }
		};
		JFrame frm = new JFrame("行情走势");
		frm.addWindowListener(wl);
		frm.getContentPane().add("Center", hf);
		frm.pack();
		frm.setSize(new Dimension(280, 240));
		frm.setVisible(true);
		hf.surf.start();
	}
}
