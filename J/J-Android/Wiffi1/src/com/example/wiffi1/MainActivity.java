package com.example.wiffi1;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.widget.TextView;

import java.io.File;
import java.io.FileInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;
import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.Toast;
import android.util.Log;
import android.widget.EditText; 
import java.io.PrintWriter;
import java.io.BufferedReader; 
import android.os.Handler;  

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;  
import java.io.DataInputStream;  
import java.io.DataOutputStream;   
import java.io.FileOutputStream;
import java.io.IOException;    

import java.net.UnknownHostException;
import java.io.InputStreamReader;
import java.net.URLDecoder;
import java.net.URLEncoder;

public class MainActivity extends Activity {

	private static final String TAG = "SOCKET";
	
    private String sendMessage = "Linux"; 
    
    private Button btn_share;  
	private Button btn_send;
	private Button btn_get;
	
	private String filePath = "/sdcard/";
	private String fileName = "1.jpg";

    private EditText msg_txt;
    private EditText file_txt;
    private EditText content;
    
    private Socket socket;  
    private PrintWriter pw;  
    private BufferedReader br;  
    private Handler handler;  
    
    private ClientSocket cs = null;  
    private static final String FILE_PATH = "/sdcard/";  
    private String ip = "10.0.2.2";  
    
    
    private int recv_port = 8821;  
    private int msg_port = 10002;
    private int fil_port = 6666;
    
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		/*
		TextView tv = new TextView(this); 
		tv.setText(new MacAddr().getLocalIpAddress()); 
		tv.setText("Six Egg.");
		setContentView(tv);
		*/
		
		msg_txt = (EditText)findViewById(R.id.ta_msg);
		file_txt = (EditText)findViewById(R.id.ta_file);
		
		content = (EditText) findViewById(R.id.tv_content);
		content.setEnabled(false);
		
		handler = new Handler();
		
		connectServer();

