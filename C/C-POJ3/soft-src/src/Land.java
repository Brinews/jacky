import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.sql.*;

public class Land extends JFrame 
{
	JFrame jf;
	JTextField textname=new JTextField();
	JPasswordField textmima=new JPasswordField();
	//JLabel label= new JLabel("ѧ����Ϣ����ϵͳ");
	JLabel labelname=new JLabel("�û���");
	JLabel labelmima=new JLabel("����");

	JButton btenter=new JButton("ȷ��");
	JButton btcancel=new JButton("���");

	public Land()
	{
		jf=this;
		setTitle("��½");
		Font f=new Font("������",Font.PLAIN,12);
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
				new JOptionPane().showMessageDialog(null,"�û�������Ϊ��!");
			}
			else if(textmima.getText().equals("")){
				new JOptionPane().showMessageDialog(null,"���벻��Ϊ��!");
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
						System.out.println("������ȷ");
						jf.setVisible(false);
						//�ر����ݿ�����
						database.cn.close();
						new Main();
					}
					else {
						System.out.println("����");
						new JOptionPane().showMessageDialog(null,"�û������������!","",JOptionPane.ERROR_MESSAGE);
					}
				}catch(Exception ex) {
					System.out.println(ex.getMessage());
				}
		}
		else{
			System.out.println("�������ݿⲻ�ɹ�!!!");
		}
	}
	public static void main(String args[]){
		new Land();
	}
}

