import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.InputEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.util.ArrayList;
import java.util.List;

import java.io.File;
import java.io.IOException;
import java.net.URL;

import javax.imageio.ImageIO;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;

import javax.swing.JToolBar;

import org.dom4j.*;
import org.dom4j.io.OutputFormat;
import org.dom4j.io.XMLWriter;
import java.io.*;

import java.util.Calendar;
import java.util.Collections;
import java.util.Comparator;
import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;

import javax.sound.midi.MidiEvent;
import javax.sound.midi.MidiSystem;
import javax.sound.midi.Sequence;
import javax.sound.midi.Sequencer;
import javax.sound.midi.ShortMessage;
import javax.sound.midi.Track;

public class Piano extends JFrame{
	
	private PianoPanel p;
	
	public Piano()
	{
		setSize(500,680);
		setDefaultCloseOperation(EXIT_ON_CLOSE);
		
		// 增加钢琴谱画面
		p = new PianoPanel();
		add(p);
		
		setVisible(true);
	}
	
	public PianoPanel getPianoPanel() { return p; }
	
	// 定时保存曲谱，每隔2秒保存一次
	public static void XmlTimerSave(final Piano pianoPane) {
        Timer timer = new Timer();
        timer.scheduleAtFixedRate(new TimerTask() {
            public void run() {
            	Dom4jXml.WriteXml(pianoPane.getPianoPanel().getSheetPane().myNotes);
            }
        }, 1000, 2000);
    }
	
	public static void main(String[] args) {
		Piano p = new Piano();
		XmlTimerSave(p);
	}
	
	//主要控制面板
	class PianoPanel extends JPanel implements ActionListener {
		
		private SheetPane sheet = null;
		private String cmdName = null;
		private MiniMusic mm = null;
		private boolean playOnce = false;

		public PianoPanel() {
			super(new BorderLayout());
			
			Resource.loadImages(); //加载使用图片
			
			JToolBar toolBar = new JToolBar("Piano Notes");
			addButtons(toolBar);
			
			sheet = new SheetPane(this);
			SheetMouseListener sml = new SheetMouseListener(sheet, this);
			
			sheet.addMouseListener(sml);

			setPreferredSize(new Dimension(650, 130));
			
			//添加工具栏
			add(toolBar, BorderLayout.PAGE_START);
			add(sheet, BorderLayout.CENTER);
			
			cmdName = "mode";
			mm = new MiniMusic();
			mm.setSheetPane(sheet);
		}
		
		public SheetPane getSheetPane()
		{
			return sheet;
		}

		@Override
		public void actionPerformed(ActionEvent arg0) {
			String cmd = arg0.getActionCommand();
			cmdName = cmd;
			
			//播放事件控制
			if (cmdName.equals("on")) {
				if (playOnce == false) { 
					playOnce = true;
					mm.start();
				}
				
				if (playOnce == true) mm.runPlay();
			}
			
			if (cmdName.equals("off")) {
				mm.stopPlay();
			}
		}
		
		public String getCommandName() {
			return cmdName;
		}
		
		//添加工具栏按钮
		protected void addButtons(JToolBar toolBar) {
		    JButton button = null;

		    for (int i = 1; i < 6; i++) {
		    	button = makeNavigationButton((2<<i)+"thNote", (2<<i)+"thNote",
		    			(2<<i)+"thNote", (2<<i)+"thNote");
		    	toolBar.add(button);
		    }

		    button = makeNavigationButton("dot", "dot",
		                                  "dot", "dot");
		    toolBar.add(button);

		    button = makeNavigationButton("wholeNote", "wholeNote",
		                                  "wholeNote", "wholeNote");
		    toolBar.add(button);
		    
		    button = makeNavigationButton("mode", "mode",
		    		"mode", "mode");
		    toolBar.add(button);
		    
		    button = makeNavigationButton("on", "on", "on", "on");
		    toolBar.add(button);
		    
		    button = makeNavigationButton("off", "off", "off", "off");
		    toolBar.add(button);
		}

		protected JButton makeNavigationButton(String imageName,
		                                       String actionCommand,
		                                       String toolTipText,
		                                       String altText) {
		    //Look for the image.
		    String imgLocation = "images/"
		                         + imageName
		                         + ".png";
		    URL imageURL = PianoPanel.class.getResource(imgLocation);

		    //Create and initialize the button.
		    JButton button = new JButton();
		    button.setActionCommand(actionCommand);
		    button.setToolTipText(toolTipText);
		    button.addActionListener(this);

		    if (imageURL != null) {                      //image found
		        button.setIcon(new ImageIcon(imageURL, altText));
		    } else {                                     //no image found
		        button.setText(altText);
		        System.err.println("Resource not found: " + imgLocation);
		    }

		    return button;
		}
	}
	
