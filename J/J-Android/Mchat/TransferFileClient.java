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
			// �����ͷ�����������
			Socket socket = new Socket("127.0.0.1", 6666);

			// ��ȡ�û������ļ���
			String filePath = "C:/usr/";
			String fileName = "1.jpg";

			// �趨�ļ���С
			File f = new File(filePath + fileName);

			// ��ȡ�ļ����ݣ�������ת��Ϊbyte[]
			FileInputStream fis = new FileInputStream(filePath + fileName);

			// ��FilePojo����fpoд��Socket clientָ���������
			long start = System.currentTimeMillis();

			DataInputStream dis = new DataInputStream(
					new BufferedInputStream(fis));

			DataOutputStream dos = new DataOutputStream(
					new BufferedOutputStream(
						socket.getOutputStream()));

			//�����ļ���
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
