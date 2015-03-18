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


//����������  
//�ڳ������,������ָ2�����������Ϣ����,��ô��������  
//�����������Ҫ��2����������,һ�˽з�����,һ�˽пͻ���,ͨ���������Ϳͻ��˵Ķ�Ӧ��ϵ��1-N  

public class SimpleServer {  

	public SimpleServer() {}

	//���ڱ��浱ǰ���ӵ��û�  
	public static ArrayList<Socket> socketList=new ArrayList<Socket>();  

	public static void main(String[] args)throws Exception {  

		//�����ļ��������߳�
		new FileListenServer().start();

		//���������ļ������߳�
		new FileSendServer().start();

		//����һ��serverSocket�����ڼ����ͻ���Socket����������  
		ServerSocket serverSocket = new ServerSocket(10002);//10002Ϊ�˷������Ķ˿ں�  
		System.out.println("��������");  

		//����ѭ�����Ͻ������Կͻ��˵�����  
		while (true) {  
			//ÿ�����յ��ͻ��˵�����ʱ����������Ҳ��Ӧ����һ��Socket  
			Socket socket = serverSocket.accept();  

			System.out.println("��������, IP��ַΪ:");  
			System.out.println(socket.getInetAddress().getHostAddress());

			// �����ӵĿͻ��˼��뵽list�б���  
			socketList.add(socket);  

			//����һ���µ��߳�  
			//�����߳�,ÿ�������ö���һ�������Լ�ר�õ������߳�  
			//����߳��ڻᴦ����Ϣ�ķ��ͺ���Ӧ  
			new MessageServer(socket,socketList).start();  
		}
	}  
}  

/* ��Ϣ���������� */
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

	//�ѽ��յ�����Ϣ���͸����ͻ���  
	public void run() {  
		try {  
			String content=null;  
			while(true){  
				//��ĳ���ͻ��˶�ȡ��Ϣ  
				if((content=br.readLine())!=null) {  
					for(Socket socket:clients){  
						if(client!=socket){  
							//�Ѷ�ȡ������Ϣ���͸������ͻ���  
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

/* �������Կͻ��˵��ļ������� */
class FileListenServer extends Thread {
	int file_port = 6666;

	public void run() {
		ServerSocket server;

		try {
			server = new ServerSocket(file_port);

			while (true) {
				Socket client = server.accept();

				//�������շ����߳�
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

			//�����ļ���
			String fileName = dis.readUTF();

			System.out.println("���յ��ļ�:" + fileName);

			DataOutputStream dos = new DataOutputStream(
					new BufferedOutputStream(
						new FileOutputStream(fileName)));
			//�����ļ�����
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

			System.out.println("�����ļ���С:" + filsiz);

			dos.close();
			dis.close();
			client.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}

/* �����ļ������߳� */
class FileSendServer extends Thread {

	int port = 8821; //���Ӷ˿�

	public void run() {

		Socket s = null;  

		try {  

			ServerSocket ss = new ServerSocket(port);  

			while (true) {  

				s = ss.accept();  //1. ��������

				DataInputStream dis = new DataInputStream(  
						new BufferedInputStream(s.getInputStream()));  

				log("�����ļ�������������.");  

				String fileName = dis.readUTF(); //2.��ȡ�������ļ���

				dis.readByte();  //3. ��ȡϵͳ��ʶ

				//4. �����ļ�����socket��
				DataOutputStream dos = 
					new DataOutputStream(s.getOutputStream());  

				File file = new File(fileName);  
				//�ļ�������
				if (!file.exists()) {

					log(fileName + ", ���ļ�������.");

					dos.writeUTF("NO-EXIST");
					dos.flush();

					dis.close();
					dos.close();
					s.close();

					continue;
				}

				System.out.println("�������ļ���С:" 
						+ (int) file.length());  

				DataInputStream fis = new DataInputStream(  
							new BufferedInputStream(
							new FileInputStream(fileName)));  


				//5. д�ļ���
				dos.writeUTF(file.getName());  
				dos.flush();  

				//6. д�ļ�����
				dos.writeLong((long) file.length());  
				dos.flush();  

				//7. д�ļ�����
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

				// ע��ر�socket���ӣ���Ȼ�ͻ��˻�ȴ�server������  
				// ֱ��socket��ʱ���������ݲ�������  
				fis.close();  
				dos.close();
				s.close();  

				log("�ļ��������.\n");  
			}  
		} catch (IOException e) {  
			e.printStackTrace();  
		} 
	}

	void log(String msg) {  
		System.out.println(msg);  
	}  
}