	// 打谱区 : 绘制琴线
	class SheetPane extends JPanel {
		public int WGAP = 30;
		public int SECT = 80;
		public int HGAP = 180;
		
		public SheetPane(PianoPanel p) {
			
			piano = p;
			
			setSize(Resource.SHEET_WIDTH, Resource.SHEET_HEIGHT);
			setLayout(null);
			setBackground(Color.WHITE);
			setVisible(true);
			
			//添加前导符
			for (int i = 0; i < Resource.LINE_NUMBER; i++) {
				Note n = new Note(0, 1, WGAP, HGAP*i+WGAP, Resource.NOTE_WIDTH, 
						Resource.HEAD_HEIGHT, this);
				n.setImage("g8", Resource.getNoteImage("g8"));
				this.add(n);
				
				n = new Note(0, 1, WGAP, HGAP*i+WGAP+Resource.HEAD_HEIGHT, 
						Resource.NOTE_WIDTH, Resource.HEAD_HEIGHT, this);
				n.setImage("g4", Resource.getNoteImage("g4"));
				this.add(n);
			}
		}
		
		/* 
		 * 右键删除
		 */
		public void removeNote(Note n) 
		{
			for (int i = 0; i < myNotes.size(); i++)
				if (myNotes.get(i) == n) {
					myNotes.remove(i);
					break;
				}
			
			this.remove(n);
		}
		
		public void paintComponent(Graphics g){
		      super.paintComponent(g);
		      
		      // 画行线
		      g.setColor(Color.BLACK);
		      
		      for (int i = 0; i < Resource.LINE_NUMBER; i++) {
		    	  g.drawLine(WGAP+Resource.NOTE_WIDTH, WGAP+HGAP*i+18, 
		    			  Resource.SHEET_WIDTH, WGAP+HGAP*i+18);
		    	  g.drawLine(WGAP+Resource.NOTE_WIDTH, WGAP+HGAP*i+25, 
		    			  Resource.SHEET_WIDTH, WGAP+HGAP*i+25);
		    	  g.drawLine(WGAP+Resource.NOTE_WIDTH, WGAP+HGAP*i+31, 
		    			  Resource.SHEET_WIDTH, WGAP+HGAP*i+31);
		    	  g.drawLine(WGAP+Resource.NOTE_WIDTH, WGAP+HGAP*i+38, 
		    			  Resource.SHEET_WIDTH, WGAP+HGAP*i+38);
		    	  g.drawLine(WGAP+Resource.NOTE_WIDTH, WGAP+HGAP*i+44, 
		    			  Resource.SHEET_WIDTH, WGAP+HGAP*i+44);
		    	  
		    	  g.drawLine(WGAP+Resource.NOTE_WIDTH, WGAP+HGAP*i+Resource.NOTE_HEIGHT+42, 
		    			  Resource.SHEET_WIDTH, WGAP+HGAP*i+Resource.NOTE_HEIGHT+42);
		    	  g.drawLine(WGAP+Resource.NOTE_WIDTH, WGAP+HGAP*i+Resource.NOTE_HEIGHT+48, 
		    			  Resource.SHEET_WIDTH, WGAP+HGAP*i+Resource.NOTE_HEIGHT+48);
		    	  g.drawLine(WGAP+Resource.NOTE_WIDTH, WGAP+HGAP*i+Resource.NOTE_HEIGHT+54, 
		    			  Resource.SHEET_WIDTH, WGAP+HGAP*i+Resource.NOTE_HEIGHT+54);
		    	  g.drawLine(WGAP+Resource.NOTE_WIDTH, WGAP+HGAP*i+Resource.NOTE_HEIGHT+60, 
		    			  Resource.SHEET_WIDTH, WGAP+HGAP*i+Resource.NOTE_HEIGHT+60);
		    	  g.drawLine(WGAP+Resource.NOTE_WIDTH, WGAP+HGAP*i+Resource.NOTE_HEIGHT+66, 
		    			  Resource.SHEET_WIDTH, WGAP+HGAP*i+Resource.NOTE_HEIGHT+66);
		    	  
		    	  // 画列线
			      for (int j = 0; j < Resource.SHEET_WIDTH/SECT+1; j++) {
			    	  g.drawLine(WGAP+Resource.NOTE_WIDTH+j*SECT, 
			    			  WGAP+HGAP*i+18, 
			    			  WGAP+Resource.NOTE_WIDTH+j*SECT, 
			    			  WGAP+HGAP*i+44);
			    	  
			    	  g.drawLine(WGAP+Resource.NOTE_WIDTH+j*SECT, 
			    			  WGAP+HGAP*i+Resource.NOTE_HEIGHT+42, 
			    			  WGAP+Resource.NOTE_WIDTH+j*SECT, 
			    			  WGAP+HGAP*i+Resource.NOTE_HEIGHT+66);
			      }
		      }
		}
		
