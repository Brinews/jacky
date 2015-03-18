import java.io.BufferedReader;  
import java.io.IOException;  
import java.io.InputStream;  
import java.io.InputStreamReader;  
import java.io.PrintWriter;  
import java.net.Socket;  
import java.net.URLDecoder;  
import java.net.URLEncoder;  

public class Client {  
	/** 
	 * @param args 
	 */  
	public static void main(String[] args) throws Exception {  

		//创建连接到本机，端口为10002的socket  
		Socket client = new Socket("127.0.0.1", 10002);  

		new MyThread(client).start();  
		InputStream is = client.getInputStream();  
		BufferedReader br = new BufferedReader(new InputStreamReader(is));  

		while (true) {  
			String msg = br.readLine();  
			//对收到的信息进行解码  
			msg=URLDecoder.decode(msg, "UTF-8");  
			System.out.println(msg);  
		}  
	}  
}  

class MyThread extends Thread {  

	Socket client;  

	public MyThread(Socket client) {  
		super();  
		this.client = client;  
	}  
	public void run() {  
		// 发出消息  
		try {  
			BufferedReader br = new BufferedReader(new InputStreamReader(  
						System.in));  
			PrintWriter pw = new PrintWriter(client.getOutputStream(), true);  
			// 把输入的内容输出到socket  
			while (true) {  
				String msg = br.readLine();  
				//对发出的消息进行编码  
				msg = URLEncoder.encode(msg, "UTF-8");  
				pw.println(msg);  
			}  
		} catch (IOException e) {  
			// TODO Auto-generated catch block  
			e.printStackTrace();  
		}  
	}  
}  