		btn_send = (Button) this.findViewById(R.id.btn_send);
		btn_send.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				messageSend();
			}
		});
		
		btn_share = (Button)findViewById(R.id.btn_share);  
        btn_share.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				fileShare();//�ļ�����
			}
		});
        
        btn_get = (Button)findViewById(R.id.btn_get);  
        btn_get.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				String fileName = file_txt.getText().toString();
				fileRecv(fileName);//�ļ�����
			}
		});
        
	}
	
	/*
	 * ����pc������,������Ϣ����
	 */
	private void connectServer()
	{
		try {  
            socket=new Socket(ip, msg_port); 
        } catch (UnknownHostException e) {  
            e.printStackTrace();  
            Log.e("socket","unknown host");  
        } catch (IOException e) {  
            e.printStackTrace();  
            Log.e("socket","io execption");  
        }  
        
		if(socket==null){  
            Log.e("socket","null");  
        }  
        else  
            try {  
            	
            pw=new PrintWriter(socket.getOutputStream());  
            br=new BufferedReader(new InputStreamReader(socket.getInputStream()));
            
            if(pw!=null && br!=null){  
                new Thread(
                		new Runnable()
                		{
                			@Override
                			public void run() {
                				try {  
                	                String str;  
                	                while((str=br.readLine())!=null){  
                	                    
                	                	final String s=URLDecoder.decode("\n"+str, "UTF-8");  
                	                    
                	                    handler.post(new Runnable(){  
                	      
                	                        public void run() {  
                	                            //Toast.makeText(MainActivity.this, s, Toast.LENGTH_LONG)
                	                            //     .show();

                	                        	try {
                	                        		if (s.indexOf("SHA-FIL:")>=0) {
                	                        			file_txt.setText(s.substring(9));
                	                        		}
                	                        		else {
                	                        			content.append(s);
                	                        			content.setSelection(content.length());
                	                        		}
                	                        	}
                	                        	catch(Exception e)
                	                        	{
                	                        		Log.e(TAG, e.toString());
                	                        	}
                	                              
                	                        }});  
                	                      
                	                }  
                	            } catch (IOException e) {  
                	                e.printStackTrace();  
                	            }  

                			}
                		}).start();  
            }  
        } catch (IOException e) {  
            e.printStackTrace();  
        }  

	}
	
	private void messageSend()
	{
		String str = msg_txt.getText().toString();
		
		content.append("\n��˵:"+str+"\n");
		
		try {
			str = URLEncoder.encode(str, "UTF-8");
		
			pw.println(str);
			pw.flush();
		}
		catch (IOException e){
			e.printStackTrace();
		}
	}
	
	/* share�ļ��������� */
	private void fileShare() {
		String str = file_txt.getText().toString();
		try {
			str = URLEncoder.encode("SHA-FIL:"+str, "UTF-8");
			pw.println(str);
			pw.flush();
			
			fileSend(); //�����ļ���PC������
			
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	/* ����share�������ļ� */
	private void fileRecv(String fileName) {  
        if (createConnection()) {  
            sendMessage(fileName);  
            getMessage();  
        }  
    }  
	
    private void getMessage() {  
        if (cs == null)  
            return;  
        
        DataInputStream inputStream = null;  
        inputStream = cs.getMessageStream();  
        
        try {  
            String savePath = FILE_PATH;  
            int bufferSize = 8192;  
            byte[] buf = new byte[bufferSize];  
            int passedlen = 0;  
            long len = 0;  
            
            //1. �����ļ���  
            String name = inputStream.readUTF();
            
            if (name.equals("NO-EXIST")) {
            	Log.d("AndroidClient", "�ļ�������");
            	return;
            }
            
            savePath += name;
            Log.d("AndroidClient","@@@savePath"+savePath);  
            DataOutputStream fileOut = new DataOutputStream(  
                    new BufferedOutputStream(new BufferedOutputStream(  
                            new FileOutputStream(savePath))));  
            //2.�����ļ�����
            len = inputStream.readLong();  
            Log.d("AndoridClient","�ļ��ĳ���Ϊ:"+len);  
            
            Log.d("AndroidClient","��ʼ�����ļ�");  
            while(true) {  
                int read = 0;  
                if (inputStream != null) {  
                    read = inputStream.read(buf);  
                }  
                passedlen += read;  
                if (read == -1) {  
                    break;  
                }  
                Log.d("AndroidClient","�ļ�������"+(passedlen*100/len)+"%/n");  
                fileOut.write(buf,0,read);  
            }  
            Log.d("AndroidClient","@@@�ļ��������"+savePath);
            
            fileOut.close();
            
        } catch (IOException e) {  
            e.printStackTrace();  
        }  
    } 
    
    private void sendMessage(String fileName) {  
        if (cs == null)  
            return;  
        
        cs.sendMessage(fileName);
        cs.sendMessage(sendMessage);  
    } 
    
    private boolean createConnection() { 
    	
        cs = new ClientSocket(ip, recv_port);  
        cs.createConnection();  
        
        Log.d("RECV", "���ӷ������ɹ�:");  
        
        return true;  
    }  
    
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	
	public void fileSend()
	{
		try
		{
			// �����ͷ�����������
			Socket socket = new Socket(ip, fil_port);
			
			// ����FilePojo����
			FileSerIo fpo = new FileSerIo();
			
			// ��ȡ�û������ļ���
			fileName = file_txt.getText().toString();

			// �趨�ļ���
			fpo.setFileName(fileName);

			// �趨�ļ���С
			File f = new File(filePath + fileName);
			long fileLength = f.length();
			fpo.setFileLength(fileLength);

			// ��ȡ�ļ����ݣ�������ת��Ϊbyte[]
			FileInputStream fis = new FileInputStream(filePath + fileName);

			/*
			byte[] fileContent = new byte[(int) fileLength];
			fis.read(fileContent, 0, (int) fileLength);
			fpo.setFileContent(fileContent);
			*/

			// ��FilePojo����fpoд��Socket clientָ���������
			long start = System.currentTimeMillis();
			
			/* object���ͷ�ʽ, android��֧��
			ObjectOutputStream oos = new ObjectOutputStream(
					socket.getOutputStream());
			oos.writeObject(fpo);
			oos.flush();
			oos.close();
			*/
			
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
			
			Toast.makeText(this.getApplicationContext(),
					"It takes " + (end - start) + "ms", Toast.LENGTH_LONG)
					.show();
		}
		catch (Exception ioe)
		{
			Log.i(TAG, ioe.toString());
			ioe.printStackTrace();
		}
	}
}
