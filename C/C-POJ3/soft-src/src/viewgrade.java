import java.awt.event.*;
import java.awt.*;
import javax.swing.*;
import java.sql.*;

class viewgrade extends JInternalFrame {
    viewgrade() {
        super("�鿴ѧ���ɼ���Ϣ");
        Container con=getContentPane();
        con.setLayout(new BorderLayout());
        JTextArea te=new JTextArea();
        JScrollPane croll=new JScrollPane(te);
        con.add(croll,BorderLayout.CENTER);
        database.joinDB();
        String sql="SELECT * FROM grade";

        try{ 
            if(database.query(sql)){   	
                while(database.rs.next()) {    
                    te.append("ѧ��:  "+database.rs.getString(1)+"  ");
                    te.append("�κ�:  "+database.rs.getString(2)+"  ");
                    te.append("����:  "+database.rs.getString(3)+"  ");
                    te.append("�ɼ�:   "+database.rs.getString(4)+"  ");
                    te.append("�Ͽ�ʱ��:"+database.rs.getString(5)+"  ");
                    te.append("��ʦ��:   "+database.rs.getString(6)+"  ");
                    te.append("���Һ�:   "+database.rs.getString(7)+"  ");
                    te.append("\n");
                }

            }
        }
        catch(SQLException ex) 
        {  System.out.println(ex);}	

        this.setClosable(true);
        setVisible(true);
        this.setBounds(20,70,610,350);
    }
}
