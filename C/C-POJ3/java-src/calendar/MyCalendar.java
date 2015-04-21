/*
 * calendar application, 2015-04
 */

import java.awt.*;
import java.text.*;
import javax.swing.*;
import java.awt.event.*;
import java.util.*;

public class MyCalendar implements ItemListener  
{
	JFrame mainFrame;
	JLabel yearLabel,monthLabel,showTextLabel,timeLabel;
	JTextArea showArea;
	JComboBox yearBox,monthBox;
	JPanel panel_top,panel_down;
	Container con;

	String year[]=new String[9000];
	String month[]=new String[12];

	Date today;
	GregorianCalendar cal;

	int totalDays,startDay,i;
	String selectYear,selectMonth;

	Object [][] day=new Object[4][6];

	static final String head[]={"星期日","星期一","星期二","星期三",
									"星期四","星期五","星期六"};

	SimpleDateFormat timeFormat=new 
					SimpleDateFormat("yyyy年MM月dd日     HH:mm:ss");

	public MyCalendar()
	{
		mainFrame=new JFrame("*万年历*");
		con=mainFrame.getContentPane();

		yearLabel=new JLabel("选择年份:");
		monthLabel=new JLabel("选择月份:");

		//最大10000年
		for(int i=1900;i<10000;i++) {
			try {
				year[i-1900]=String.valueOf(i);
			}
			catch(Exception e) {
				JOptionPane.showMessageDialog(mainFrame,"出现异常");
			}
		}

		yearBox=new JComboBox(year);
		yearBox.setSelectedItem("2015");

		yearBox.addItemListener(this);

		for(int i=0;i<12;i++) {
			month[i] = String.valueOf(i);
			//(new Integer(i)).toString();
		}

		monthBox=new JComboBox(month);
		monthBox.setSelectedItem("5");

		monthBox.addItemListener(this);

		//添加顶部选择
		panel_top=new JPanel();
		panel_top.setLayout(new FlowLayout());

		panel_top.add(yearLabel);
		panel_top.add(yearBox);
		panel_top.add(monthLabel);
		panel_top.add(monthBox);

		con.add(panel_top,BorderLayout.NORTH);

		//添加中间日期 
		showArea=new JTextArea();
		showArea.setEditable(false);

		showDate(2015,5);
		con.add(showArea,BorderLayout.CENTER);

		//添加底部显示
		showTextLabel=new JLabel("现在时间是：");
		today=new Date();
		timeLabel=new JLabel(timeFormat.format(today));

		panel_down=new JPanel();
		panel_down.setLayout(new FlowLayout());
		panel_down.add(showTextLabel);
		panel_down.add(timeLabel);
		con.add(panel_down,BorderLayout.SOUTH);

		mainFrame.setSize(400,300);
		mainFrame.setVisible(true);
		mainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}

	public void showDate(int year,int month)
	{
		showArea.setText("");

		cal=new GregorianCalendar(year,month-1,1);

		totalDays=cal.getActualMaximum(Calendar.DAY_OF_MONTH);
		startDay=cal.get(Calendar.DAY_OF_WEEK)-1;

		//显示星期数
		for(i=0;i<head.length;i++)
			showArea.append(head[i]+"      ");
		showArea.append("\n");

		//输出第一天之前的空格，每个输出项8个空格
		for(i=0;i<startDay;i++) {
			showArea.append("                 ");
		}       

		//依次输出每一天，每一个输出项占8个字符宽度
		for(int days=1;days<=totalDays;days++)
		{
			if(days<10) {
				showArea.append("         "+days+"      ");
			}
			else if(days>=10) {
				showArea.append("        "+days+"    ");
			}

			i++; //判断是否过了一个星期

			if(i==7) {
				showArea.append("\n");
				i=0;
			}
		}
	}

	//日期切换事件
	public void itemStateChanged(ItemEvent e)
	{
		if(e.getStateChange()==ItemEvent.SELECTED)
		{ 
			selectYear=(String)(yearBox.getSelectedItem());
			selectMonth=(String)(monthBox.getSelectedItem());
			//更新日期显示
			showDate(Integer.parseInt(selectYear),Integer.parseInt(selectMonth));
		}
	}

	public static void main(String argv[])
	{
		new MyCalendar();
	}
}

