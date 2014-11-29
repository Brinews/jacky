package networking;

import java.awt.Rectangle;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

import javax.swing.JFrame;
import javax.swing.JTextArea;

/**
 * main socket server
 * 
 * @author
 * 
 */
public class RobotServer extends JFrame implements Runnable {

	Thread serverThread = null;
	ServerSocket server = null;
	Socket sk = null;

	/* only accept two client */
	BufferedReader rdr[] = new BufferedReader[2];
	PrintWriter wtr[] = new PrintWriter[2];
	int clientNum = 0;

	JTextArea displayWindow; /* display server state */

	public RobotServer() {
		super();
		this.setBounds(new Rectangle(350, 300));
		this.setLayout(null);
		this.setResizable(false);
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		displayWindow = new JTextArea();
		displayWindow.setBounds(5, 5, 340, 290);
		this.add(displayWindow);
	}

	@Override
	public void run() {
		
		try {
			server = new ServerSocket(9908);
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		displayWindow.append("Server Listening On Port: 9908\n");
		
		try {
			while (true) {
				
				/* waiting for client's connection */
				try {
					sk = server.accept();
					
					if (clientNum >= 2) {
						displayWindow.append("Connection is full..\n");
						sk.close();

						continue;
					}

					/* start a thread to process connection */
					ServerThread sth = new ServerThread(sk, ++clientNum);
					sth.start();

					if (clientNum == 1) {
						displayWindow.append("Tagger Logon. Welcome!\n");
					} else if (clientNum == 2) {
						displayWindow
						  .append("Puzzler Logon. Welcome!\n Game Begin.\n");
						
						/* broadcast start message */
						for (int i = 1; i < 3; i++) {
							wtr[i-1].println(i + " Game Start!\n");
							wtr[i-1].flush();
						}
					}

					Thread.currentThread().sleep(1000);

				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		} finally {
			try {
				server.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	
	public void start()
	{
		if (serverThread == null) {
			serverThread = new Thread(this);
		}
		
		serverThread.start();
	}

	/**
	 * 处理线程
	 */
	class ServerThread extends Thread {
		Socket sk = null;
		int fdNum = 0;

		public ServerThread(Socket st, int no) {
			sk = st;
			fdNum = no - 1;
		}

		/***
		 * 准备数据流
		 */
		@Override
		public void run() {
			try {
				wtr[fdNum] = new PrintWriter(sk.getOutputStream());
				rdr[fdNum] = new BufferedReader(new InputStreamReader(sk
						.getInputStream()));
				
				wtr[fdNum].println("Hello player: " + fdNum);
				wtr[fdNum].flush();
				
				while (true) {
					
					if (sk.isClosed()){
						break;
					}
					
					String msg = rdr[fdNum].readLine();
					
					if (msg == null) break;
					
					/* waiting for process */
					if (msg.indexOf("IMAGE-SYNC") >= 0) {
						displayWindow.append(msg + "\n");
						
						wtr[1-fdNum].println(msg);
						wtr[1-fdNum].flush();
					}
					
					if (msg.indexOf("SELECT-SYNC") >= 0) {
						displayWindow.append(msg + "\n");
						
						wtr[1-fdNum].println(msg);
						wtr[1-fdNum].flush();
					}
					
					if (msg.indexOf("TARGET-SYNC") >= 0) {
						displayWindow.append(msg + "\n");
						
						wtr[1-fdNum].println(msg);
						wtr[1-fdNum].flush();
					}
					
					if (msg.indexOf("RESULT-SYNC") >= 0) {
						displayWindow.append(msg + "\n");
						
						wtr[1-fdNum].println(msg);
						wtr[1-fdNum].flush();
					}
				}
				
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	
	public static void main(String[] args)
	{
		new RobotServer().start();
	}
}
