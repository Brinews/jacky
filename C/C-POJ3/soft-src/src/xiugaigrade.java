import java.awt.event.*;
import java.awt.*;
import javax.swing.*;
import java.sql.*;

public class xiugaigrade extends JInternalFrame {
	
	private JButton butCancel,butOk,butShow;
    private JLabel jLabel1,jLabel2,jLabel3,jLabel4,jLabel5,jLabel6,jLabel7;
    private JTextField jtf1,jtf2,jtf3,jtf4,jtf5,jtf6,jtf7;

    public xiugaigrade() {//空构造
    super("修改成绩信息");
        initComponents();
        
        this.setVisible(true);
        this.setClosable(true);
        this.setSize(300,310);
        
   }

    private void initComponents() {
    	
    	//实例化并添加控件
        jLabel1 = new JLabel();
        jLabel2 = new JLabel();
        jLabel3 = new JLabel();
        jLabel4 = new JLabel();
        jLabel5 = new JLabel();
        jLabel6 = new JLabel();
        jLabel7 = new JLabel();       
        jtf1=new JTextField();
        jtf2=new JTextField();
        jtf3=new JTextField();
        jtf4=new JTextField();
        jtf5=new JTextField();
        jtf6=new JTextField();
        jtf7=new JTextField();     
        butOk = new JButton();
        butCancel = new JButton();
        butShow = new JButton();       
        //实例化并添加控件完成

        getContentPane().setLayout(null);//设置布局
        
        
        //为空间放置具体位置
         jLabel1.setText("学 号");
        getContentPane().add(jLabel1);
        jLabel1.setBounds(30, 30, 70, 20);

        jLabel2.setText("VBA开发");
        getContentPane().add(jLabel2);
        jLabel2.setBounds(30, 60, 70, 20);

        jLabel3.setText("大学英语");
        getContentPane().add(jLabel3);
        jLabel3.setBounds(30, 90, 60, 20);
        
        jLabel4.setText("java开发");
        getContentPane().add(jLabel4);
        jLabel4.setBounds(30, 120, 70, 20);

        jLabel5.setText("SQL Server");
        getContentPane().add(jLabel5);
        jLabel5.setBounds(30, 150, 70, 20);
        
        jLabel6.setText("高等数学");
        getContentPane().add(jLabel6);
        jLabel6.setBounds(30, 180, 70, 20);
        
        jLabel7.setText("网站建设");
        getContentPane().add(jLabel7);
        jLabel7.setBounds(30, 210, 70, 20);
                
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
        
        butShow.setText("显示信息");
        getContentPane().add(butShow);
        butShow.setBounds(20, 240,90, 27);

        butOk.setText("修改");
        getContentPane().add(butOk);
        butOk.setBounds(120, 240, 70, 27);
        butOk.enable(false);
        
        butCancel.setText("清空");
        getContentPane().add(butCancel);
        butCancel.setBounds(200,240,70,27);
        
        //为空间放置具体位置结束
        database.joinDB();
        butShow.addActionListener(new ActionListener(){
        	public void actionPerformed(ActionEvent e){
				if(jtf1.getText().equals("")){
					new JOptionPane().showMessageDialog(null,"请输入学号");
					}
				else {
					String sql="select * from grade where 学号=" + "'" +jtf1.getText() +"'";
					try{
						if(database.query(sql)){//此处用了个database.executeSQL(sql)提示resultset 关闭
					    if(!database.rs.first())
			{
				JOptionPane.showMessageDialog(null,"没有该学生信息...");
			}
				else {	    	database.rs.first();
					    jtf2.setText(database.rs.getString("VBA开发"));	
		                jtf3.setText(database.rs.getString("大学英语"));
		                jtf4.setText(database.rs.getString("java开发"));
		                jtf5.setText(database.rs.getString("SQLServer"));
		                jtf6.setText(database.rs.getString("高等数学"));
		                jtf7.setText(database.rs.getString("网站建设"));
		                butOk.setEnabled(true);		        
         		        } }
						}
			catch(NullPointerException upe)
		        {
		        	System.out.println(upe.toString());
		        }
		    catch(SQLException sqle)
			{
				System.out.println(sqle.toString());
			}
			catch(Exception ex)
			{
				System.out.println(ex.toString());
			}
					}
        		}
        	});
                
        butOk.addActionListener(new ActionListener(){
        	public void actionPerformed(ActionEvent e){
				
						String sql="update grade set VBA开发='"+ jtf2.getText() +"',大学英语='"+ jtf3.getText() +"',java开发='"+ jtf4.getText()+"',SQLServer='"+ jtf5.getText()+"',高等数学='"+ jtf6.getText()+"',网站建设='"+ jtf7.getText()+"'"+ " where 学号='"+jtf1.getText()+"'";
					try{
					
						if(database.executeSQL(sql)){
					    	new JOptionPane().showMessageDialog(null,"更新成功！");			        
         		         }
						}
					catch(Exception ea){}
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
        		
        		}
        	});

    }
}