import java.io.*;
import java.util.Scanner;
import java.util.Date;
import java.text.SimpleDateFormat;

public class GlobalConf {

	public static String baseConf = "./conf/base.conf";
	public static String coverImage = "";
	public static String pdfFileName = "";
	//默认值
	public static int pageHeight = 560, pageWidth = 595;

	public static String mainCont = "";
	public static String suppCont = "";
	public static String subCont = "";
	public static String tendCont = "";
	public static String mailCont = "";

	private String FileRead(String fileName)
	{
		String ret = "";
		File fd = null;
		Reader in = null;

		//System.out.println("[" + fileName + "]");

		int c;
		try {
			fd = new File(fileName);
			in = new FileReader(fd);
		} catch (FileNotFoundException fn) {
			fn.printStackTrace();
		}

		try {
			while ((c = in.read()) > 0) {
				ret += (char) c;
			}
		} catch (IOException ie) {
			ie.printStackTrace();
		}

		return ret;
	}

	public void InitGlobalConf()
	{
		File fd = null;
		Reader in = null;
		int c;

		try {
			fd = new File(baseConf);
			in = new FileReader(fd);
		} catch (FileNotFoundException fnfe) {
			fnfe.printStackTrace();
		}

		String line = "";
		try {
			while ((c = in.read()) > 0) {
				//注释,进行忽略
				if ((char)c == '#') {
					while ((char)c != '\n') c = in.read();
				}

				line += (char) c;
				int pos = 0;


				if (c == '\n') {
					//System.out.println("<" + line + ">");
					if ((pos = line.indexOf("PageSize")) >= 0) {
						pos += 8;
						//读入两个数
						Scanner sin = new Scanner(line.substring(pos));
						pageWidth = sin.nextInt();
						pageHeight = sin.nextInt();
						if (pageWidth < 400 || pageWidth > 1400) pageWidth = 595;
						if (pageHeight < 500 || pageHeight > 1400) pageHeight = 560;
					}

					if ((pos = line.indexOf("DayImage")) >= 0) {
						pos += "DayImage".length();
						coverImage = line.substring(pos).trim();
					}

					if ((pos = line.indexOf("PdfFileName")) >= 0) {
						pos += "PdfFileName".length();
						pdfFileName = line.substring(pos).trim();

						Date now = new Date();
						SimpleDateFormat sdf = new SimpleDateFormat("yyyyMMdd");
						pdfFileName = sdf.format(now) + pdfFileName;
					}

					if ((pos = line.indexOf("MainPageContent")) >= 0) {
						pos += "MainPageContent".length();
						mainCont = line.substring(pos).trim();
						mainCont = FileRead(mainCont);
					}

					if ((pos = line.indexOf("SupplementContent")) >= 0) {
						pos += "SupplementContent".length();
						suppCont = line.substring(pos).trim();
						suppCont = FileRead(suppCont);
					}

					if ((pos = line.indexOf("TrendContent")) >= 0) {
						pos += "TrendContent".length();
						tendCont = line.substring(pos).trim();
						tendCont = FileRead(tendCont);
					}

					if ((pos = line.indexOf("SubPageContent")) >= 0) {
						pos += "SubPageContent".length();
						subCont = line.substring(pos).trim();
						subCont = FileRead(subCont);
					}

					if ((pos = line.indexOf("MailContent")) >= 0) {
						pos += "MailContent".length();
						mailCont = line.substring(pos).trim();
						mailCont = FileRead(mailCont);
					}

					line = "";
				}
			}
		}
		catch (IOException e) {
			e.printStackTrace();
		}
	}

	public GlobalConf()
	{
		InitGlobalConf();

		//For Debug
		System.out.println("Cover:" + coverImage);
		System.out.println("Main:" + mainCont);
	}
}
