import java.awt.event.*;
import java.awt.*;
import javax.swing.*;
import java.sql.*;

class SearchResult extends JInternalFrame {
    SearchResult(String sql) {
        super("查看查询到的学生信息");

        Container con=getContentPane();
        con.setLayout(new BorderLayout());
        JTextArea te=new JTextArea();
        JScrollPane croll=new JScrollPane(te);
        con.add(croll,BorderLayout.CENTER);

        database.joinDB();

        try{ 
            if(database.query(sql)){   	
                while(database.rs.next()) {    
                    te.append("学号:  "+database.rs.getString(1)+"  ");
                    te.append("姓名:  "+database.rs.getString(2)+"  ");
                    te.append("性别:  "+database.rs.getString(3)+"  ");
                    te.append("年龄:   "+database.rs.getString(4)+"  ");
                    te.append("政治面貌:"+database.rs.getString(5)+"  ");
                    te.append("系别:   "+database.rs.getString(6)+"  ");
                    te.append("班级:   "+database.rs.getString(7)+"  ");
                    te.append("出生年月:   "+database.rs.getString(8)+"  ");
                    te.append("籍贯:   "+database.rs.getString(9)+"  ");
                    te.append("家庭住址:   "+database.rs.getString(10)+"  ");
                    te.append("本校住址:   "+database.rs.getString(11)+"  ");
                    te.append("个人电话:   "+database.rs.getString(12)+"  ");
                    te.append("家庭电话:   "+database.rs.getString(13)+"  ");
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
