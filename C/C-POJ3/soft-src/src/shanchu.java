import java.awt.event.*;
import java.awt.*;
import javax.swing.*;
import java.sql.*;

public class shanchu extends JInternalFrame {

    private JButton butCancel,butOk;
    private JLabel jLabel1;
    private JTextField jtf1;
    private JPanel p;
    public shanchu() {//空构造
        super("删除学生信息");
        initComponents();

        this.setVisible(true);
        this.setClosable(true); 
        this.setLocation(100,200) ;   
    }

    private void initComponents() {

        jLabel1 = new JLabel();
        Container con=getContentPane();
        con.setLayout(new FlowLayout());
        jLabel1.setText("请输入学号");
        jtf1=new JTextField("",10);
        butOk=new JButton("删除");
        butCancel=new JButton("清空");
        p=new JPanel();
        p.add(jLabel1);
        p.add(jtf1);
        p.add(butOk);
        p.add(butCancel);
        con.add(p);
        setVisible(true);
        this.pack();
        database.joinDB();
        butOk.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e){
                if(jtf1.getText().equals("")){
                    new JOptionPane().showMessageDialog(null,"请输入学号");
                }
                else {
                    String sql="select * from stud where 学号=" + "'" +jtf1.getText() +"'";
                    try{
                        if(database.query(sql)){//此处用了个database.executeSQL(sql)提示resultset 关闭
                            if(!database.rs.first()) {
                                JOptionPane.showMessageDialog(null,"没有该学生信息...");
                            }
                            else {
                                String sqq="delete from stud where 学号='" +jtf1.getText() +"'";
                                String qll="delete from grade where 学号='"+jtf1.getText() +"'";
                                String slq="select * from grade where 学号='"+jtf1.getText()+"'"; 
                                if(database.query(slq)){
                                    if(database.rs.first()){
                                        if(database.executeSQL(qll)){ }
                                        if(database.executeSQL(sqq)){JOptionPane.showMessageDialog(null,"删除成功");}
                                    }
                                } 
                            }
                        }
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


        butCancel.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e){
                jtf1.setText("");
            }
        });
    }
}


