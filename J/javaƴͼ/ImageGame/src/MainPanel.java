import java.awt.*; 
import java.io.*;
import java.util.*;
import java.awt.event.*; 
import javax.swing.*;
import java.applet.Applet;

public class MainPanel 
	extends JFrame 
		implements ActionListener,MouseListener,MouseMotionListener
{
	private static final long serialVersionUID = 1L;

	private PicGame wm;
	private int level;
	private Container picholder;
	private JPanel mainPanel;
	private JMenuItem theItem[];
	private JMenu allMenu[];
	private JMenuBar bar;
	private ImageIcon icon;
	private Painter pic;
	private String name;

	public MainPanel()
	{
		super("ƴͼ");

		picholder = getContentPane();
		picholder.setLayout( new BorderLayout( 5, 5 ) );
		mainPanel = new JPanel( new GridLayout( 1, 2, 5, 5 ) );

		theItem = new JMenuItem[ 9 ];
		allMenu = new JMenu[ 3 ];
		bar = new JMenuBar();

		level = 3;

		initPanel("����.jpg");
		initMenu();

		picholder.add( bar, BorderLayout.NORTH );
		picholder.add( mainPanel, BorderLayout.CENTER );

		addMouseListener( this );
		addMouseMotionListener( this );

		setSize(wm.getSize(1)*2+5+9,wm.getSize(0)+63); 
		setVisible(true);

		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}

	//��ʼ�����
	public void initPanel( String name )
	{
		icon = new ImageIcon(name);
		wm = new PicGame(name,level);
		wm.setSize(wm.getPreferredSize());

		pic = new Painter();
		pic.setSize(wm.getPreferredSize());
		pic.setVisible(true);

		mainPanel.setSize(wm.getSize(1)*2+5+9,wm.getSize(0));
		mainPanel.add(wm);
		mainPanel.add(pic);
	}

	//��ʼ���˵�
	public void initMenu()
	{
		String menu[] = { "ͼƬ�趨", "�Ѷ�ѡ��", "��ʼ��Ϸ"};
		char men[] = { 'F', 'p', 'S' };
		String item[] = { "ѡ��ͼƬ","��","һ��","����","��ʼ��Ϸ", "�˳���Ϸ"};

		for( int i=0; i<item.length; i++ )
		{
			theItem[ i ] = new JMenuItem( item[ i ] );
			theItem[ i ].addActionListener( this );
		}

		for( int i=0; i<menu.length; i++ )
		{
			allMenu[ i ] = new JMenu( menu[ i ] );
			allMenu[ i ].setMnemonic( men[ i ] );
			bar.add( allMenu[ i ] );
		}

		for( int i=0; i<item.length; i++ )
		{ 
			if( i<1 )allMenu[ 0 ].add( theItem[ i ] );
			if( i<4 && i>=1 )allMenu[ 1 ].add( theItem[ i ] );
			if( i<7 && i>=4 )allMenu[ 2 ].add( theItem[ i ] );
		}
	} 

	public void open()
	{
		JFileChooser file = new JFileChooser();
		file.setFileSelectionMode( JFileChooser.FILES_ONLY );

		int result = file.showOpenDialog(this);
		if( result == JFileChooser.CANCEL_OPTION )
			return;
		File get = file.getSelectedFile();

		if( get == null || get.getName().equals("") )
			JOptionPane.showMessageDialog( this, "Invalid File Name" ,"Error",JOptionPane.ERROR_MESSAGE );
		else 
		{//����ͼ
			name = get.getAbsolutePath();
			mainPanel.remove(wm);
			mainPanel.remove(pic);
			initPanel(name);
			setSize(1000,1000);
			setSize(wm.getSize(1)*2+5+9,wm.getSize(0)+63);  
		}
	}

	//�¼�
	public void actionPerformed( ActionEvent event )
	{
		if     ( event.getActionCommand() == "ѡ��ͼƬ" )open();
		else if( event.getActionCommand() == "��ʼ��Ϸ" )wm.pieceRandPlace();
		else if( event.getActionCommand() == "�˳���Ϸ" )System.exit( 0 );
		else if( event.getActionCommand() == "��" )
		{
			level=3;
			wm.setLevel(3);
		}
		else if( event.getActionCommand() == "һ��" )
		{
			level=4;
			wm.setLevel(4);
		}
		else if( event.getActionCommand() == "����" ){
			level=5;
			wm.setLevel(5);
		}
	}

	//����¼�
	public void mouseClicked( MouseEvent event )
	{ 
		int x = event.getX();
		int y = event.getY();
	}

	public void mousePressed( MouseEvent event )
	{
		int x = event.getX();
		int y = event.getY();

		wm.selectChess(x, y);
	}

	public void mouseReleased( MouseEvent event )
	{
		int x = event.getX();
		int y = event.getY();

		wm.moveChess(x, y);
	}

	public void mouseEntered( MouseEvent event )
	{}
	public void mouseExited( MouseEvent event )
	{}

	public void mouseDragged( MouseEvent event )
	{
		wm.setMoveXY(event.getX(), event.getY());
	}

	public void mouseMoved( MouseEvent event )
	{}

	public class Painter extends JPanel
	{
		private static final long serialVersionUID = 1L;
		public void paintComponent( Graphics g )
		{
			super.paintComponent(g);
			g.drawImage( icon.getImage(), 0,0,wm.getSize(1),wm.getSize(0), this );
		}
	}

	public static void main( String args[] )
	{
		MainPanel game = new MainPanel();
	}
}
