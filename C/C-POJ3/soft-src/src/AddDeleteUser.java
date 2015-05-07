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

    public AddDeleteUser() {//空构造
      super("用户信息添加和删除");
        initComponents();
        this.setVisible(true);
        this.setClosable(true);
        this.setSize(268,350);
        
   }

    private void initComponents() {
    	
    	//实例化并添加控件
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
        //实例化并添加控件完成

        getContentPane().setLayout(null);//设置布局
        
        
        //为空间放置具体位置
        jLabel1.setText("新用户名:");
        getContentPane().add(jLabel1);
        jLabel1.setBounds(30, 30, 70, 20);

        jLabel2.setText("输入密码:");
        getContentPane().add(jLabel2);
        jLabel2.setBounds(30, 60, 70, 18);

        jLabel3.setText("确认密码:");
        getContentPane().add(jLabel3);
        jLabel3.setBounds(30, 90, 60, 18);

        getContentPane().add(txtname);
        txtname.setBounds(100, 30, 130, 24);

        getContentPane().add(pas1);
        pas1.setBounds(100, 60, 130, 24);

        getContentPane().add(pas2);
        pas2.setBounds(100, 90, 130, 24);

        butOk.setText("添加");

        getContentPane().add(butOk);
        butOk.setBounds(80, 130, 70, 27);

        butACancel.setText("清空");
        getContentPane().add(butACancel);
        butACancel.setBounds(160, 130, 70, 27);

        jLabel4.setText("已有用户名:");
        getContentPane().add(jLabel4);
        jLabel4.setBounds(30, 180, 80, 18);

        getContentPane().add(cbUserName);
        cbUserName.setBounds(100, 180, 130, 24);

        jLabel5.setText("密码:");
        getContentPane().add(jLabel5);
        jLabel5.setBounds(30, 210, 60, 18);

        getContentPane().add(pas3);
        pas3.setBounds(100, 216, 130, 24);

        butDelete.setText("删除");
        getContentPane().add(butDelete);
        butDelete.setBounds(79, 260, 70, 27);

        butDCancel.setText("清空");
        getContentPane().add(butDCancel);
        butDCancel.setBounds(160, 260, 70, 27);
        
        //为空间放置具体位置结束

//将所有用用户名读出来
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

//为添加和取消按钮加事件-----------------------------------------
        butOk.addActionListener(new ActionListener(){
        	public void actionPerformed(ActionEvent e){
				if(txtname.getText().equals("")){
					new JOptionPane().showMessageDialog(null,"用户名不能为空!");
					}
				else if(pas1.getText().equals("")){
					new JOptionPane().showMessageDialog(null,"密码不能为空!");
					}
				else if(pas1.getText().equals(pas2.getText())){
					String sql="insert into user values('"+ txtname.getText() +"','"+ pas1.getText() +"')";
					try{
						if(database.executeSQL(sql)){
					    	new JOptionPane().showMessageDialog(null,"添加成功！");
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

//为删除和取消按钮加事件---------------------------------------
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
	         						new JOptionPane().showMessageDialog(null,"删除成功！");
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
	         				new JOptionPane().showMessageDialog(null,"密码不正确！");
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