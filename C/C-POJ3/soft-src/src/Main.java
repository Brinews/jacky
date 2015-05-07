import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.sql.*;

public class Main extends JFrame {

    JDesktopPane deskpane = new JDesktopPane();//在窗体里建立虚拟桌面并实例化
    JPanel p = new JPanel();//创建一个面板并实例化
    public Main(){//构造函数
        setTitle("学生信息管理系统");//设置窗体标题
        Container con = getContentPane();
        con.setLayout(new BorderLayout());//创建一个布局
        con.add(deskpane,BorderLayout.CENTER);//实例虚拟桌面的布局
        Font f =new Font("新宋体",Font.PLAIN,12);	
        JMenuBar mb = new JMenuBar();//实例化菜单栏
        mb.setOpaque(true);
        setJMenuBar(mb);

        JMenu systemM = new JMenu("系统管理");
        systemM.setFont(f);
        mb.add(systemM);		

        JMenuItem land = new JMenuItem("重新登陆");
        land.setFont(f);

        JMenuItem exit = new JMenuItem("退出系统");
        exit.setFont(f);	
        systemM.add(land);
        systemM.add(exit);
        JMenu userM = new JMenu("用户管理");
        userM.setFont(f);
        mb.add(userM);
        JMenuItem password = new JMenuItem("密码修改");
        password.setFont(f);
        JMenuItem addDelete = new JMenuItem("添加/删除用户");
        addDelete.setFont(f);
        userM.add(password);
        userM.add(addDelete);

        JMenu studentM = new JMenu("学生信息管理");
        studentM.setFont(f);
        mb.add(studentM);
        JMenu addM= new JMenu("添加记录");
        addM.setFont(f);
        JMenu viewM = new JMenu("信息查看");
        viewM.setFont(f);
        JMenu modifM = new JMenu("信息修改");
        modifM.setFont(f);		
        JMenu searchM = new JMenu("信息查询");
        searchM.setFont(f);		
        JMenuItem addinf = new JMenuItem("添加信息记录");
        addinf.setFont(f);
        addM.add(addinf);
        JMenuItem addgrade = new JMenuItem("添加成绩记录");
        addgrade.setFont(f);
        addM.add(addgrade);

        JMenuItem viewstud = new JMenuItem("基本信息查看");
        viewstud.setFont(f);
        viewM.add(viewstud);

        JMenuItem viewgrade = new JMenuItem("成绩信息查看");
        addDelete.setFont(f);
        viewM.add(viewgrade);

        JMenuItem xiugai = new JMenuItem("基本信息修改");
        addDelete.setFont(f);
        modifM.add(xiugai);

        JMenuItem xiugaigrade = new JMenuItem("成绩信息修改");
        addDelete.setFont(f);
        modifM.add(xiugaigrade);

        JMenuItem basicSearch = new JMenuItem("基本信息检索");
        basicSearch.setFont(f);
        searchM.add(basicSearch);

        JMenuItem shanchu= new JMenuItem("删除学生信息");
        addDelete.setFont(f);

        studentM.add(addM);
        studentM.add(viewM);
        studentM.add(modifM);
        studentM.add(searchM);
        studentM.add(shanchu);


        password.addActionListener(new ActionListener(){//密码修改监听
            public void actionPerformed(ActionEvent e){
                System.out.println("xgmima");
                deskpane.add(new xgmima());
            }
        });

        land.addActionListener(new ActionListener(){//重新登陆监听
            public void actionPerformed(ActionEvent e){
                System.out.println("Land");
                setVisible(false);
                new Land();
            }
        });
        addDelete.addActionListener(new ActionListener(){//添加/删除用户监听
            public void actionPerformed(ActionEvent e){
                deskpane.add(new AddDeleteUser());
            }
        });
        addinf.addActionListener(new ActionListener(){//添加学生信息
            public void actionPerformed(ActionEvent e){
                deskpane.add(new addstud());
            }
        });
        addgrade.addActionListener(new ActionListener(){//添加成绩信息
            public void actionPerformed(ActionEvent e){
                deskpane.add(new addgrade());
            }
        });

        viewstud.addActionListener(new ActionListener(){//学生基本信息查看
            public void actionPerformed(ActionEvent e){
                System.out.println("查看学生基本信息");
                deskpane.add(new viewstud());
            }
        });

        viewgrade.addActionListener(new ActionListener(){//学生成绩信息查看
            public void actionPerformed(ActionEvent e){
                System.out.println("查看学生成绩信息");
                deskpane.add(new viewgrade());
            }
        });
        xiugai.addActionListener(new ActionListener(){//学生基本信息修改
            public void actionPerformed(ActionEvent e){
                System.out.println("修改学生基本信息");
                deskpane.add(new xiugai());
            }
        });
        xiugaigrade.addActionListener(new ActionListener(){//学生成绩信息修改
            public void actionPerformed(ActionEvent e){
                System.out.println("修改学生成绩信息");
                deskpane.add(new xiugaigrade());
            }
        });

        shanchu.addActionListener(new ActionListener(){//删除学生信息
            public void actionPerformed(ActionEvent e){
                System.out.println("修改学生成绩信息");
                deskpane.add(new shanchu());
            }
        });

        basicSearch.addActionListener(new ActionListener(){//学生基本信息查询
            public void actionPerformed(ActionEvent e){
                System.out.println("查询学生基本信息");
                deskpane.add(new BasicSearch(deskpane));
            }
        });

        exit.addActionListener(new ActionListener(){//退出系统监听
            public void actionPerformed(ActionEvent e){
                setVisible(false);
            }
        });

        this.setBounds(200,200,800,600);
        setVisible(true);	 
    }

    public static void main(String[] args){
        new Land();
    }
}

