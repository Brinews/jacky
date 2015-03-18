import java.io.BufferedInputStream;  
import java.io.BufferedOutputStream;  
import java.io.DataInputStream;  
import java.io.DataOutputStream;  
import java.io.File;  
import java.io.FileInputStream;  
import java.io.IOException;  
import java.net.ServerSocket;  
import java.net.Socket;  

public class TransferFileClient {

	private void start() {
		try
		{
			// 建立和服务器的连接
			Socket socket = new Socket("127.0.0.1", 6666);

			// 获取用户输入文件名
			String filePath = "C:/usr/";
			String fileName = "1.jpg";

			// 设定文件大小
			File f = new File(filePath + fileName);

			// 读取文件内容，并将其转换为byte[]
			FileInputStream fis = new FileInputStream(filePath + fileName);

			// 将FilePojo对象fpo写到Socket client指定的输出流
			long start = System.currentTimeMillis();

			DataInputStream dis = new DataInputStream(
					new BufferedInputStream(fis));

			DataOutputStream dos = new DataOutputStream(
					new BufferedOutputStream(
						socket.getOutputStream()));

			//发送文件名
			dos.writeUTF(fileName);
			dos.flush();

			int bufsiz = 8192;
			byte[] buf = new byte[bufsiz];

			while (true) {
				int read = 0;
				if (dis != null)
					read = dis.read(buf);

				if (read == -1) break;

				dos.write(buf, 0, read);
			}
			dos.flush();

			dis.close();
			dos.close();
			socket.close();

			long end = System.currentTimeMillis();

			System.out.println("Times:"+(end-start));

		}
		catch (Exception ioe)
		{
			ioe.printStackTrace();
		}
	}

	public static void main(String args[]) {
		new TransferFileClient().start();
	}
}
