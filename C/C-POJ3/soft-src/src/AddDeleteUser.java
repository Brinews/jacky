import java.awt.event.*;
import java.awt.*;
import javax.swing.*;
import java.sql.*;

public class AddDeleteUser extends JInternalFrame {
	
	private JButton butACancel,butDCancel,butDelete,butOk;
    private JComboBox cbUserName;
    private JLabel jLabel1,jLabel2,jLabel3,jLabel4,jLabel5;
    private JPasswordField pas1,pas2,pas3;
    private JTextField txtname;

    public AddDeleteUser() {//�չ���
      super("�û���Ϣ��Ӻ�ɾ��");
        initComponents();
        this.setVisible(true);
        this.setClosable(true);
        this.setSize(268,350);
        
   }

    private void initComponents() {
    	
    	//ʵ��������ӿؼ�
        jLabel1 = new JLabel();
        jLabel2 = new JLabel();
        jLabel3 = new JLabel();
        txtname = new JTextField();
        pas1 = new JPasswordField();
        pas2 = new JPasswordField();
        butOk = new JButton();
        butACancel = new JButton();
        jLabel4 = new JLabel();
        cbUserName = new JComboBox();
        jLabel5 = new JLabel();
        pas3 = new JPasswordField();
        butDelete = new JButton();
        butDCancel = new JButton();
        //ʵ��������ӿؼ����

        getContentPane().setLayout(null);//���ò���
        
        
        //Ϊ�ռ���þ���λ��
        jLabel1.setText("���û���:");
        getContentPane().add(jLabel1);
        jLabel1.setBounds(30, 30, 70, 20);

        jLabel2.setText("��������:");
        getContentPane().add(jLabel2);
        jLabel2.setBounds(30, 60, 70, 18);

        jLabel3.setText("ȷ������:");
        getContentPane().add(jLabel3);
        jLabel3.setBounds(30, 90, 60, 18);

        getContentPane().add(txtname);
        txtname.setBounds(100, 30, 130, 24);

        getContentPane().add(pas1);
        pas1.setBounds(100, 60, 130, 24);

        getContentPane().add(pas2);
        pas2.setBounds(100, 90, 130, 24);

        butOk.setText("���");

        getContentPane().add(butOk);
        butOk.setBounds(80, 130, 70, 27);

        butACancel.setText("���");
        getContentPane().add(butACancel);
        butACancel.setBounds(160, 130, 70, 27);

        jLabel4.setText("�����û���:");
        getContentPane().add(jLabel4);
        jLabel4.setBounds(30, 180, 80, 18);

        getContentPane().add(cbUserName);
        cbUserName.setBounds(100, 180, 130, 24);

        jLabel5.setText("����:");
        getContentPane().add(jLabel5);
        jLabel5.setBounds(30, 210, 60, 18);

        getContentPane().add(pas3);
        pas3.setBounds(100, 216, 130, 24);

        butDelete.setText("ɾ��");
        getContentPane().add(butDelete);
        butDelete.setBounds(79, 260, 70, 27);

        butDCancel.setText("���");
        getContentPane().add(butDCancel);
        butDCancel.setBounds(160, 260, 70, 27);
        
        //Ϊ�ռ���þ���λ�ý���

//���������û���������
         database.joinDB();
         String sql="select * from user";
         try{
         	if(database.query(sql)){
         		while(database.rs.next()){
         			String name=database.rs.getString("user_id");
         			cbUserName.addItem(name);
         			}
         		}
         	}
         catch(Exception e){}

//Ϊ��Ӻ�ȡ����ť���¼�-----------------------------------------
        butOk.addActionListener(new ActionListener(){
        	public void actionPerformed(ActionEvent e){
				if(txtname.getText().equals("")){
					new JOptionPane().showMessageDialog(null,"�û�������Ϊ��!");
					}
				else if(pas1.getText().equals("")){
					new JOptionPane().showMessageDialog(null,"���벻��Ϊ��!");
					}
				else if(pas1.getText().equals(pas2.getText())){
					String sql="insert into user values('"+ txtname.getText() +"','"+ pas1.getText() +"')";
					try{
						if(database.executeSQL(sql)){
					    	new JOptionPane().showMessageDialog(null,"��ӳɹ���");
         			        cbUserName.addItem(txtname.getText());
         		         }
						}
					catch(Exception ea){}
					}
        		}
        	});

        butACancel.addActionListener(new ActionListener(){
        	public void actionPerformed(ActionEvent e){
        		txtname.setText("");
        		pas1.setText("");
        		pas2.setText("");
        		}
        	});
//--------------------------------------------------------------

//Ϊɾ����ȡ����ť���¼�---------------------------------------
        butDelete.addActionListener(new ActionListener(){
        	public void actionPerformed(ActionEvent e){
         		String name="" + cbUserName.getSelectedItem();
         		String sql="select * from user where user_id='"+ name +"'";
         		try{
         		   if(database.query(sql)){
         		   	    database.rs.next();
	         			String pas=pas3.getText();
	         			String password=database.rs.getString("Password");
	         			System.out.println(password);
	         			if(pas.equals(password)){
	         					String sdelete="delete from user where user_id='"+ name +"'";
	         					if(database.executeSQL(sdelete)){
	         						new JOptionPane().showMessageDialog(null,"ɾ���ɹ���");
                                    pas3.setText("");
                                    cbUserName.removeAllItems();
                                    String sql1="select * from user";
						         	if(database.query(sql1)){
						         		while(database.rs.next()){
						         			String name1=database.rs.getString("user_id");
						         			cbUserName.addItem(name1);
						         			}
						         		}
	         						}
	         				}
	         			else{
	         				new JOptionPane().showMessageDialog(null,"���벻��ȷ��");
	         				}
	         			}
	         	    }
	         	 catch(Exception el){
	         	 	System.out.println(el);
	         	 	}
        		}
        	});

        butDCancel.addActionListener(new ActionListener(){
        	public void actionPerformed(ActionEvent e){
        		pas3.setText("");
        		}
        	});


//--------------------------------------------------------------
    }
}