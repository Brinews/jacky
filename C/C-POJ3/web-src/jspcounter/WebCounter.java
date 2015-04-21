package beans;

import java.io.*;    

public class WebCounter extends Object {    
	private String currentRecord = null;//�����ı��ı���    
	private BufferedReader file; //BufferedReader�������ڶ�ȡ�ļ�����    
	private String path;//�ļ�����·����    
	public WebCounter() {    
	}    
	//ReadFile����������ȡ�ļ�filePath�е����ݣ��������������    
	public String ReadFile(String filePath) throws FileNotFoundException    
	{    
		path = filePath;    
		//�����µ�BufferedReader����    
		file = new BufferedReader(new FileReader(path));    
		String returnStr =null;    
		try    
		{    
			//��ȡһ�����ݲ����浽currentRecord������    
			currentRecord = file.readLine();    
		}    
		catch (IOException e)    
		{//������    
			System.out.println("��ȡ���ݴ���.");    
		}    
		if (currentRecord == null)    
			//����ļ�Ϊ��    
			returnStr = "û���κμ�¼";    
		else    
		{//�ļ���Ϊ��    
			returnStr =currentRecord;    
		}    
		//���ض�ȡ�ļ�������    
		return returnStr;    
	}    
	//ReadFile��������������counter+1��д�뵽�ı��ļ�filePath��    
	//��ʵ�ּ��������Ĺ���    
	public void WriteFile(String filePath,String counter)
		throws FileNotFoundException    
	{    
		path = filePath;    
		//��counterת��Ϊint���Ͳ���һ    
		int Writestr = Integer.parseInt(counter)+1;    
		try {    
			//����PrintWriter��������д�����ݵ��ļ���    
			PrintWriter pw = new PrintWriter(
								new FileOutputStream(filePath));    
			//���ı���ʽ��ӡ����Writestr    
			pw.println(Writestr);    
			//���PrintWriter����    
			pw.close();    
		} catch(IOException e) {    
			//������    
			System.out.println("д���ļ�����"+e.getMessage());    
		}    
	}    
}
