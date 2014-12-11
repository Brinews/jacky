import java.io.*;

public class Main
{
	public static void main(String[] args) {

		//创建PDF文件
		System.out.println("开始创建PDF文件");
		hqzs_pdf hp = new hqzs_pdf();
		hp.CreatePdfDocument();

		//发送邮件
		System.out.println("开始发送PDF文件");
		hqzs_mail hm = new hqzs_mail();
		hm.SendToAll();

		System.out.println("完成。");
	}
}
