import java.awt.event.*;
import java.awt.*;
import javax.swing.*;
import java.sql.*;

class viewgrade extends JInternalFrame {
    viewgrade() {
        super("查看学生成绩信息");
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
                    te.append("学号:  "+database.rs.getString(1)+"  ");
                    te.append("课号:  "+database.rs.getString(2)+"  ");
                    te.append("课名:  "+database.rs.getString(3)+"  ");
                    te.append("成绩:   "+database.rs.getString(4)+"  ");
                    te.append("上课时间:"+database.rs.getString(5)+"  ");
                    te.append("教师号:   "+database.rs.getString(6)+"  ");
                    te.append("教室号:   "+database.rs.getString(7)+"  ");
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
