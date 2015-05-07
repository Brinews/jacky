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

    public BasicSearch(JDesktopPane jdp) {//�չ���
        super("ѧ����Ϣ��ѯ");
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
        jLabel1.setText("ѧ��");
        jLabel2.setText("רҵ");
        jLabel3.setText("�༶");
        jtf1=new JTextField("",10);
        jtf2=new JTextField("",10);
        jtf3=new JTextField("",10);
        butOk=new JButton("��ѯ");
        butCancel=new JButton("���");
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
                        "�������ѯ����");
                } else {

                    String sql="select * from stud where ";
                    if (!jtf1.getText().equals("")) {
                        sql += "ѧ��=" + "'" +jtf1.getText() +"'";
                    }
                    if (!jtf2.getText().equals("")) {
                        if (sql.length() > 25) {
                            sql += " and ";
                        }
                        sql += "ϵ��=" + "'" +jtf2.getText() +"'";
                    }
                    if (!jtf3.getText().equals("")) {
                        if (sql.length() > 25) {
                            sql += " and ";
                        }
                        sql += "�༶=" + "'" +jtf3.getText() +"'";
                    }

                    try{
                        if(database.query(sql)){
                            //�˴����˸�database.executeSQL(sql)��ʾresultset �ر�
                            if(!database.rs.first()) {
                                JOptionPane.showMessageDialog(null,
                                        "û�в�ѯ�������Ϣ...");
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
