import java.awt.event.*;
import java.awt.*;
import javax.swing.*;
import java.sql.*;

class SearchResult extends JInternalFrame {
    SearchResult(String sql) {
        super("�鿴��ѯ����ѧ����Ϣ");

        Container con=getContentPane();
        con.setLayout(new BorderLayout());
        JTextArea te=new JTextArea();
        JScrollPane croll=new JScrollPane(te);
        con.add(croll,BorderLayout.CENTER);

        database.joinDB();

        try{ 
            if(database.query(sql)){   	
                while(database.rs.next()) {    
                    te.append("ѧ��:  "+database.rs.getString(1)+"  ");
                    te.append("����:  "+database.rs.getString(2)+"  ");
                    te.append("�Ա�:  "+database.rs.getString(3)+"  ");
                    te.append("����:   "+database.rs.getString(4)+"  ");
                    te.append("������ò:"+database.rs.getString(5)+"  ");
                    te.append("ϵ��:   "+database.rs.getString(6)+"  ");
                    te.append("�༶:   "+database.rs.getString(7)+"  ");
                    te.append("��������:   "+database.rs.getString(8)+"  ");
                    te.append("����:   "+database.rs.getString(9)+"  ");
                    te.append("��ͥסַ:   "+database.rs.getString(10)+"  ");
                    te.append("��Уסַ:   "+database.rs.getString(11)+"  ");
                    te.append("���˵绰:   "+database.rs.getString(12)+"  ");
                    te.append("��ͥ�绰:   "+database.rs.getString(13)+"  ");
                    te.append("\n");
                }
            }
        }
        catch(SQLException ex) {  
            System.out.println(ex);
        }	

        this.setClosable(true);
        setVisible(true);

        setBounds(20,70,780,350);
    }
}