		public PianoPanel piano;
		public List<Note> myNotes = new ArrayList<Note>();
	}
	
	class SheetMouseListener extends MouseAdapter{
		public SheetPane sheet;
		public PianoPanel piano;
		
		public SheetMouseListener(SheetPane s, PianoPanel p)
		{
			sheet = s;
			piano = p;
		}
		
		// 根据用户选择及鼠标点击添加曲谱元素
		public void mouseClicked(MouseEvent event)
		{
			// 非曲谱符号
			if (piano.getCommandName().equals("mode")
					|| piano.getCommandName().equals("on")
					|| piano.getCommandName().equals("off"))
				return;
			
			// 鼠标左击
			int mods = event.getModifiers();
			if ((mods & InputEvent.BUTTON1_MASK) != 0) {

				int x = event.getPoint().x;
				int y = event.getPoint().y;

				int p = -1;
				int level = 0;

				// 判断是否在曲谱线上
				for (int i = 0; i < Resource.LINE_NUMBER; i++) {
					if (x >= sheet.WGAP+Resource.NOTE_WIDTH && x <= Resource.SHEET_WIDTH
							&& y >= sheet.WGAP+sheet.HGAP*i+18 
							&& y <= sheet.WGAP+sheet.HGAP*i+Resource.NOTE_HEIGHT+66) {
						level = (y-sheet.WGAP+sheet.HGAP*i-18)/6;
						p = i;
						break;
					}
				}

				// 在曲谱线上才添加曲谱元素
				if (p >= 0) {
					Note m = new Note(p, level, x, y, Resource.NOTE_WIDTH, Resource.NOTE_HEIGHT, sheet);
					m.setImage(piano.getCommandName(), Resource.getNoteImage(piano.getCommandName()));
					sheet.add(m);
					sheet.repaint();

					sheet.myNotes.add(m);
				}
			}
		}

		public void mousePressed(MouseEvent event)
		{
		}
	}

	// 琴谱元素
	class Note extends JPanel {

		public int width, height; //曲谱图片大小
		public int posx, posy; //曲谱元素位置
		public int partno, level; //曲谱类型，标号
		private Image image; //曲谱元素图片
		private String type = null;

		public Note(int p, int l, int posX, int posY, int width, int height, SheetPane sp)
		{
			setLayout(null);
			setSize(width, height);
			setBounds(posX, posY,width, height);
			setVisible(true);

			this.partno = p;
			this.level = l;
			
			this.posx = posX;
			this.posy = posY;
			
			this.width = width;
			this.height = height;
			
			this.addMouseListener(new NoteMouseListener(sp));
		}
		
		public void paintComponent(Graphics g){
		      super.paintComponent(g);
		      g.drawImage(image,0,0, width, height,null);
		}     
		
		public void setImage(String t, Image img)
		{
			image=img;
			type = t;
		}
		
		public Image getImage()
		{
			return image;
		}
		
		public String getType() 
		{
			return type;
		}
		
		public int getLevel()
		{
			return level;
		}
	}
	
	// 打谱事件监听
	class NoteMouseListener extends MouseAdapter{
		private SheetPane sheet;
		
		public NoteMouseListener(SheetPane sp)
		{
			sheet = sp;
		}
		
		public void mouseClicked(MouseEvent event)
		{
			int mods = event.getModifiers();
			if ((mods & InputEvent.BUTTON3_MASK) != 0) {
				//右键删除
				sheet.removeNote((Note)event.getSource());
			}
		}
		
		public void mousePressed(MouseEvent event)
	    {
	    }
	}
	
	//资源名称
	static class Resource {
		public final static int SHEET_WIDTH = 380;
		public final static int SHEET_HEIGHT = 300;
		public final static int LINE_NUMBER = 3;
		
		public final static int NOTE_WIDTH = 30;
		public final static int NOTE_HEIGHT = 35;
		
		public final static int HEAD_HEIGHT = 60;
		
		public static Image[] NOTE_IMAGES;
		public final static String[] NOTE_NAMES = {"2thNote", "4thNote", "8thNote",
			"16thNote", "32thNote", "64thNote", "dot", "wholeNote", "mode", "g8", "g4"};
		
		public static void loadImages()
		{
			NOTE_IMAGES = new Image[NOTE_NAMES.length];
			
	        //For each type of piece...
			for (int i = 0; i < NOTE_NAMES.length; i++)
			{
	            // ...construct the appropriate file name..
				String filePieceName = NOTE_NAMES[i];
				File imageFile = new File("images" + File.separator + filePieceName + ".png");
	            
	            // ...and try to load the corresponding image file.
				try
				{
					NOTE_IMAGES[i] = ImageIO.read(imageFile);
				}
				catch (IOException e)
				{
					e.printStackTrace();
				}
			}
		}
		
