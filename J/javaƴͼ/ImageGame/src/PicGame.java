import java.util.Timer;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.image.BufferedImage;

public class PicGame extends JPanel {

	private int height,width;
	private int picH,picW;
	private int rowI,colI;
	private int chessLevel;

	private int movX, movY; /* moved place */

	private ImageIcon img;
	private Image optImage;
	private	ImageIcon ico;
	private Graphics bg;

	private int check[];
	private boolean success;

	private int clicktimes = 0;
	private int src = -1, dst = -1;
	private int begin = 0;

	public PicGame()
	{
	}

	//���캯��
	public PicGame( String name, int l )
	{
		init(name,l);
	}

	//��ʼ�����в���ֵ
	public void init( String name,int l)
	{
		success = false;

		chessLevel = l;
		check = new int[ chessLevel*chessLevel ];
		for(int i=0;i<chessLevel*chessLevel;i++)
			check[i]=i;

		clicktimes = 0;
		src = -1;

		rowI = chessLevel;
		colI = chessLevel;

		img = new ImageIcon( name );
		crowIeSize();
		picH = height/rowI;
		picW = width/colI;

		optImage = new BufferedImage( width, height, 1);
		bg = optImage.getGraphics();
		bg.drawImage( img.getImage(), 0,0,width, height, this );

		ico = new ImageIcon( "over.gif" );
	}

	//��ͼƬ�������Ŵ���
	public void crowIeSize()
	{
		double w = (double)img.getImage().getWidth( null );
		double h = (double)img.getImage().getHeight( null );
		if( w >= h)
		{
			width = 500;
			height = (int)(h/w*500)+1;
		}
		else
		{
			height = 500;
			width = (int)(w/h*500)+1;
		}
	}

	//����һСƬ
	void drawSeg(int seg, int x, int y, Graphics g)
	{
		int dx = seg % chessLevel * picW;
		int dy = seg / chessLevel * picH;

		g.drawImage(optImage, //img.getImage(),
				x*picW, y*picH, x*picW+picW, y*picH+picH,
				dx, dy, dx+picW, dy+picH, 
				this);

		if (y*chessLevel + x == src)  {
			g.fillRect(x*picW, y*picH, picW, picH);
		}
	}

	/* ����ƶ�ͼ�� */
	void moveSegDraw(int seg, int x, int y, Graphics g)
	{
		int dx = seg % chessLevel * picW;
		int dy = seg / chessLevel * picH;

		g.drawImage(optImage,
				x, y, x+picW, y+picH,
				dx, dy, dx+picW, dy+picH, 
				this);
	}

	void setMoveXY(int x, int y)
	{
		if (x > 10 && y > 100) {
			movX = x-10;
			movY = y-100;
			repaint();
		}
	}

	void moveChess(int x, int y)
	{
		int origSrc = src;
		src = -1;

		int gox = (x - 10) / picW;
		int goy = (y - 100) / picH;

		if (gox >= chessLevel || gox < 0
				|| goy >= chessLevel || goy < 0)
			return;

		//����λ��
		int num = goy*chessLevel + gox;

		if (origSrc < 0 || num == origSrc ||
				check[num] == num || begin == 0) {
			repaint();
			return;
		}

		picBlockSwap(origSrc, num);
		repaint();
	}

	public void paint( Graphics g )
	{
		/* ��ͼ */
		for (int i = 0; i < colI; i++) {
			for (int j = 0; j < rowI; j++) {
				drawSeg(check[i*chessLevel+j], j, i, g);
			}
		}

		/* ����ƶ� */
		if (src >= 0) {
			moveSegDraw(check[src], movX, movY, g);
		}

		g.setColor( Color.WHITE );

		/* ������ */
		for (int i = 0; i < rowI; i++) 
			for (int j = 0; j < colI; j++)
				g.drawRect(i * picW, j * picH, picW,picH);

		/* ƴͼ��� */
		if( success ) { 
			g.drawImage( ico.getImage(), 0,0, width,height, this );
		}            
	}

	//��ȡ�����ĳߴ�
	public Dimension getPreferredSize()
	{
		return new Dimension( width,height );
	}

	//��ȡ���Ŀ� ��
	public int getSize( int x )
	{
		if(x==1)return width;
		else return height;
	}

	//�Ѷ�����
	public void setLevel( int x )
	{
		chessLevel = x;
		begin = 0;

		//�ػ�
		reInit();
		repaint();
	}

	/**********************************************
	 *����ͼƬ����
	 */
	public void picBlockSwap(int srcn, int dstn)
	{
		int temp = check[dstn];
		check[dstn] = check[srcn];
		check[srcn] = temp;

		isSuccess();
		repaint();

		return;
	}

	/**********************************************
	 * ��Ӧ�����
	 */
	public void selectChess(int x, int y)
	{
		int gox = (x - 10) / picW;
		int goy = (y - 100) / picH;

		if (gox >= chessLevel || gox < 0
				|| goy >= chessLevel || goy < 0)
			return;

		//����λ��
		int num = goy*chessLevel + gox;

		if (check[num] == num) return; /* λ����ȷ�Ĳ���Ҫ��ƴ */

		if (num == src || begin == 0) return;

		src = num;
	}

	//����
	public void reInit()
	{
		success = false;

		check = new int[ chessLevel*chessLevel ];
		for(int i=0;i<chessLevel*chessLevel;i++)
			check[i]=i;

		rowI = colI =chessLevel;
		crowIeSize();

		picH = height/rowI;
		picW = width/colI;

		bg.drawImage( img.getImage(), 0,0,width,height, this );
	}

	//�������
	public void pieceRandPlace()
	{
		reInit();

		int total = chessLevel*chessLevel;

		int step = 11+ total;
		int pos = (int)(Math.random()*total);

		//����������
		while(step>0)
		{
			int tmp = check[step%total];
			check[step%total] = check[pos];
			check[pos] = tmp;

			pos = (int)(Math.random()*total);
			step--;
		}

		begin = 1; //��ʼ

		repaint();
	}

	//����Ƿ�ƴͼ�ɹ�
	public void isSuccess()
	{
		success = true;

		for(int i=0; i<chessLevel*chessLevel; i++)
			if(check[i]!=i)
				success = false;
	}
} 
