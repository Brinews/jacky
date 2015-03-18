import java.io.BufferedReader;  
import java.io.IOException;  
import java.io.InputStream;  
import java.io.InputStreamReader;  
import java.io.OutputStream;  
import java.io.PrintWriter;  
import java.net.ServerSocket;  
import java.net.Socket;  
import java.net.URLDecoder;  
import java.net.UnknownHostException;  
import java.util.ArrayList;  

import java.io.BufferedInputStream;  
import java.io.BufferedOutputStream;  
import java.io.DataInputStream;  
import java.io.DataOutputStream;  
import java.io.FileOutputStream;  

import java.io.File;
import java.io.FileInputStream;


//服务器程序  
//在程序界中,网络是指2个程序产生信息交互,那么构成网络  
//所以网络必须要有2个程序以上,一端叫服务器,一端叫客户端,通常服务器和客户端的对应关系是1-N  

public class SimpleServer {  

	public SimpleServer() {}

	//用于保存当前连接的用户  
	public static ArrayList<Socket> socketList=new ArrayList<Socket>();  

	public static void main(String[] args)throws Exception {  

		//启动文件服务器线程
		new FileListenServer().start();

		//启动发送文件服务线程
		new FileSendServer().start();

		//创建一个serverSocket，用于监听客户端Socket的连接请求  
		ServerSocket serverSocket = new ServerSocket(10002);//10002为此服务器的端口号  
		System.out.println("服务启动");  

		//采用循环不断接收来自客户端的请求  
		while (true) {  
			//每当接收到客户端的请求时，服务器端也对应产生一个Socket  
			Socket socket = serverSocket.accept();  

			System.out.println("服务连接, IP地址为:");  
			System.out.println(socket.getInetAddress().getHostAddress());

			// 把连接的客户端加入到list列表中  
			socketList.add(socket);  

			//启动一个新的线程  
			//任务线程,每个连接用都有一个属于自己专用的任务线程  
			//这个线程内会处理信息的发送和响应  
			new MessageServer(socket,socketList).start();  
		}
	}  
}  

/* 消息交互服务器 */
class MessageServer extends Thread {  

	Socket client;  
	ArrayList<Socket> clients;  

	BufferedReader br;  

	public MessageServer(Socket client,ArrayList<Socket> clients)throws Exception {  
		super();  
		this.client = client;  
		this.clients = clients;  

		br = new BufferedReader(new InputStreamReader(this.client.getInputStream()));  
	}

	//把接收到的消息发送给各客户端  
	public void run() {  
		try {  
			String content=null;  
			while(true){  
				//从某个客户端读取信息  
				if((content=br.readLine())!=null) {  
					for(Socket socket:clients){  
						if(client!=socket){  
							//把读取到的消息发送给各个客户端  
							PrintWriter pw = new PrintWriter(socket.getOutputStream(), true);  
							pw.println(socket.getInetAddress().getHostAddress() + ":" + content);  
						}  
					}  
					content = URLDecoder.decode(content, "UTF-8");  
					System.out.println(content);  
				}     
			}
		}catch (IOException e) {  
			e.printStackTrace();  
		}  
	}  
}

/* 接收来自客户端的文件服务器 */
class FileListenServer extends Thread {
	int file_port = 6666;

	public void run() {
		ServerSocket server;

		try {
			server = new ServerSocket(file_port);

			while (true) {
				Socket client = server.accept();

				//启动接收服务线程
				new FileRecvServer(client).start();
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}

class FileRecvServer extends Thread {
	Socket client;

	public FileRecvServer(Socket s) {
		client = s;
	}

	public void run() {
		try {
			DataInputStream dis = new DataInputStream(
					new BufferedInputStream(client.getInputStream()));

			//接收文件名
			String fileName = dis.readUTF();

			System.out.println("接收到文件:" + fileName);

			DataOutputStream dos = new DataOutputStream(
					new BufferedOutputStream(
						new FileOutputStream(fileName)));
			//接收文件内容
			int bufsiz = 8192;
			int filsiz = 0;
			byte[] buf = new byte[bufsiz];

			while (true) {
				int read = 0;
				if (dis != null)
					read = dis.read(buf);

				if (read == -1)
					break;

				filsiz += read;

				dos.write(buf, 0, read);
			}

			System.out.println("接收文件大小:" + filsiz);

			dos.close();
			dis.close();
			client.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}

/* 发送文件服务线程 */
class FileSendServer extends Thread {

	int port = 8821; //连接端口

	public void run() {

		Socket s = null;  

		try {  

			ServerSocket ss = new ServerSocket(port);  

			while (true) {  

				s = ss.accept();  //1. 建立连接

				DataInputStream dis = new DataInputStream(  
						new BufferedInputStream(s.getInputStream()));  

				log("接收文件传输请求连接.");  

				String fileName = dis.readUTF(); //2.读取待发送文件名

				dis.readByte();  //3. 读取系统标识

				//4. 建立文件流及socket流
				DataOutputStream dos = 
					new DataOutputStream(s.getOutputStream());  

				File file = new File(fileName);  
				//文件不存在
				if (!file.exists()) {

					log(fileName + ", 该文件不存在.");

					dos.writeUTF("NO-EXIST");
					dos.flush();

					dis.close();
					dos.close();
					s.close();

					continue;
				}

				System.out.println("待发送文件大小:" 
						+ (int) file.length());  

				DataInputStream fis = new DataInputStream(  
							new BufferedInputStream(
							new FileInputStream(fileName)));  


				//5. 写文件名
				dos.writeUTF(file.getName());  
				dos.flush();  

				//6. 写文件长度
				dos.writeLong((long) file.length());  
				dos.flush();  

				//7. 写文件内容
				int bufferSize = 8192;  
				byte[] buf = new byte[bufferSize];  
				while (true) {  
					int read = 0;  
					if (fis != null) {  
						read = fis.read(buf);  
					}  
					if (read == -1) {  
						break;  
					}  
					dos.write(buf,0,read);  
				}  
				dos.flush();  

				// 注意关闭socket链接，不然客户端会等待server的数据  
				// 直到socket超时，导致数据不完整。  
				fis.close();  
				dos.close();
				s.close();  

				log("文件传输完成.\n");  
			}  
		} catch (IOException e) {  
			e.printStackTrace();  
		} 
	}

	void log(String msg) {  
		System.out.println(msg);  
	}  
}