		public static Image getNoteImage(String noteName) {
			for (int i = 0; i < NOTE_NAMES.length; i++)
			{
				if (noteName.equals(NOTE_NAMES[i]))
					return NOTE_IMAGES[i];
			}
			
			return null;
		}
	}
	
	// 写XML乐谱文件
	static class Dom4jXml {
		public static void WriteXml(List<Note> notes) {
			Document doc = DocumentHelper.createDocument();
			
			Element root = doc.addElement("music-piano");
			
			// 格式 比较简单：part/type/voice 
			for (int i = 0; i < notes.size(); i++) {

				Element eltNote = root.addElement("part").addAttribute("no", "" + notes.get(i).partno);
				Element eltType = eltNote.addElement("type");
				Element eltVoice = eltNote.addElement("voice");
				eltType.setText(notes.get(i).getType());
				eltVoice.setText(""+notes.get(i).getLevel());
			}

			try {
				OutputFormat format = new OutputFormat("    ", true);
				format.setEncoding("gb2312");
				XMLWriter xmlWriter = new XMLWriter(new PrintWriter(new FileOutputStream("music.xml")), format);
				xmlWriter.write(doc);
				xmlWriter.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	
	/**
	 *    MiniMusic
	 */
	class MiniMusic extends Thread {
		
		private SheetPane sp = null;
		Sequencer player;
		List<Note> sortNotes = new ArrayList<Note>();
		
		public void setSheetPane(SheetPane s) {
			sp = s;
		}
		
		Comparator<Note> comparator = new Comparator<Note>(){  
			public int compare(Note s1, Note s2) {  
				//先排X 
				if(s1.posx != s2.posx){  
					return s1.posx - s2.posx;  
				}  
				else{  
					//再排Y
					if(s1.posy != s2.posy){  
						return s1.posy - s2.posy;
					}  
					
					return 0;
				}  
			}  
		};  

		/*
		 * 进行播放
		 */
	    public void play(){
	        try {
	            player = MidiSystem.getSequencer();
	            player.open();
	            //节拍器
	            Sequence seq = new Sequence(Sequence.PPQ,4);
	            Track track = seq.createTrack();
	            
	            sortNotes.clear();
	            sortNotes.addAll(sp.myNotes);
	            Collections.sort(sortNotes, comparator);
	            
	            for (int i = 0; i < sortNotes.size(); i++)
	            {
	            	String type = sortNotes.get(i).type;
	            	int level = sortNotes.get(i).level;
	            	int t = 0;
	            	
	            	for (int j = 0; j < Resource.NOTE_NAMES.length; j++) {
	            		if (type.equals(Resource.NOTE_NAMES[j])) {
	            			t = j;
	            		}
	            	}
	            	
	            	track.add(makeEvent(144, 1, level+10, 100, t));
	            	track.add(makeEvent(128, 1, level+10, 100, t+2));
	            }
	            
	            player.setSequence(seq);
	            player.setLoopCount(player.LOOP_CONTINUOUSLY);    //指定无穷的重复次数
	            player.setTempoInBPM(120);
	            player.start();
	        } catch (Exception e) {
	        }
	    }
	    
	    /**
	     * MidiEvent是组合乐曲的指令,一连串的MidiEvent就好像是乐谱一样.
	     * MidiEvent用来指示在何时执行什么操作,每个指令都必须包括该指令的执行时机.也就是说,乐声应该在那一拍发响.
	     * 
	     * @param comd    
	     *                 chmod代表信息类型 144类型的信息代表NOTE ON表示打开 128代表NOTE OFF 表示关闭
	     * @param chan
	     *                 chan表示频道,每个频道代表不同的演奏者.
	     *                 例如:一号频道是吉他,二号频道是Bass.或者可以像IronMaiden用3把不同音色的吉他编制
	     * @param one    
	     *                 one表示音符,从0~127代表不同音高
	     * @param two
	     *                 two代表音道/音量,用多大的音道按下? 0几乎听不到,100算是差不多
	     * @param tick
	     * @return
	     */
	    public MidiEvent makeEvent(int comd, int chan, int one, int two, int tick){
	        MidiEvent event = null;
	        try {
	            ShortMessage a = new ShortMessage();
	            a.setMessage(comd,chan,one,two);
	            //表示在tick拍启动a这个Message
	            event = new MidiEvent(a, tick);
	        } catch (Exception e) {
	            e.printStackTrace();
	        }
	        return event;
	    }
	    
	    public void run() {
	    	play();
	    }
	    
	    // 播放停止
	    public void stopPlay() 
	    {
	    	player.stop();
	    }
	    
	    // 播放开始
	    public void runPlay()
	    {
	    	player.start();
	    }
	}
}
