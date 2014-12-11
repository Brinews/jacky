import java.io.IOException;
import java.io.FileNotFoundException;
import java.io.OutputStream;
import java.io.FileOutputStream;

import java.util.Date;
import java.util.Calendar;
import java.util.TimeZone;
import java.util.Locale;
import java.util.ArrayList;
import java.text.SimpleDateFormat;

import com.itextpdf.text.Document;
import com.itextpdf.text.DocumentException;
import com.itextpdf.text.pdf.PdfWriter;
import com.itextpdf.text.PageSize;
import com.itextpdf.text.Paragraph;
import com.itextpdf.text.Image;
import com.itextpdf.text.Font;
import com.itextpdf.text.Element;
import com.itextpdf.text.Rectangle;
import com.itextpdf.text.BaseColor;
import com.itextpdf.text.BadElementException;
import com.itextpdf.text.pdf.BaseFont;
import com.itextpdf.text.pdf.draw.DrawInterface;
import com.itextpdf.text.pdf.PdfContentByte;
import com.itextpdf.text.pdf.PdfTemplate;
import com.itextpdf.text.Phrase;
import com.itextpdf.text.Chunk;
import com.itextpdf.text.Chapter;
import com.itextpdf.text.Section;
import com.itextpdf.text.pdf.PdfPCell;
import com.itextpdf.text.pdf.PdfTransition;
import com.itextpdf.text.pdf.PdfPTable;
import com.itextpdf.text.pdf.PdfPageEventHelper;
import com.itextpdf.text.pdf.ColumnText;
import com.itextpdf.text.pdf.draw.LineSeparator;

import java.sql.ResultSet;

/**
  根据需求创建PDF文件
  主要分为两类页
Author: LiangYang
Update v1.0: 2011-12-10
  */

//import com.itextpdf.text.pdf.PdfGraphics2D;

import java.net.MalformedURLException;
import java.net.URL;

class hqzs_pdf{

	//页眉页脚
	private static class Header extends PdfPageEventHelper {

		@Override
		public void onStartPage(PdfWriter pdfWriter, Document document) {
			ColumnText.showTextAligned(pdfWriter.getDirectContent(), Element.ALIGN_CENTER,
					new Phrase("股市行情分析", new Font(kai, 11)), GlobalConf.pageWidth - 50, GlobalConf.pageHeight - 25, 0);
		}

        @Override
        public void onEndPage(PdfWriter pdfWriter, Document document) {
			//super.onStartPage(pdfWriter, document);    
			//To change body of overridden methods use File | Settings | File Templates.
            ColumnText.showTextAligned(pdfWriter.getDirectContent(), Element.ALIGN_CENTER
					,new Phrase(String.format("第%d页",pdfWriter.getPageNumber()), new Font(kai, 10)), GlobalConf.pageWidth/2, 20, 0);
        }
    }

	//常规黑体字体
	public static final Font YH_BOLD;
	public static final Font YH_NORMAL;
	
	//黑体
	private static BaseFont msyh = null;
	private static BaseFont msyhbd = null;

	//宋体
	private static BaseFont sun = null;

	//楷体
	private static BaseFont kai = null;

	private static String[] listStr = {
		"总指标", "市场资金充裕性", "消费物价", "生产物价",
		"进出口", "经济增长", "宏观景气", "技术指标",
	};

	//数据库中的列名
	private static String[] colName = {
		"totalValue", "liquidity", "price_1", "price_2",
		"IE", "product_M", "Qinqi", "TechIndicator" 
	};

	//相关参考指标值
	private static double[] listVal = {0, 0, 0, 0, 0, 0, 0, 0};
	private double[] deltaDay = {0, 0, 0, 0, 0, 0, 0, 0};
	private double[] deltaWeek = {0, 0, 0, 0, 0, 0, 0, 0};

	//历史数据, 指数和上证指数

