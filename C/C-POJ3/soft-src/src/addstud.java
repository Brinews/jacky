import java.awt.event.*;
import java.awt.*;
import javax.swing.*;
import java.sql.*;

public class addstud extends JInternalFrame {
	
	private JButton butCancel,butOk;
    private JLabel jLabel1,jLabel2,jLabel3,jLabel4,jLabel5,jLabel6,jLabel7;
    private JLabel jLabel8,jLabel9,jLabel10,jLabel11,jLabel12,jLabel13;

    private JTextField jtf1,jtf2,jtf3,jtf4,jtf5,jtf6,jtf7;
    private JTextField jtf8,jtf9,jtf10,jtf11,jtf12,jtf13;

    public addstud() {//�չ���
    super("���ѧ����Ϣ");
        initComponents();
        
        this.setVisible(true);
        this.setClosable(true);
        this.setSize(260,510);
        
   }

    private void initComponents() {
    	
    	//ʵ��������ӿؼ�
        jLabel1 = new JLabel();
        jLabel2 = new JLabel();
        jLabel3 = new JLabel();
        jLabel4 = new JLabel();
        jLabel5 = new JLabel();
        jLabel6 = new JLabel();
        jLabel7 = new JLabel();       

        jLabel8 = new JLabel();       
        jLabel9 = new JLabel();       
        jLabel10 = new JLabel();       
        jLabel11 = new JLabel();       
        jLabel12 = new JLabel();       
        jLabel13 = new JLabel();       

        jtf1=new JTextField();
        jtf2=new JTextField();
        jtf3=new JTextField();
        jtf4=new JTextField();
        jtf5=new JTextField();
        jtf6=new JTextField();
        jtf7=new JTextField();     

        jtf8=new JTextField();     
        jtf9=new JTextField();     
        jtf10=new JTextField();     
        jtf11=new JTextField();     
        jtf12=new JTextField();     
        jtf13=new JTextField();     

        butOk = new JButton();
        butCancel = new JButton();       
        //ʵ��������ӿؼ����

        getContentPane().setLayout(null);//���ò���
        
        
        //Ϊ�ռ���þ���λ��
        jLabel1.setText("ѧ ��");
        getContentPane().add(jLabel1);
        jLabel1.setBounds(30, 30, 70, 20);

        jLabel2.setText("�� ��");
        getContentPane().add(jLabel2);
        jLabel2.setBounds(30, 60, 70, 20);

        jLabel3.setText("�� ��");
        getContentPane().add(jLabel3);
        jLabel3.setBounds(30, 90, 60, 20);
        
        jLabel4.setText("�� ��");
        getContentPane().add(jLabel4);
        jLabel4.setBounds(30, 120, 70, 20);

        jLabel5.setText("������ò");
        getContentPane().add(jLabel5);
        jLabel5.setBounds(30, 150, 70, 20);
        
        jLabel6.setText("ϵ ��");
        getContentPane().add(jLabel6);
        jLabel6.setBounds(30, 180, 70, 20);
        
        jLabel7.setText("�� ��");
        getContentPane().add(jLabel7);
        jLabel7.setBounds(30, 210, 70, 20);

        /**/

        jLabel8.setText("��������");
        getContentPane().add(jLabel8);
        jLabel8.setBounds(30, 240, 70, 20);

        jLabel9.setText("�� ��");
        getContentPane().add(jLabel9);
        jLabel9.setBounds(30, 270, 70, 20);

        jLabel10.setText("��ͥסַ");
        getContentPane().add(jLabel10);
        jLabel10.setBounds(30, 300, 70, 20);

        jLabel11.setText("��Уסַ");
        getContentPane().add(jLabel11);
        jLabel11.setBounds(30, 330, 70, 20);

        jLabel12.setText("������ϵ�绰");
        getContentPane().add(jLabel12);
        jLabel12.setBounds(30, 360, 70, 20);

        jLabel13.setText("��ͥ��ϵ�绰");
        getContentPane().add(jLabel13);
        jLabel13.setBounds(30, 390, 70, 20);

        /**/
                
        getContentPane().add(jtf1);
        jtf1.setBounds(100, 30, 130, 24);

        getContentPane().add(jtf2);
        jtf2.setBounds(100, 60, 130, 24);

        getContentPane().add(jtf3);
        jtf3.setBounds(100, 90, 130, 24);
        
        getContentPane().add(jtf4);
        jtf4.setBounds(100, 120, 130, 24);
        
        getContentPane().add(jtf5);
        jtf5.setBounds(100, 150, 130, 24);
        
        getContentPane().add(jtf6);
        jtf6.setBounds(100, 180, 130, 24);
        
        getContentPane().add(jtf7);
        jtf7.setBounds(100, 210, 130, 24);

        /**/

        getContentPane().add(jtf8);
        jtf8.setBounds(100, 240, 130, 24);

        getContentPane().add(jtf9);
        jtf9.setBounds(100, 270, 130, 24);

        getContentPane().add(jtf10);
        jtf10.setBounds(100, 300, 130, 24);

        getContentPane().add(jtf11);
        jtf11.setBounds(100, 330, 130, 24);

        getContentPane().add(jtf12);
        jtf12.setBounds(100, 360, 130, 24);

        getContentPane().add(jtf13);
        jtf13.setBounds(100, 390, 130, 24);
        /**/
        
        butOk.setText("���");

        getContentPane().add(butOk);
        butOk.setBounds(80, 420, 70, 27);

        butCancel.setText("���");
        getContentPane().add(butCancel);
        butCancel.setBounds(160, 420, 70, 27);

       
        //Ϊ�ռ���þ���λ�ý���


         database.joinDB();
        
//Ϊ��Ӻ�ȡ����ť���¼�-----------------------------------------
        butOk.addActionListener(new ActionListener(){
        	public void actionPerformed(ActionEvent e){
				if(jtf1.getText().equals("")){
					new JOptionPane().showMessageDialog(null,"ѧ�Ų���Ϊ��");
					}
				else if(jtf2.getText().equals("")){
					new JOptionPane().showMessageDialog(null,"��������Ϊ��");
					}
				else if(jtf3.getText().equals("")){
					new JOptionPane().showMessageDialog(null,"�Ա���Ϊ��");
					}
				else {
					String sql="insert into stud values('"+ 
            jtf1.getText() +"','"+ jtf2.getText() +"','"+ 
            jtf3.getText() +"','"+ jtf4.getText()+"','"+ 
            jtf5.getText()+"','"+ jtf6.getText()+"','"+ 
            jtf7.getText()+"','"+ jtf8.getText()+"','"+ 
            jtf9.getText()+"','"+ jtf10.getText()+"','"+ 
            jtf11.getText()+"','"+ jtf12.getText()+"','"+ 
            jtf13.getText()+"')";
					try{
						if(database.executeSQL(sql)){
					    	new JOptionPane().showMessageDialog(null,"��ӳɹ���");			        
         		         }
						}
					catch(Exception ea){}
					}
        		}
        	});

        butCancel.addActionListener(new ActionListener(){
        	public void actionPerformed(ActionEvent e){
                jtf1.setText("");
                jtf2.setText("");
                jtf3.setText("");
                jtf4.setText("");
                jtf5.setText("");
                jtf6.setText("");
                jtf7.setText("");
                /**/
                jtf8.setText("");
                jtf9.setText("");
                jtf10.setText("");
                jtf11.setText("");
                jtf12.setText("");
                jtf13.setText("");
            }
        });
    }
}
