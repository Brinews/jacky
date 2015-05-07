import java.awt.event.*;
import java.awt.*;
import javax.swing.*;
import java.sql.*;
import java.util.*;

public class BasicSearch extends JInternalFrame {

    private JButton butCancel,butOk;
    private JLabel jLabel1;
    private JTextField jtf1;
    private JLabel jLabel2;
    private JTextField jtf2;
    private JLabel jLabel3;
    private JTextField jtf3;
    private JPanel p;
    private JDesktopPane deskpane;

    public BasicSearch(JDesktopPane jdp) {//空构造
        super("学生信息查询");
        initComponents();

        deskpane = jdp;

        this.setVisible(true);
        this.setClosable(true); 
        this.setLocation(100,200) ;   
    }

    private void initComponents() {

        jLabel1 = new JLabel();
        jLabel2 = new JLabel();
        jLabel3 = new JLabel();
        Container con=getContentPane();
        con.setLayout(new FlowLayout());
        jLabel1.setText("学号");
        jLabel2.setText("专业");
        jLabel3.setText("班级");
        jtf1=new JTextField("",10);
        jtf2=new JTextField("",10);
        jtf3=new JTextField("",10);
        butOk=new JButton("查询");
        butCancel=new JButton("清空");
        p=new JPanel();
        p.add(jLabel1);
        p.add(jtf1);
        p.add(jLabel2);
        p.add(jtf2);
        p.add(jLabel3);
        p.add(jtf3);
        p.add(butOk);
        p.add(butCancel);
        con.add(p);
        setVisible(true);
        this.pack();

        database.joinDB();

        butOk.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e){
                if(jtf1.getText().equals("")
                    && jtf2.getText().equals("")
                    && jtf3.getText().equals("")){
                    new JOptionPane().showMessageDialog(null,
                        "请输入查询条件");
                } else {

                    String sql="select * from stud where ";
                    if (!jtf1.getText().equals("")) {
                        sql += "学号=" + "'" +jtf1.getText() +"'";
                    }
                    if (!jtf2.getText().equals("")) {
                        if (sql.length() > 25) {
                            sql += " and ";
                        }
                        sql += "系别=" + "'" +jtf2.getText() +"'";
                    }
                    if (!jtf3.getText().equals("")) {
                        if (sql.length() > 25) {
                            sql += " and ";
                        }
                        sql += "班级=" + "'" +jtf3.getText() +"'";
                    }

                    try{
                        if(database.query(sql)){
                            //此处用了个database.executeSQL(sql)提示resultset 关闭
                            if(!database.rs.first()) {
                                JOptionPane.showMessageDialog(null,
                                        "没有查询到相关信息...");
                            } else {
                                deskpane.add(new SearchResult(sql));
                            } 
                        }
                    } catch(SQLException sqle) {
                        System.out.println(sqle.toString());
                    }
                    catch(Exception ex) {
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