	//日收益率
	private double[][] histData = { 
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0}
	};

	//6个月度收益率
	private double[][] monthHist = { 
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0}
	};



	//上证指数
	private double[] histSHCI = { 
		0, 0, 0, 0, 0, 0, 0
		/*
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0}
		*/
	};

	private String[] histDate = {"", "", "", "", "", "", "", ""}; 

	//绘制趋势图的数据,从2005年开始
	private float[] trendMax = {-1000000, -1000000, -1000000, -1000000,
								-1000000, -1000000, -1000000, -1000000};
	private float[] trendMin = {1000000, 1000000, 1000000, 1000000,
								1000000, 1000000, 1000000, 1000000};
	private int trendTotal = 0;

	class Record{
		float[] val = {0, 0, 0, 0, 0, 0, 0, 0};
		public Record(float[] v) 
		{
			for (int i = 0; i < 8; i++) val[i] = v[i];
		}
	}

	private ArrayList<Record> trendData = new ArrayList<Record>(3000);

	/*********************数据库读取*************************************/

	private int hasRecord(hqzs_dbcon dbc, String nowDay, String tableName) 
	{
		int count = 0;
		String sql = "select * from " + tableName + " where date = '" + nowDay + "'";
		ResultSet rs = dbc.ReadDataSet(sql);

		try {
			while (rs.next()) {
				count++;
			}
		} catch (Exception e) {
			e.printStackTrace();
		}

		return count;
	}

	//读取给定天的八个指标值
	private double[] GetChinaTRecord(hqzs_dbcon dbc, String nowDay, String tableName)
	{
		double[] val = {0, 0, 0, 0, 0, 0, 0, 0};
		int idr = 0;

		//注意数据库中日期字段的规范(YYYY-MM-DD)
		String sql = "select * from " + tableName + " where date = '" + nowDay + "'";
		ResultSet rs = dbc.ReadDataSet(sql);

		try {
			while (rs.next()) {
				for (idr = 0; idr < 8; idr++) {
					Double d = new Double(rs.getString(colName[idr]));
					val[idr] = d.doubleValue();
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}

		return val;
	}

	//读取上证指数
	private double GetSHCIRecord(hqzs_dbcon dbc, String nowDay)
	{
		double val = 0.0;
		//double[] val = {0, 0, 0, 0, 0, 0, 0, 0};
		//int idr = 0;

		//注意数据库中日期字段的规范
		/* use COTrade; */
		String sql = "select * from COTrade.SHCI_1440 where date = '" + nowDay + "'";
		ResultSet rs = dbc.ReadDataSet(sql);

		try {
			while (rs.next()) {
				Double d = new Double(rs.getString("indexV"));
				val = d.doubleValue();
				/*
				for (idr = 0; idr < 8; idr++) {
					Double d = new Double(rs.getString(colName[idr]));
					val[idr] = d.doubleValue();
				}
				*/
			}
		} catch (Exception e) {
			e.printStackTrace();
		}

		return val;
	}

	//判断是否为工作日
	private boolean isWeekDay(int year, int month, int day)
	{
		//Date d = new Date(year, month, day);
		Calendar cal = Calendar.getInstance();
		cal.setTimeZone(TimeZone.getTimeZone("Asia/Shanghai"));
		cal.set(year, month-1, day);
		String str = cal.getTime().toString();

		if (str.indexOf("Sat") == 0 || str.indexOf("Sun") == 0) return true;

		return false;
	}

	//计算前一工作日的日期, 格式yyyy-MM-dd
	private String GetPrevDay(String nowDay)
	{
		int[][] DAYS = {
			{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
			{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
		};

		int isRunYear = 0;

		int year, month, day;
		String[] str = nowDay.split("-");
		year = (new Integer(str[0])).intValue();
		month = (new Integer(str[1])).intValue();
		day = (new Integer(str[2])).intValue();

		if (year % 4 == 0) isRunYear = 1;
		if (year % 100 == 0 && year % 400 != 0) isRunYear = 0;

		do {
			day = day - 1;
			if (day == 0) {
				month = month - 1;
				if (month == 0) {
					month = 12;
					year = year - 1;
				}
				day = DAYS[isRunYear][month-1];
			}
		} while (isWeekDay(year, month, day));

		return String.format("%4d-%02d-%02d", year, month, day);
	}


	//计算差X天的Delta值，以便后续判断走势
	private double[] ComputeDaysDelta(hqzs_dbcon dbc, String nowDay, int days)
	{
		int di = 0;
		double[] total = {0, 0, 0, 0, 0, 0, 0, 0}; //累计X天
		double[] dayVal = {0, 0, 0, 0, 0, 0, 0, 0};//当天

		while (++di <= days) {
			//计算前一天的日期，排除周末
			String prevDay = GetPrevDay(nowDay);
			//必须在数据库中存在该记录，否则继续向前推
			while (hasRecord(dbc, prevDay, "chinaTotal") == 0) {
				prevDay = GetPrevDay(prevDay);
			}
			nowDay = prevDay;

			//读取数据
			dayVal = GetChinaTRecord(dbc, prevDay, "chinaTotal");

			//累加数据
			for (int i = 0; i < 8; i++) {
				total[i] += dayVal[i];
			}
		}

		//求出平均值
		for (int i = 0; i < 8; i++) 
			total[i] = total[i] / days;

		//计算delta
		for (int i = 0; i < 8; i++)
			dayVal[i] = listVal[i] - total[i];

		return dayVal;
	}

	//月收益率
	private double[] GetMonthRecord(hqzs_dbcon dbc, int i, String tableName) 
	{
		double[] val = {0, 0, 0, 0, 0, 0, 0, 0};
		int idr = 0;

		//注意数据库中日期字段的规范
		String sql = "select * from " + tableName + " where date <= '" + 
						histDate[i] + "' and date >= '" + histDate[i+1] + "'";
		ResultSet rs = dbc.ReadDataSet(sql);

		try {
			while (rs.next()) {
				for (idr = 0; idr < 8; idr ++) {
					Double d = new Double(rs.getString(colName[idr]));
					//累计求和
					val[idr] += d.doubleValue();
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}

		return val;
	}

	//读取历史数据
	private void ReadHistRecord(hqzs_dbcon dbc)
	{
		for (int i = 0; i < 7; i++) {
			histData[i] = GetChinaTRecord(dbc, histDate[i], "chinaTotal");

			System.out.printf("%s :", histDate[i]);
			for (int j = 0; j < 8; j++) {
				System.out.printf("%s %.2f", colName[j], histData[i][j]);
			}
			System.out.println("");
		}

		//计算各个指标的月度收益率
		for (int i = 0; i < 7; i++) {
			monthHist[i] = GetMonthRecord(dbc, i, "chinaTotalPL");
		}

		for (int i = 0; i < 7; i++) {
			histSHCI[i] = GetSHCIRecord(dbc, histDate[i]);
		}
	}

	private void ReadTrendRecord(hqzs_dbcon dbc, String nowDay, String tableName)
	{
		float[] val = {0, 0, 0, 0, 0, 0, 0, 0};
		int idr = 0;

		trendTotal = 0;
		trendData.clear();

		//注意数据库中日期字段的规范
		String sql = "select * from " + tableName + " where date >= '2005-01-01' and date <= '" + nowDay + "'";
		ResultSet rs = dbc.ReadDataSet(sql);

		try {
			while (rs.next()) {
				for (idr = 0; idr < 8; idr++) {
					Float d = new Float(rs.getString(colName[idr]));
					//累计收益率
					val[idr] += d.floatValue();

					//找出上下限
					if (val[idr] > trendMax[idr]) trendMax[idr] = val[idr];
					if (val[idr] < trendMin[idr]) trendMin[idr] = val[idr];

				}

				Record r = new Record(val);

				for (int i = 0; i < 8; i++)
					System.out.printf("%03.2f  ", r.val[i]);
				System.out.println("");

				trendData.add(r);

				trendTotal++;
			}
		} catch (Exception e) {
			e.printStackTrace();
		}

		System.out.println("trendTotal=" + trendTotal);
		for (int i = 0; i < 8; i ++) {
			System.out.printf("%s  Max:%.2f, Min:%.2f\n", colName[i], trendMax[i], trendMin[i]);
		}
		System.out.println("");
	}

	private void DataInit()
	{
		//数据初始化，读取数据库
		hqzs_dbcon dbcon = new hqzs_dbcon();

		//7个历史日期初始化,包括当天日期histDate[0]
		InitHistDate(dbcon);

		if (dbcon.GetStatus() < 0) return;

		//读取当天数据
		listVal = GetChinaTRecord(dbcon, histDate[0], "chinaTotal");
		for (int i = 0; i < 8; i++) {
			System.out.printf("listVal[%d]=%.02f", i, listVal[i]);
		}
		System.out.println("");

		//计算前一工作日的delta
		deltaDay = ComputeDaysDelta(dbcon, histDate[0], 1);
		
		for (int i = 0; i < 8; i++) {
			System.out.printf("DeltaD[%d]=%.02f", i, deltaDay[i]);
		}
		System.out.println("");

		//计算前五个工作日的delta
		deltaWeek = ComputeDaysDelta(dbcon, histDate[0], 5);

		for (int i = 0; i < 8; i++) {
			System.out.printf("DeltaW[%d]=%.02f", i, deltaWeek[i]);
		}
		System.out.println("");

		//读取历史数据,累加日均收益率
		ReadHistRecord(dbcon);

		//收集趋势图数据
		ReadTrendRecord(dbcon, histDate[0], "chinaTotalPL");
	}

	/**********************PDF文件处理***********************************/

	static {

		try {
			msyh = BaseFont.createFont("./resource/ttf/msyh.ttf", 
					BaseFont.IDENTITY_H, BaseFont.EMBEDDED);

			msyhbd = BaseFont.createFont("./resource/ttf/msyhbd.ttf", 
					BaseFont.IDENTITY_H, BaseFont.EMBEDDED);

			sun = BaseFont.createFont("./resource/ttf/simsun.ttc,1", 
					BaseFont.IDENTITY_H, BaseFont.EMBEDDED);

			kai = BaseFont.createFont("./resource/ttf/simkai.ttf", 
					BaseFont.IDENTITY_H, BaseFont.EMBEDDED);

		} catch (DocumentException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}

		YH_BOLD = new Font(msyhbd, 14);
		YH_NORMAL = new Font(msyh, 14);
	}


	public hqzs_pdf()
	{
	}

	//设置显示效果
	class TransitionDuration extends PdfPageEventHelper {
		public void onStartPage(PdfWriter writer, Document document) {
			writer.setTransition(new PdfTransition(PdfTransition.DISSOLVE, 3));
			writer.setDuration(5);
		}
	}

	//添加图片,居中, 并对图片进行缩放处理，以便适应文档大小
	private void DrawImage(Document docm, String jpgName) 
		throws BadElementException
	{
		Image img = null;

		//System.out.println("JpgName:" + jpgName);

		try {

			img = Image.getInstance(jpgName);

			float imgWidth = img.getPlainWidth();
			float imgHeight = img.getPlainHeight();
			float percent = (float)100.0;

			//System.out.println("W=" + imgWidth + "H=" + imgHeight);

			if (imgWidth > GlobalConf.pageWidth*0.9) {
				percent *= GlobalConf.pageWidth*0.9/imgWidth;
			}

			if (imgHeight > GlobalConf.pageHeight*0.53) {
				if (percent > GlobalConf.pageHeight * 53 / imgHeight) {
					percent = GlobalConf.pageHeight * 53 / imgHeight;
				}
			}

			System.out.println("P=" + percent);

			img.scalePercent((float)percent);
			//img.scaleToFit(GlobalConf.pageWidth * 0.6f, GlobalConf.pageHeight * 0.6f);
			img.setAlignment(Element.ALIGN_CENTER);

		} catch (MalformedURLException mue) {
			System.err.println(mue.getMessage());
		} catch (IOException e) {
			System.err.println(e.getMessage());
		}

		try {
			docm.add(img);
		} catch (DocumentException de) {
			System.err.println(de.getMessage());
		}
	}

	//写中文数据
	private void WriteCJKFonts(Document docm, int size, String content)
	{
		try {
			BaseFont bf = BaseFont.createFont("STSongStd-Light", 
					"UniGB-UCS2-H", BaseFont.NOT_EMBEDDED);
			Font font = new Font(bf, size, Font.NORMAL);
			docm.add(new Paragraph(content, font));

		} catch (IOException e) {
			System.err.println(e.getMessage());
		} catch (DocumentException de) {
			System.err.println(de.getMessage());
		}
	}

	/* create the pdf doc */
	public void CreatePdfDocument() 
	{
		//初始化相关配置
		GlobalConf gc = new GlobalConf();

		//数据初始化
		DataInit();

		//设置页面大小
		Rectangle pagesize = new Rectangle(GlobalConf.pageWidth, GlobalConf.pageHeight);
		Document docm = new Document(pagesize,(float)GlobalConf.pageWidth*0.05f,(float)GlobalConf.pageWidth*0.05f,
				(float)GlobalConf.pageHeight*0.05f,(float) GlobalConf.pageHeight*0.1f);
		PdfWriter pwriter = null;


		try {

			try {
				//初始化pdf读写对象
				pwriter = PdfWriter.getInstance(docm, new FileOutputStream(GlobalConf.pdfFileName));
			} catch (FileNotFoundException fnfe) {
				System.err.println(fnfe.getMessage());
			}

			//设置页眉页脚
			Header header = new Header();
			pwriter.setPageEvent(header);

			pwriter.setViewerPreferences(pwriter.PageModeUseOutlines);
			//pwriter.setViewerPreferences(PdfWriter.PageModeFullScreen);
			//pwriter.setPageEvent(new TransitionDuration());

			docm.addAuthor("Liang Yang");
			docm.addSubject("PDF File AutoYielded by Software");

			docm.open();

			//制作封面
			MakePDFCover(docm);

			//制作目录
			MakePDFIndex(docm);
			//docm.newPage();

			//写后续9页内容
			for (int i = 1; i <= 8; i++) {

				Paragraph cTitle = new Paragraph(listStr[i-1], YH_NORMAL);
				cTitle.setSpacingBefore(10);
				Chapter chap = new Chapter(cTitle, i);

				//Paragraph subTitle = new Paragraph("Section " + i, YH_NORMAL);
				//Section sect = chap.addSection(subTitle, 1);
				//sect.setBookmarkOpen(false);

				String str = BuildFirstDesc(i-1);
				System.out.println("Page" + i + " : " + str);

				if (i == 1) {
				//第一页,totalValue判断，内容较多
					AddFirstDescription(chap, str);
				} else {
					AddDescription(chap, str);
				}

				//添加1.2表格
				AddHistTable(chap, i-1);

				//添加1.3走势说明
				AddTendDesc(chap);

				//添加章节
				docm.add(chap);

				//绘制每页中图像, 包括温度计和走势图
				MakePageImage(pwriter, (float)(listVal[i-1]), i-1);

			}

		
		} catch (DocumentException de) {
			System.err.println(de.getMessage());
		}

		docm.close();
	}

	public void MakePDFCover(Document docm) throws DocumentException
	{
		Paragraph pg = new Paragraph("中国股市大盘", new Font(msyhbd, 32));
		pg.setIndentationLeft(5);
		docm.add(pg);

		pg = new Paragraph("中期走势 e", new Font(msyh, 26));
		pg.setIndentationLeft(30);
		docm.add(pg);

		Date date = new Date();
		SimpleDateFormat df = new SimpleDateFormat("yyyy年MM月dd日");
		pg = new Paragraph(df.format(date), YH_NORMAL);
		pg.setAlignment(Element.ALIGN_RIGHT);
		docm.add(pg);

		//分割线
		docm.add(new LineSeparator(0.5f, 100, new BaseColor(181, 1, 0), 0, -5));

		//图片显示
		DrawImage(docm, GlobalConf.coverImage);

		//单位
		pg = new Paragraph("__________", new Font(msyh, 14, Font.NORMAL, new BaseColor(5, 200, 70)));
		pg.setSpacingBefore(10);
		pg.setAlignment(Element.ALIGN_RIGHT);
		docm.add(pg);
		pg = new Paragraph("明博金融", new Font(msyh, 14, Font.NORMAL, new BaseColor(5, 200, 70)));
		//pg.setSpacingAfter(60);
		pg.setAlignment(Element.ALIGN_RIGHT);
		docm.add(pg);

	}

	private void AddCatalog(Document docm, String catg, int pno) throws DocumentException
	{
		String fmStr = catg;
		int i = fmStr.length();
		while (i++ < 21) fmStr += "····";

		Paragraph pg = new Paragraph(String.format("%s%2d", fmStr, pno), 
										new Font(msyh, 13));

		pg.setIndentationLeft(GlobalConf.pageWidth*0.05f + 16);
		docm.add(pg);
	}

	public void MakePDFIndex(Document docm) throws DocumentException
	{
		docm.newPage();
		docm.add(new Paragraph(" "));
		
		String first = "总指标";
		int len = first.length();
		while (len++ < 15) first += "    ";
		first += " ";

		Paragraph pg = new Paragraph(String.format("%s%2d", first, 3), 
										new Font(msyh, 16, Font.BOLD, new BaseColor(10, 10, 225)));
		pg.setAlignment(Element.ALIGN_JUSTIFIED);
		pg.setSpacingBefore(GlobalConf.pageHeight*0.20f);
		pg.setSpacingAfter(30);
		pg.setIndentationLeft(GlobalConf.pageWidth*0.05f);
		docm.add(pg);

		for (int i = 3; i < 10; i++) {
			AddCatalog(docm, listStr[i-2], i+1);
		}
	}

	public void DrawThermoImage(PdfWriter pwriter, float idi) throws DocumentException
	{
		Image img = null;
		PdfContentByte pcb = pwriter.getDirectContentUnder();

		int bot = GlobalConf.pageHeight - 195, left = 70;

		try {
			img = Image.getInstance("./resource/thermo.jpg");
			float imgW = img.getPlainWidth();
			float imgH = img.getPlainHeight();

			pcb.addImage(img, imgW, 0, 0, imgH, left, bot);

		} catch (MalformedURLException mue) {
			System.err.println(mue.getMessage());
		} catch (IOException e) {
			System.err.println(e.getMessage());
		}
	}

	public void DrawThermometer(PdfWriter pwriter, float idi)
	{
		int tp = 60;
		int lp = 30;
		PdfContentByte pcb = pwriter.getDirectContent();

		//绘制外沿
		/* 80 * 100 */
		/*
		pcb.rectangle(lp + 10, GlobalConf.pageHeight - 120 - tp, 100, 120);
		pcb.rectangle(lp + 20, GlobalConf.pageHeight - 110 - tp, 80, 5);
		pcb.rectangle(lp + 55, GlobalConf.pageHeight - 105 - tp, 10, 90);
		pcb.rectangle(lp + 20, GlobalConf.pageHeight - 15 - tp, 80, 5);
		*/


		/* 绘制刻度 */
		/*
		for (int i = 0; i < 10; i++) {
			int H = GlobalConf.pageHeight - ((9-i)*9 + 15 + tp);
			pcb.moveTo(lp + 55, H);
			pcb.lineTo(lp + 50, H);

			if (i == 4) {
				pcb.lineTo(lp + 45, H);
			}
		}
		*/

		/* 绘制刻度 */
		/*
		for (int i = 0; i < 10; i++) {
			int H = GlobalConf.pageHeight - ((9-i)*9 + 15 + tp);
			pcb.moveTo(lp + 65, H);
			pcb.lineTo(lp + 70, H);

			if (i == 4) {
				pcb.lineTo(lp + 75, H);
			}
		}

		pcb.stroke();
		*/

		if (idi > 0.25 && idi <= 1) {
			pcb.setColorStroke(new BaseColor(214, 3, 9)); /* red */
		} else if (idi <= 0.25 && -0.25 <= idi) {
			pcb.setColorStroke(new BaseColor(232, 221, 15)); /* yellow */
		} else if (idi < -0.25 && idi >= -1) {
			pcb.setColorStroke(new BaseColor(34, 177, 76)); /* green */
		}
		
		//显示当前值
		pcb.rectangle(lp + 57, GlobalConf.pageHeight - 105 - tp, 6, (int)(75*(idi+1)/2.0));
		pcb.rectangle(lp + 58, GlobalConf.pageHeight - 105 - tp, 4, (int)(75*(idi+1)/2.0));
		pcb.rectangle(lp + 59, GlobalConf.pageHeight - 105 - tp, 2, (int)(75*(idi+1)/2.0));
		pcb.rectangle(lp + 60, GlobalConf.pageHeight - 105 - tp, 2, (int)(75*(idi+1)/2.0));

		pcb.stroke();
	}

	//做映射
	private String MapValStr(double val) 
	{
		if (val > 0.25) return "看多";
		if (val <= 0.25 && val >= -0.25) return "看平";
		if (val < -0.25) return "看空";

		return "";
	}

	private String MapDeltaStr(double val)
	{
		if (val > 0.1) return "更乐观";
		if (val < -0.1) return "更悲观";

		return "判断不变";
	}

	private String MapDeltaAll(double[] delta)
	{
		String str = "主要原因：";
		String le = "", bei = "";
		{

			for (int i = 1; i < 8; i++) {
				if (delta[i] > 0.1)
					le += listStr[i] + "、";
			}

			if (le.length() > 0) {
				le = le.substring(0, le.length()-1);
				le += "更为乐观";
			}

			for (int i = 1; i < 8; i++) {
				if (delta[i] < -0.1) {
					bei += listStr[i] + "、";
				}
			}

			if (bei.length() > 0) {
				bei = bei.substring(0, bei.length()-1);
				bei += "更为悲观。";
			}
		}

		if (le.length() == 0 && bei.length() == 0) return "";
		if (le.length() > 0 && bei.length() > 0) return str + le + ",而" + bei;
		if (le.length() == 0 && bei.length() > 0) return str + bei;
		if (le.length() > 0 && bei.length() == 0) return str + le + "。";

		return "";
	}

	private String ManualReplace(String desc, String oldS, String newS)
	{
		String str = "";
		for (int i = 0; i < desc.length(); i++) {
			String ch = desc.substring(i, i+1);
			if (oldS.equals(ch)) {
				//System.out.println(ch);
				str += newS;
			}
			else {
				str += ch;
			}
		}

		return str;
	}

	//动态构造内容
	private String BuildFirstDesc(int idr)
	{
		String desc = "";
		if (idr == 0) {
			String repStr = MapValStr(listVal[idr]);

			desc = GlobalConf.mainCont;
			
			//进行替换
			String digit = String.format("%.2f", listVal[idr]);
			desc = ManualReplace(desc, "①", repStr);
			desc = ManualReplace(desc, "②", digit);

			//System.out.println(repStr + "==>" + desc);

			//每天判断及原因
			repStr = MapDeltaStr(deltaDay[idr]);
			desc = ManualReplace(desc, "③", repStr);

			repStr = MapDeltaAll(deltaDay);
			desc = ManualReplace(desc, "④", repStr);

			//每周判断及原因
			repStr = MapDeltaStr(deltaWeek[idr]);
			desc = ManualReplace(desc, "⑤", repStr);

			repStr = MapDeltaAll(deltaWeek);
			desc = ManualReplace(desc, "⑥", repStr);

		} else {
			String repStr = listStr[idr];

			desc = GlobalConf.subCont;

			//名称替换
			desc = ManualReplace(desc, "①", repStr);
			
			//判断替换
			String digit = String.format("%.2f", listVal[idr]);
			repStr = MapValStr(listVal[idr]);
			desc = ManualReplace(desc, "②", repStr);
			desc = ManualReplace(desc, "③", digit);

			repStr = MapDeltaStr(deltaWeek[idr]);
			desc = ManualReplace(desc, "④", repStr);
		}

		return desc;
	}

	//添加"valueTotal"图一的描述
	private void AddFirstDescription(Chapter chap, String txt) throws DocumentException
	{
		Paragraph pg = new Paragraph(txt, new Font(msyh, 11));
		pg.setAlignment(Element.ALIGN_JUSTIFIED);
		pg.setSpacingBefore(3);
		pg.setIndentationLeft(130);

		chap.add(pg);

		pg = new Paragraph(GlobalConf.suppCont, new Font(kai, 9));
		pg.setAlignment(Element.ALIGN_JUSTIFIED);
		pg.setSpacingBefore(5);
		pg.setIndentationLeft(90);
		
		chap.add(pg);
	}

	//添加其他指标-温度计相关的描述
	private void AddDescription(Chapter chap, String txt) throws DocumentException
	{
		Paragraph pg = new Paragraph(txt, new Font(msyh, 12));
		pg.setAlignment(Element.ALIGN_JUSTIFIED);
		pg.setSpacingBefore(6);
		pg.setIndentationLeft(140);

		chap.add(pg);

		pg = new Paragraph("注：本指标的判断分为\"多头\"，\"中性\"，\"空头\"三级。0.25以上为多头，-0.25一下为空头，【-0.25,025】之间为中性。该指标对未来一个月左右大盘走势作出预测。", new Font(kai, 11));
		pg.setAlignment(Element.ALIGN_JUSTIFIED);
		pg.setSpacingBefore(3);
		pg.setIndentationLeft(140);
		pg.setSpacingAfter(15);
		
		chap.add(pg);
	}

	//历史日期初始化
	private void InitHistDate(hqzs_dbcon dbc)
	{
		int year, month, day;

		Date now = new Date();

		SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd");
		histDate[0] = df.format(now);

		while (hasRecord(dbc, histDate[0], "chinaTotal") == 0) {
			histDate[0] = GetPrevDay(histDate[0]);
		}

		Integer I = new Integer(histDate[0].substring(0, 4));
		year = I.intValue();

		I = new Integer(histDate[0].substring(5, 7));
		month = I.intValue();

		System.out.println("Year="+year+",Month=" + month + ".");
		TimeZone tz = TimeZone.getTimeZone("Asia/Shanghai");
		Calendar cal = Calendar.getInstance();

		int[] D = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
		for (int i = 1; i < 8; i++) {

			month = month - 1;

			if (month <= 0) {
				month = 12;
				year = year - 1;
			}

			day = D[month-1];

			//Date d = new Date(year, month, day);

			//排除周末
			//d.setHours(d.getHours()-8);
			//String dow = new SimpleDateFormat("E").format(d);
			cal.setTimeZone(tz);
			cal.set(year, month-1, day);
			String dow = cal.getTime().toString();
			System.out.println("dow=" + dow );
			
			if (dow.indexOf("Sat") == 0) day -= 1;
			if (dow.indexOf("Sun") == 0) day -= 2;

			histDate[i] = String.format("%4d-%02d-%02d", year, month, day);

			while (hasRecord(dbc, histDate[i], "chinaTotal") == 0) {
				histDate[i] = GetPrevDay(histDate[i]);
			}

			System.out.println("Date=" + histDate[i]);
		}
	}

	//添加历史数据表
	private void AddHistTable(Chapter chap, int idr) throws DocumentException
	{
		Paragraph pg = new Paragraph(String.format("表%d:系统预测值和大盘实际走势", idr+1), new Font(kai, 12));
		pg.setSpacingBefore(3);
		pg.setSpacingAfter(5);
		pg.setAlignment(Element.ALIGN_CENTER);
		chap.add(pg);

		PdfPTable pTable = new PdfPTable(4);
		pTable.setWidths(new int[]{1,1,1,1});
		//pTable.setTotalWidth(GlobalConf.pageWidth - 80);
		//添加标题
		PdfPCell cell = new PdfPCell(new Phrase("日期", new Font(msyh, 10)));
		cell.setHorizontalAlignment(Element.ALIGN_CENTER); //水平居中
		cell.setVerticalAlignment(Element.ALIGN_MIDDLE); //垂直居中
		pTable.addCell(cell);
		
		cell = new PdfPCell(new Phrase("当前预测值(%)", new Font(msyh, 10)));
		cell.setHorizontalAlignment(Element.ALIGN_CENTER); //水平居中
		cell.setVerticalAlignment(Element.ALIGN_MIDDLE); //垂直居中
		pTable.addCell(cell);

		cell = new PdfPCell(new Phrase("月度收益率(%)", new Font(msyh, 10)));
		cell.setHorizontalAlignment(Element.ALIGN_CENTER); //水平居中
		cell.setVerticalAlignment(Element.ALIGN_MIDDLE); //垂直居中
		pTable.addCell(cell);

		cell = new PdfPCell(new Phrase("上证指数", new Font(msyh, 10)));
		cell.setHorizontalAlignment(Element.ALIGN_CENTER); //水平居中
		cell.setVerticalAlignment(Element.ALIGN_MIDDLE); //垂直居中
		pTable.addCell(cell);

		//添加记录
		for (int i = 0; i < 7; i++) {
			//System.out.println(histDate[i]);

			String val = "";
			cell = new PdfPCell(new Phrase(histDate[i], new Font(kai, 11)));
			cell.setHorizontalAlignment(Element.ALIGN_CENTER); //水平居中
			cell.setVerticalAlignment(Element.ALIGN_MIDDLE); //垂直居中
			pTable.addCell(cell);

			if (idr > 0) {
				/* sign */
				val = String.format("%.2f", 100 * histData[i][idr]);
			}
			else {
				val = String.format("%.2f", 100 * histData[i][idr]);
			}

			cell = new PdfPCell(new Phrase(val, new Font(kai, 11)));
			cell.setNoWrap(true);
			cell.setHorizontalAlignment(Element.ALIGN_CENTER); //水平居中
			cell.setVerticalAlignment(Element.ALIGN_MIDDLE); //垂直居中
			pTable.addCell(cell);

			val = String.format("%.2f", 100 * monthHist[i][idr]);
			cell = new PdfPCell(new Phrase(val, new Font(kai, 11)));
			cell.setNoWrap(true);
			cell.setHorizontalAlignment(Element.ALIGN_CENTER); //水平居中
			cell.setVerticalAlignment(Element.ALIGN_MIDDLE); //垂直居中
			pTable.addCell(cell);

			val = String.format("%.2f", histSHCI[i]);
			cell = new PdfPCell(new Phrase(val, new Font(kai, 11)));
			cell.setHorizontalAlignment(Element.ALIGN_CENTER); //水平居中
			cell.setVerticalAlignment(Element.ALIGN_MIDDLE); //垂直居中
			pTable.addCell(cell);
		}

		chap.add(pTable);
	}

	//添加趋势图说明
	private void AddTendDesc(Chapter chap)
	{
		//System.out.println(GlobalConf.tendCont);

		Paragraph pg = new Paragraph(GlobalConf.tendCont, 
									new Font(sun, 11));
		pg.setAlignment(Element.ALIGN_JUSTIFIED);
		pg.setSpacingBefore(5);

		chap.add(pg);
	}

	//绘制坐标单位
	private void AddCoordUnit(PdfWriter pwriter, int idr) throws DocumentException
	{
		int tp = GlobalConf.pageWidth / 2 - 130, lp = 50;
		int w = 260, h = 100;

		PdfContentByte pcb = pwriter.getDirectContent();

		//显示坐标单位
		PdfTemplate tmplat = pcb.createTemplate(33, 11);
		tmplat.beginText();
		tmplat.setFontAndSize(kai, 11);
		tmplat.showText("金额");
		tmplat.endText();
		pcb.addTemplate(tmplat, tp+1, lp + h - 5);

		PdfTemplate tmp = pcb.createTemplate(33, 11);
		tmp.beginText();
		tmp.setFontAndSize(kai, 11);
		tmp.showText("时间");
		tmp.endText();
		pcb.addTemplate(tmp, tp + w - 5, lp + 2);

		for (int i = 2005; i <= 2012; i++) {
			tmp = pcb.createTemplate(33, 10);
			tmp.beginText();
			tmp.setFontAndSize(kai, 10);
			tmp.showText(String.format("%d", i));
			tmp.endText();
			pcb.addTemplate(tmp, tp + 5 + (i-2005)*(w-25)/7, lp - 5);
			
			pcb.moveTo(tp + 10 + (i-2005)*(w-25)/7, lp + 5);
			pcb.lineTo(tp + 10 + (i-2005)*(w-25)/7, lp + 7);
			pcb.stroke();
		}

		//坐标值说明
		/*
		PdfTemplate tmp2 = pcb.createTemplate(250, 11);
		tmp2.beginText();
		tmp2.setFontAndSize(kai, 11);
		tmp2.showText(String.format(" 收益率区间:[%.2f,%.2f]", trendMin[idr], trendMax[idr]));
		tmp2.endText();
		pcb.addTemplate(tmp2, tp + 30, (int)(lp + h - 5));
		*/

		float tm = (trendMax[idr] - trendMin[idr]) / 7;
		for (int i = 0; i < 8; i++) {
			tmp = pcb.createTemplate(18, 10);
			tmp.beginText();
			tmp.setFontAndSize(kai, 9);
			tmp.showText(String.format("%1.1f", trendMin[idr] + i * tm));
			tmp.endText();
			pcb.addTemplate(tmp, tp - 11, lp + 5 + i*(h-18)/7);
			
			pcb.moveTo(tp + 10, lp + 5 + i*(h-18)/7);
			pcb.lineTo(tp + 12, lp + 5 + i*(h-18)/7);
			pcb.stroke();
		}

		if (idr == 0) {
			tmp = pcb.createTemplate(390, 10);
			tmp.beginText();
			tmp.setFontAndSize(kai, 9);
			tmp.showText("注:将日收益率从2005年1月1日加总到t天得到\"t天的累计收益率\"");
			tmp.endText();
			pcb.addTemplate(tmp, 10, 5);
		}
	}

	//根据名称来读数据库
	private void DrawTrend(PdfWriter pwriter, String col, int idr) throws DocumentException
	{
		//起点
		int tp = GlobalConf.pageWidth / 2 - 130, lp = 50;
		int w = 260, h = 100;

		PdfContentByte pcb = pwriter.getDirectContent();

		pcb.setColorStroke(new BaseColor(0, 0, 0));

		//绘制外框
		pcb.rectangle(tp - 15, lp-10, w+32, h+5+10);

		//绘制横坐标
		pcb.moveTo(tp+10, lp + 5);
		pcb.lineTo(tp+w-10, lp + 5);

		pcb.lineTo(tp+w-13, lp + 2);
		pcb.moveTo(tp+w-10, lp + 5);
		pcb.lineTo(tp+w-13, lp + 8);

		//绘制纵坐标
		pcb.moveTo(tp+10, lp+5);
		pcb.lineTo(tp+10, lp+h-10);

		pcb.lineTo(tp+10-3, lp+h-13);
		pcb.moveTo(tp+10, lp+h-10);
		pcb.lineTo(tp+10+3, lp+h-13);

		pcb.moveTo(tp+10, lp+5);

		//绘制该指标下的趋势图, 170*90
		float hSect = trendMax[idr] - trendMin[idr];
		float wSect = trendTotal;

		if (wSect == 0) {
			pcb.stroke();
			return;
		}

		float sampH = 1.0f * (h - 15) / hSect;
		float sampW = 1.0f * (w - 30) / wSect;
		int samp = 1; //抽样间隔

		pcb.moveTo(tp + 10, lp + 5 +(int)((trendData.get(0).val[idr] - trendMin[idr])*sampH));

		if (sampW <= 1) {
			samp = (int)(1.0f/sampW);
			sampW = 1;
		}

		System.out.println("samp=" + samp + "SampW=" + sampW + "SampH=" + sampH);

		for (int i = 0; i < (w - 30); i ++) {
			//抽样取值
			if (i >= trendTotal) break;
			int x = (int)(tp + 10 + sampW*(i+1));
			int y = lp + 5 + (int)((trendData.get(i*samp).val[idr] - trendMin[idr])*sampH);

			System.out.printf("(%.2f, %d, %d)", trendData.get(i*samp).val[idr], x, y);

			pcb.lineTo(x, y);
		}
		System.out.println("");
		/*
		for (int i = 0; i < 90; i++) {
			pcb.lineTo((int)(tp + 10 + i*1.5f), lp+5+i);
		}
		*/

		pcb.stroke();
	}

	//每页绘制图像
	public void MakePageImage(PdfWriter pwriter, float idi, int idr) throws DocumentException
	{
		DrawThermometer(pwriter, idi);
		DrawThermoImage(pwriter, idi);

		//读取数据库来进行绘走势图
		DrawTrend(pwriter, colName[idr], idr);
		AddCoordUnit(pwriter, idr);
	}
}
