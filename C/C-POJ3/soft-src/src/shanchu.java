import java.awt.event.*;
import java.awt.*;
import javax.swing.*;
import java.sql.*;

public class shanchu extends JInternalFrame {

    private JButton butCancel,butOk;
    private JLabel jLabel1;
    private JTextField jtf1;
    private JPanel p;
    public shanchu() {//�չ���
        super("ɾ��ѧ����Ϣ");
        initComponents();

        this.setVisible(true);
        this.setClosable(true); 
        this.setLocation(100,200) ;   
    }

    private void initComponents() {

        jLabel1 = new JLabel();
        Container con=getContentPane();
        con.setLayout(new FlowLayout());
        jLabel1.setText("������ѧ��");
        jtf1=new JTextField("",10);
        butOk=new JButton("ɾ��");
        butCancel=new JButton("���");
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
                    new JOptionPane().showMessageDialog(null,"������ѧ��");
                }
                else {
                    String sql="select * from stud where ѧ��=" + "'" +jtf1.getText() +"'";
                    try{
                        if(database.query(sql)){//�˴����˸�database.executeSQL(sql)��ʾresultset �ر�
                            if(!database.rs.first()) {
                                JOptionPane.showMessageDialog(null,"û�и�ѧ����Ϣ...");
                            }
                            else {
                                String sqq="delete from stud where ѧ��='" +jtf1.getText() +"'";
                                String qll="delete from grade where ѧ��='"+jtf1.getText() +"'";
                                String slq="select * from grade where ѧ��='"+jtf1.getText()+"'"; 
                                if(database.query(slq)){
                                    if(database.rs.first()){
                                        if(database.executeSQL(qll)){ }
                                        if(database.executeSQL(sqq)){JOptionPane.showMessageDialog(null,"ɾ���ɹ�");}
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


