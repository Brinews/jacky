package networking;

import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Rectangle;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.border.TitledBorder;

import robotFixing.Game;

public class RobotClient extends JFrame implements Runnable{
	
	private String ipAddr = "127.0.0.1";
	private int portNum = 9908;
	
	private Socket socket = null;
	BufferedReader rdr = null;
	PrintWriter wtr = null;
	
	private JTextArea displayArea;
	private JTextField ipText;
	private JTextField portText;
	private JButton logonBtn;
	
	private Game tagger = null;
	private Game puzzler = null;
	
	private Thread conThread = null;
	
	public RobotClient(){
		super();
		
		this.setBounds(new Rectangle(370, 390));
		this.setLayout(null);
		//this.setResizable(false);
		this.setVisible(false);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		JPanel mainPane = new JPanel();
		mainPane.setSize(new Dimension(350, 350));
		mainPane.setBorder(new TitledBorder("1"));
		this.add(mainPane);
		
		JPanel disPane = new JPanel();
		disPane.setLayout(new FlowLayout());
		disPane.setBounds(5, 5, 320, 180);
		displayArea = new JTextArea(10, 10);
		displayArea.setBounds(10, 10, 330, 170);
		displayArea.setSize(new Dimension(320, 160));
		displayArea.append("Welcome !\n");
		
		disPane.add(displayArea);
		mainPane.add(displayArea);
		
		mainPane.setLayout(null);
		
		JPanel ipPane = new JPanel();
		ipPane.setBounds(5, 180, 320, 60);
		ipPane.setBorder(new TitledBorder("i"));
		ipPane.setLayout(new FlowLayout(FlowLayout.LEFT));
		ipText = new JTextField("127.0.0.1",20);
		JLabel ipLabel = new JLabel("Server IP:");
		ipPane.add(ipLabel);
		ipPane.add(ipText);
		
		mainPane.add(ipPane);
		
		JPanel portPane = new JPanel();
		portPane.setBounds(5, 240, 320, 60);
		portPane.setBorder(new TitledBorder("p"));
		portPane.setLayout(new FlowLayout(FlowLayout.LEFT));
		portText = new JTextField("9908", 20);
		JLabel portLabel = new JLabel("Port Num:");
		portPane.add(portLabel);
		portPane.add(portText);
		
		mainPane.add(portPane);
		
		JPanel btnPanel = new JPanel();
		btnPanel.setBounds(5, 300, 320, 40);
		ButtonListener conAction = new ButtonListener();
		logonBtn = new JButton();
		logonBtn.setText("LOGON");
		logonBtn.addActionListener(conAction);
		btnPanel.add(logonBtn);
		
		mainPane.add(btnPanel);
	}
	
	public void start()
	{
		if (conThread == null)
			conThread = new Thread(this);
		
		this.setVisible(true);
	}
	
	@Override 
	public void run(){
		
		try {
			if (ipText.getText()!="") ipAddr = ipText.getText();
			if (portText.getText()!="") portNum = Integer.parseInt(portText.getText());
			
			socket = new Socket(ipAddr, portNum);
			
			wtr = new PrintWriter(socket.getOutputStream());
			rdr = new BufferedReader(new InputStreamReader(socket.getInputStream()));
			
			while (true) {
				String recvInfo = rdr.readLine();
				displayArea.append(recvInfo + "\n");
				
				if (recvInfo.indexOf("1 Game Start!") >= 0) {
					/* tagger player */
					tagger = new Game("tagger", wtr, rdr);
					tagger.start();
				}
				
				if (recvInfo.indexOf("2 Game Start!") >= 0) {
					/* puzzler player */
					puzzler = new Game("puzzler", wtr, rdr);
					puzzler.start();
				}
				
				if (recvInfo.indexOf("IMAGE-SYNC") >= 0) {
					/* puzzler received image id */
					
					int imageId = Integer.parseInt(recvInfo.split(" ")[1]);
					int rows = Integer.parseInt(recvInfo.split(" ")[2]);
					int cols = Integer.parseInt(recvInfo.split(" ")[3]);
					
					puzzler.syncGameStage(imageId, rows, cols);
				}
				
				if (recvInfo.indexOf("SELECT-SYNC") >= 0) {
					/* puzzler receive selected chunks info */
					String[] str = recvInfo.split(" ");
					int []selectedChunks = new int[str.length-1];
					for (int i = 1; i < str.length; i++) {
						selectedChunks[i-1] = Integer.parseInt(str[i]);
					}
					
					puzzler.syncSelectedChunks(selectedChunks);
				}
				
				if (recvInfo.indexOf("TARGET-SYNC") >= 0) {
					/* puzzler receive target pattern */
					String[] tar = recvInfo.split(" ");
					
					int[] target = new int[tar.length - 1];
					for (int i = 1; i < tar.length; i++) {
						target[i-1] = Integer.parseInt(tar[i]);
					}
					
					puzzler.syncGameTargetPattern(target);
				}
				
				if (recvInfo.indexOf("RESULT-SYNC") >= 0) {
					/* tagger receive result */
					String[] state = recvInfo.split(" ");
					
					boolean winflag = false;
					if (state[1].equals("1")) winflag = true;
					
					tagger.syncGameResult(winflag);
				}
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}
		finally{
			try {
				socket.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	
	class ButtonListener implements ActionListener {
		@Override
		public void actionPerformed(ActionEvent e) {
			if (e.getSource() == logonBtn) {
				conThread.start();
				logonBtn.setEnabled(false);
			}
		}
	}
	
	public static void main(String[] args)
	{
		new RobotClient().start();
	}
}
