package beans;

import java.io.*;    

public class WebCounter extends Object {    
	private String currentRecord = null;//保存文本的变量    
	private BufferedReader file; //BufferedReader对象，用于读取文件数据    
	private String path;//文件完整路径名    
	public WebCounter() {    
	}    
	//ReadFile方法用来读取文件filePath中的数据，并返回这个数据    
	public String ReadFile(String filePath) throws FileNotFoundException    
	{    
		path = filePath;    
		//创建新的BufferedReader对象    
		file = new BufferedReader(new FileReader(path));    
		String returnStr =null;    
		try    
		{    
			//读取一行数据并保存到currentRecord变量中    
			currentRecord = file.readLine();    
		}    
		catch (IOException e)    
		{//错误处理    
			System.out.println("读取数据错误.");    
		}    
		if (currentRecord == null)    
			//如果文件为空    
			returnStr = "没有任何记录";    
		else    
		{//文件不为空    
			returnStr =currentRecord;    
		}    
		//返回读取文件的数据    
		return returnStr;    
	}    
	//ReadFile方法用来将数据counter+1后写入到文本文件filePath中    
	//以实现计数增长的功能    
	public void WriteFile(String filePath,String counter)
		throws FileNotFoundException    
	{    
		path = filePath;    
		//将counter转换为int类型并加一    
		int Writestr = Integer.parseInt(counter)+1;    
		try {    
			//创建PrintWriter对象，用于写入数据到文件中    
			PrintWriter pw = new PrintWriter(
								new FileOutputStream(filePath));    
			//用文本格式打印整数Writestr    
			pw.println(Writestr);    
			//清除PrintWriter对象    
			pw.close();    
		} catch(IOException e) {    
			//错误处理    
			System.out.println("写入文件错误"+e.getMessage());    
		}    
	}    
}
