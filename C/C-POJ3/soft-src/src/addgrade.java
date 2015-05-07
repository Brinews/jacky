import java.awt.event.*;
import java.awt.*;
import javax.swing.*;
import java.sql.*;

public class addgrade extends JInternalFrame {
	
	private JButton butCancel,butOk;
    private JLabel jLabel1,jLabel2,jLabel3,jLabel4,jLabel5,jLabel6,jLabel7;
    private JTextField jtf2,jtf3,jtf4,jtf5,jtf6,jtf7;
    private JComboBox jnum;
    public addgrade() {//空构造
    super("添加学生成绩");
        initComponents();
        
        this.setVisible(true);
        this.setClosable(true);
        this.setSize(260,310);        
   }
    private void initComponents() {
    	
    	//实例化并添加控件
        jLabel1 = new JLabel();
        jLabel2 = new JLabel();
        jLabel2 = new JLabel();
        jLabel3 = new JLabel();
        jLabel4 = new JLabel();
        jLabel5 = new JLabel();
        jLabel6 = new JLabel();
        jLabel7 = new JLabel();
   
        jnum = new JComboBox();
        jtf2=new JTextField();
        jtf3=new JTextField();
        jtf4=new JTextField();
        jtf5=new JTextField();
        jtf6=new JTextField();
        jtf7=new JTextField();     
        butOk = new JButton();
        butCancel = new JButton();       
        //实例化并添加控件完成

        getContentPane().setLayout(null);//设置布局
        
        
        //为空间放置具体位置
        jLabel1.setText("学 号");
        getContentPane().add(jLabel1);
        jLabel1.setBounds(30, 30, 70, 20);

        jLabel2.setText("课 号");
        getContentPane().add(jLabel2);
        jLabel2.setBounds(30, 60, 70, 20);

        jLabel3.setText("课 名");
        getContentPane().add(jLabel3);
        jLabel3.setBounds(30, 90, 60, 20);
        
        jLabel4.setText("成 绩");
        getContentPane().add(jLabel4);
        jLabel4.setBounds(30, 120, 70, 20);

        jLabel5.setText("上课时间");
        getContentPane().add(jLabel5);
        jLabel5.setBounds(30, 150, 70, 20);
        
        jLabel6.setText("教师号");
        getContentPane().add(jLabel6);
        jLabel6.setBounds(30, 180, 70, 20);
        
        jLabel7.setText("教室号");
        getContentPane().add(jLabel7);
        jLabel7.setBounds(30, 210, 70, 20);
                
        getContentPane().add(jnum);
        jnum.setBounds(100, 30, 130, 24);

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
        
        butOk.setText("添加");

        getContentPane().add(butOk);
        butOk.setBounds(80, 240, 70, 27);

        butCancel.setText("取消");
        getContentPane().add(butCancel);
        butCancel.setBounds(160, 240, 70, 27);
       
       //将所有学号读出来
         database.joinDB();
         String sql="select 学号 from stud";  
         try{
         	if(database.query(sql)){
         		while(database.rs.next()){
         			String name=database.rs.getString("学号");
         			jnum.addItem(name);
         			}
         		}
         	}
       	catch(Exception ea){}
					
      
//为添加和取消按钮加事件-----------------------------------------
     
        butOk.addActionListener(new ActionListener(){
        public void actionPerformed(ActionEvent e){	
        String name="" + jnum.getSelectedItem();
			String sql="insert into grade values('"+ 
            name+"','"+ jtf2.getText() +"','"+ jtf3.getText() +"','"+ 
            jtf4.getText()+"','"+ jtf5.getText()+"','"+ 
            jtf6.getText()+"','"+ jtf7.getText()+"')";
					try{
						if(database.executeSQL(sql)){
					    	new JOptionPane().showMessageDialog(null,"添加成功！");
         			        
         		         }
						}
					catch(Exception ea){}
					
        	}
        	});	
        	

        butCancel.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e){	
                jtf2.setText("");
                jtf3.setText("");
                jtf4.setText("");
                jtf5.setText("");
                jtf6.setText("");
                jtf7.setText("");

            }
        });
    }
}
