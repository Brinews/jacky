import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.sql.*;

public class Land extends JFrame 
{
	JFrame jf;
	JTextField textname=new JTextField();
	JPasswordField textmima=new JPasswordField();
	//JLabel label= new JLabel("学生信息管理系统");
	JLabel labelname=new JLabel("用户名");
	JLabel labelmima=new JLabel("密码");

	JButton btenter=new JButton("确定");
	JButton btcancel=new JButton("清空");

	public Land()
	{
		jf=this;
		setTitle("登陆");
		Font f=new Font("新宋体",Font.PLAIN,12);
		Container ctn=getContentPane();
		ctn.setLayout(null);

		labelname.setBounds(45,40,55,20);
		labelname.setFont(f);
		ctn.add(labelname);
		textname.setBounds(95,40,120,20);
		ctn.add(textname);

		labelmima.setBounds(45,70,45,20);
		ctn.add(labelmima);
		labelmima.setFont(f);
		textmima.setBounds(95,70,120,20);
		ctn.add(textmima);

		btenter.setBounds(90,110,60,20);
		btenter.setFont(f);
		ctn.add(btenter);
		Monitor_1 bt=new Monitor_1();
		btenter.addActionListener(bt);

		btcancel.setBounds(155,110,60,20);
		btcancel.setFont(f);
		ctn.add(btcancel);
		Monitor_2 btc=new Monitor_2();
		btcancel.addActionListener(btc);
		jf.setResizable(false);
		jf.setLocation(400, 400);
		jf.setSize(300,200);
		jf.setVisible(true);


	}

	class Monitor_1 implements ActionListener {
		public void actionPerformed (ActionEvent e){
			if(textname.getText().equals("")){
				new JOptionPane().showMessageDialog(null,"用户名不能为空!");
			}
			else if(textmima.getText().equals("")){
				new JOptionPane().showMessageDialog(null,"密码不能为空!");
			}
			else{
				String sql="select * from user where user_id = '" + textname.getText() + "' and password = '" + textmima.getText()+ "'";
				System.out.println(sql);
				Judge(sql);
			}
		}
	}

	class Monitor_2 implements ActionListener{
		public void actionPerformed(ActionEvent e){
			textname.setText("");
			textmima.setText("");

		}	
	}

	private void Judge(String sqlString) {

		if (database.joinDB()) {
			if (database.query(sqlString))
				try{
					if(database.rs.isBeforeFirst()) {
						System.out.println("密码正确");
						jf.setVisible(false);
						//关闭数据库连接
						database.cn.close();
						new Main();
					}
					else {
						System.out.println("错误");
						new JOptionPane().showMessageDialog(null,"用户名或密码错误!","",JOptionPane.ERROR_MESSAGE);
					}
				}catch(Exception ex) {
					System.out.println(ex.getMessage());
				}
		}
		else{
			System.out.println("连接数据库不成功!!!");
		}
	}
	public static void main(String args[]){
		new Land();
	}
}

