import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.sql.*;

public class Main extends JFrame {

    JDesktopPane deskpane = new JDesktopPane();//�ڴ����ｨ���������沢ʵ����
    JPanel p = new JPanel();//����һ����岢ʵ����
    public Main(){//���캯��
        setTitle("ѧ����Ϣ����ϵͳ");//���ô������
        Container con = getContentPane();
        con.setLayout(new BorderLayout());//����һ������
        con.add(deskpane,BorderLayout.CENTER);//ʵ����������Ĳ���
        Font f =new Font("������",Font.PLAIN,12);	
        JMenuBar mb = new JMenuBar();//ʵ�����˵���
        mb.setOpaque(true);
        setJMenuBar(mb);

        JMenu systemM = new JMenu("ϵͳ����");
        systemM.setFont(f);
        mb.add(systemM);		

        JMenuItem land = new JMenuItem("���µ�½");
        land.setFont(f);

        JMenuItem exit = new JMenuItem("�˳�ϵͳ");
        exit.setFont(f);	
        systemM.add(land);
        systemM.add(exit);
        JMenu userM = new JMenu("�û�����");
        userM.setFont(f);
        mb.add(userM);
        JMenuItem password = new JMenuItem("�����޸�");
        password.setFont(f);
        JMenuItem addDelete = new JMenuItem("���/ɾ���û�");
        addDelete.setFont(f);
        userM.add(password);
        userM.add(addDelete);

        JMenu studentM = new JMenu("ѧ����Ϣ����");
        studentM.setFont(f);
        mb.add(studentM);
        JMenu addM= new JMenu("��Ӽ�¼");
        addM.setFont(f);
        JMenu viewM = new JMenu("��Ϣ�鿴");
        viewM.setFont(f);
        JMenu modifM = new JMenu("��Ϣ�޸�");
        modifM.setFont(f);		
        JMenu searchM = new JMenu("��Ϣ��ѯ");
        searchM.setFont(f);		
        JMenuItem addinf = new JMenuItem("�����Ϣ��¼");
        addinf.setFont(f);
        addM.add(addinf);
        JMenuItem addgrade = new JMenuItem("��ӳɼ���¼");
        addgrade.setFont(f);
        addM.add(addgrade);

        JMenuItem viewstud = new JMenuItem("������Ϣ�鿴");
        viewstud.setFont(f);
        viewM.add(viewstud);

        JMenuItem viewgrade = new JMenuItem("�ɼ���Ϣ�鿴");
        addDelete.setFont(f);
        viewM.add(viewgrade);

        JMenuItem xiugai = new JMenuItem("������Ϣ�޸�");
        addDelete.setFont(f);
        modifM.add(xiugai);

        JMenuItem xiugaigrade = new JMenuItem("�ɼ���Ϣ�޸�");
        addDelete.setFont(f);
        modifM.add(xiugaigrade);

        JMenuItem basicSearch = new JMenuItem("������Ϣ����");
        basicSearch.setFont(f);
        searchM.add(basicSearch);

        JMenuItem shanchu= new JMenuItem("ɾ��ѧ����Ϣ");
        addDelete.setFont(f);

        studentM.add(addM);
        studentM.add(viewM);
        studentM.add(modifM);
        studentM.add(searchM);
        studentM.add(shanchu);


        password.addActionListener(new ActionListener(){//�����޸ļ���
            public void actionPerformed(ActionEvent e){
                System.out.println("xgmima");
                deskpane.add(new xgmima());
            }
        });

        land.addActionListener(new ActionListener(){//���µ�½����
            public void actionPerformed(ActionEvent e){
                System.out.println("Land");
                setVisible(false);
                new Land();
            }
        });
        addDelete.addActionListener(new ActionListener(){//���/ɾ���û�����
            public void actionPerformed(ActionEvent e){
                deskpane.add(new AddDeleteUser());
            }
        });
        addinf.addActionListener(new ActionListener(){//���ѧ����Ϣ
            public void actionPerformed(ActionEvent e){
                deskpane.add(new addstud());
            }
        });
        addgrade.addActionListener(new ActionListener(){//��ӳɼ���Ϣ
            public void actionPerformed(ActionEvent e){
                deskpane.add(new addgrade());
            }
        });

        viewstud.addActionListener(new ActionListener(){//ѧ��������Ϣ�鿴
            public void actionPerformed(ActionEvent e){
                System.out.println("�鿴ѧ��������Ϣ");
                deskpane.add(new viewstud());
            }
        });

        viewgrade.addActionListener(new ActionListener(){//ѧ���ɼ���Ϣ�鿴
            public void actionPerformed(ActionEvent e){
                System.out.println("�鿴ѧ���ɼ���Ϣ");
                deskpane.add(new viewgrade());
            }
        });
        xiugai.addActionListener(new ActionListener(){//ѧ��������Ϣ�޸�
            public void actionPerformed(ActionEvent e){
                System.out.println("�޸�ѧ��������Ϣ");
                deskpane.add(new xiugai());
            }
        });
        xiugaigrade.addActionListener(new ActionListener(){//ѧ���ɼ���Ϣ�޸�
            public void actionPerformed(ActionEvent e){
                System.out.println("�޸�ѧ���ɼ���Ϣ");
                deskpane.add(new xiugaigrade());
            }
        });

        shanchu.addActionListener(new ActionListener(){//ɾ��ѧ����Ϣ
            public void actionPerformed(ActionEvent e){
                System.out.println("�޸�ѧ���ɼ���Ϣ");
                deskpane.add(new shanchu());
            }
        });

        basicSearch.addActionListener(new ActionListener(){//ѧ��������Ϣ��ѯ
            public void actionPerformed(ActionEvent e){
                System.out.println("��ѯѧ��������Ϣ");
                deskpane.add(new BasicSearch(deskpane));
            }
        });

        exit.addActionListener(new ActionListener(){//�˳�ϵͳ����
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

