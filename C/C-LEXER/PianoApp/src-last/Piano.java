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
		
		// ���Ӹ����׻���
		p = new PianoPanel();
		add(p);
		
		setVisible(true);
	}
	
	public PianoPanel getPianoPanel() { return p; }
	
	// ��ʱ�������ף�ÿ��2�뱣��һ��
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
	
	//��Ҫ�������
	class PianoPanel extends JPanel implements ActionListener {
		
		private SheetPane sheet = null;
		private String cmdName = null;
		private MiniMusic mm = null;
		private boolean playOnce = false;

		public PianoPanel() {
			super(new BorderLayout());
			
			Resource.loadImages(); //����ʹ��ͼƬ
			
			JToolBar toolBar = new JToolBar("Piano Notes");
			addButtons(toolBar);
			
			sheet = new SheetPane(this);
			SheetMouseListener sml = new SheetMouseListener(sheet, this);
			
			sheet.addMouseListener(sml);

			setPreferredSize(new Dimension(650, 130));
			
			//��ӹ�����
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
			
			//�����¼�����
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
		
		//��ӹ�������ť
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
	
	// ������ : ��������
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
			
			//���ǰ����
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
		 * �Ҽ�ɾ��
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
		      
		      // ������
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
		    	  
		    	  // ������
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
		
		// �����û�ѡ��������������Ԫ��
		public void mouseClicked(MouseEvent event)
		{
			// �����׷���
			if (piano.getCommandName().equals("mode")
					|| piano.getCommandName().equals("on")
					|| piano.getCommandName().equals("off"))
				return;
			
			// ������
			int mods = event.getModifiers();
			if ((mods & InputEvent.BUTTON1_MASK) != 0) {

				int x = event.getPoint().x;
				int y = event.getPoint().y;

				int p = -1;
				int level = 0;

				// �ж��Ƿ�����������
				for (int i = 0; i < Resource.LINE_NUMBER; i++) {
					if (x >= sheet.WGAP+Resource.NOTE_WIDTH && x <= Resource.SHEET_WIDTH
							&& y >= sheet.WGAP+sheet.HGAP*i+18 
							&& y <= sheet.WGAP+sheet.HGAP*i+Resource.NOTE_HEIGHT+66) {
						level = (y-sheet.WGAP+sheet.HGAP*i-18)/6;
						p = i;
						break;
					}
				}

				// ���������ϲ��������Ԫ��
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

	// ����Ԫ��
	class Note extends JPanel {

		public int width, height; //����ͼƬ��С
		public int posx, posy; //����Ԫ��λ��
		public int partno, level; //�������ͣ����
		private Image image; //����Ԫ��ͼƬ
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
	
	// �����¼�����
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
				//�Ҽ�ɾ��
				sheet.removeNote((Note)event.getSource());
			}
		}
		
		public void mousePressed(MouseEvent event)
	    {
	    }
	}
	
	//��Դ����
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
	
	// дXML�����ļ�
	static class Dom4jXml {
		public static void WriteXml(List<Note> notes) {
			Document doc = DocumentHelper.createDocument();
			
			Element root = doc.addElement("music-piano");
			
			// ��ʽ �Ƚϼ򵥣�part/type/voice 
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
				//����X 
				if(s1.posx != s2.posx){  
					return s1.posx - s2.posx;  
				}  
				else{  
					//����Y
					if(s1.posy != s2.posy){  
						return s1.posy - s2.posy;
					}  
					
					return 0;
				}  
			}  
		};  

		/*
		 * ���в���
		 */
	    public void play(){
	        try {
	            player = MidiSystem.getSequencer();
	            player.open();
	            //������
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
	            player.setLoopCount(player.LOOP_CONTINUOUSLY);    //ָ��������ظ�����
	            player.setTempoInBPM(120);
	            player.start();
	        } catch (Exception e) {
	        }
	    }
	    
	    /**
	     * MidiEvent�����������ָ��,һ������MidiEvent�ͺ���������һ��.
	     * MidiEvent����ָʾ�ں�ʱִ��ʲô����,ÿ��ָ����������ָ���ִ��ʱ��.Ҳ����˵,����Ӧ������һ�ķ���.
	     * 
	     * @param comd    
	     *                 chmod������Ϣ���� 144���͵���Ϣ����NOTE ON��ʾ�� 128����NOTE OFF ��ʾ�ر�
	     * @param chan
	     *                 chan��ʾƵ��,ÿ��Ƶ������ͬ��������.
	     *                 ����:һ��Ƶ���Ǽ���,����Ƶ����Bass.���߿�����IronMaiden��3�Ѳ�ͬ��ɫ�ļ�������
	     * @param one    
	     *                 one��ʾ����,��0~127����ͬ����
	     * @param two
	     *                 two��������/����,�ö�����������? 0����������,100���ǲ��
	     * @param tick
	     * @return
	     */
	    public MidiEvent makeEvent(int comd, int chan, int one, int two, int tick){
	        MidiEvent event = null;
	        try {
	            ShortMessage a = new ShortMessage();
	            a.setMessage(comd,chan,one,two);
	            //��ʾ��tick������a���Message
	            event = new MidiEvent(a, tick);
	        } catch (Exception e) {
	            e.printStackTrace();
	        }
	        return event;
	    }
	    
	    public void run() {
	    	play();
	    }
	    
	    // ����ֹͣ
	    public void stopPlay() 
	    {
	    	player.stop();
	    }
	    
	    // ���ſ�ʼ
	    public void runPlay()
	    {
	    	player.start();
	    }
	}
}
