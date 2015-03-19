import java.io.BufferedInputStream;  
import java.io.DataInputStream;  
import java.io.DataOutputStream;  
import java.io.File;  
import java.io.FileInputStream;  
import java.io.IOException;  
import java.net.ServerSocket;  
import java.net.Socket;  

public class TransferFileServer {  
	private static final int HOST_PORT = 8821;  
	private void start() {  
		Socket s = null;  
		try {  
			ServerSocket ss = new ServerSocket(HOST_PORT);  
			while (true) {  
				String filePath = "C:/usr/2.jpg";  //����
				File file = new File(filePath);  

				System.out.println("�ļ�����:" + (int) file.length());  

				s = ss.accept();  
				log("����Socket����");  
				DataInputStream dis = new DataInputStream(  
						new BufferedInputStream(s.getInputStream()));  
				dis.readByte();  
				DataInputStream fis = new DataInputStream(  
						new BufferedInputStream(new FileInputStream(filePath)));  
				DataOutputStream dos = new DataOutputStream(s.getOutputStream());  
				dos.writeUTF(file.getName());  
				dos.flush();  

				dos.writeLong((long) file.length());  
				dos.flush();  

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
				s.close();  

				log("�ļ��������");  
			}  
		} catch (IOException e) {  
			// TODO Auto-generated catch block  
			e.printStackTrace();  
		}  
	}  

	void log(String msg) {  
		System.out.println(msg);  
	}  

	public static void main(String args[]) {  
		new TransferFileServer().start();  
	}  
}  