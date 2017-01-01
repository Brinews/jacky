import java.awt.Color;
import java.awt.Component;
import java.awt.Graphics;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

import java.awt.*;
import java.awt.event.*;

import javax.swing.BorderFactory;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JPanel;

import javax.swing.*;
import javax.swing.table.*;

import java.io.*;
import java.util.*;

public class Mp3Player {
    public static void main(String[] args) {
        Mp3Frame frame = new Mp3Frame();
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setVisible(true);
    }
}

/* Three panel:
 *  Picture panel
 *  Control panel
 *  List panel
 */

class ListPanel extends JPanel implements MouseListener {
    private JScrollPane jsp; // albums list
    private JTable jt;
    private Model model;

    private JScrollPane likesPane; // players list
    private JTable likesTable;
    private Model likesModel;

    private JButton addToPlayerList;
    private JButton removeFromPlayerList;

    private InfoPanel infopane;
    private PicPanel picpane;

    private AlbumCollection pAcl;
    private Playlist pPlt;

    public ListPanel (AlbumCollection acl, Playlist plt,
            InfoPanel ipl, PicPanel ppl) {
        pAcl = acl;
        pPlt = plt;

        infopane = ipl;
        picpane = ppl;


        addAlbumList();
        addToPlayerList = new JButton("+ Add To Playlist");
        removeFromPlayerList = new JButton("- Remove Selected");

        addToPlayerList.addActionListener(new ActionListener() {
            /*
             * add to playlist
             */
            public void actionPerformed(ActionEvent event) {
                int row = jt.getSelectedRow();
                if (row >= 0) {
                    pPlt.addTrackToPlaylist(
                        (String)model.getValueAt(row, 1),
                        (String)model.getValueAt(row, 2));
                    updatePlayList();
                }
            }
        });

        removeFromPlayerList.addActionListener(new ActionListener() {
            /*
             * remove from playlist
             */
            public void actionPerformed(ActionEvent event) {
                int row = likesTable.getSelectedRow();
                if (row >= 0) {
                    pPlt.removeTrack(row);
                    updatePlayList();
                }
            }
        });

        add(addToPlayerList);
        add(removeFromPlayerList);
        addPlayerList();
    }

    public void addAlbumList () {
        model=new Model(pAcl, pPlt);
        jt=new JTable(model){
            public Component prepareRenderer(TableCellRenderer renderer,
                    int row, int column) {
                Component c = super.prepareRenderer(renderer, 
                        row, column);
                if (c instanceof JComponent) {
                    ((JComponent) c).setOpaque(false);
                }
                return c;
            }
        };

        jt.setPreferredScrollableViewportSize(new Dimension(280, 200));  
        jt.setAutoResizeMode(JTable.AUTO_RESIZE_SUBSEQUENT_COLUMNS);  

        jt.setOpaque(false);
        jt.setRowHeight(20);
        jt.setSelectionMode(ListSelectionModel.SINGLE_SELECTION );
        jt.setShowVerticalLines(false);

        jsp = new JScrollPane(jt);
        jt.addMouseListener(this);

        add(new JLabel("All Albums"));
        add(jsp, BorderLayout.CENTER);
    }

    public void updateAlbumList() {
        model.updateAlbumlist();
        model.fireTableDataChanged();
    }

    public void addPlayerList () {
        likesModel =new Model(pAcl, pPlt);
        likesTable=new JTable(likesModel);

        likesTable.setPreferredScrollableViewportSize(new Dimension(280, 200));  
        likesTable.setAutoResizeMode(JTable.AUTO_RESIZE_SUBSEQUENT_COLUMNS);  

        likesTable.setRowHeight(20);
        likesTable.setShowVerticalLines(false);
        likesTable.setSelectionMode(ListSelectionModel.SINGLE_SELECTION );

        likesPane = new JScrollPane(likesTable);

        likesTable.addMouseListener(this);

        add(new JLabel("Your Favourites"));
        add(likesPane, BorderLayout.CENTER);
    }

    public void updatePlayList() {
        likesModel.updatePlaylist();
        likesModel.fireTableDataChanged();
    }

    @Override
	public void mouseClicked(MouseEvent e) {
        //System.out.println(e);

        /*
         * update right info panel and picture panel
         */
        if (e.getSource() == jt) {
            int row = jt.getSelectedRow();
            Album alb = pAcl.getAlbum(row);
            
            if (alb != null) {
                picpane.updatePic(alb);
                infopane.updateInfo(alb);
            }
        }

        if (e.getSource() == likesTable) {
            int row = likesTable.getSelectedRow();
            Album alb = pPlt.getAlbum(row);

            if (alb != null) {
                picpane.updatePic(alb);
                infopane.updateInfo(alb);
            }
        }
    }

    	@Override
	public void mouseEntered(MouseEvent e) {
		// TODO Auto-generated method stub
	}
    
	@Override
	public void mouseExited(MouseEvent e) {
		// TODO Auto-generated method stub
	}

    @Override
	public void mousePressed(MouseEvent e) {
		// TODO Auto-generated method stub
	}

	@Override
	public void mouseReleased(MouseEvent e) {
		// TODO Auto-generated method stub
	}
}

class ControlPanel extends JPanel {

    private JButton open;
    private JButton prev;
    private JButton next;
    private JSlider jSliderPlayProgress;

    private Thread playthread;

    MediaPlayer player = null;

    public ControlPanel() {

        player = new MediaPlayer();

        open = new JButton("Start");
        prev = new JButton("<<");
        next = new JButton(">>");

        open.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                playthread = new PlayThread();
                playthread.start();
            }
        });

        add(prev);
        add(open);
        add(next);

        jSliderPlayProgress = new JSlider();
        jSliderPlayProgress.setValue(0);
        jSliderPlayProgress.setEnabled(false);
        jSliderPlayProgress.setPreferredSize(new Dimension(200, 20));

        add(jSliderPlayProgress);
    }

	public class PlayThread extends Thread {
		@Override
		public void run() {
			try {
				playAll();
			} catch (IOException e) {
				e.printStackTrace();
			} 
		}
	}
    
	public void playAll() throws IOException {
		while (true) {
			if (player.isStop()) {
				try {
                    System.out.println("begin");
                    File song = new File("test.mp3");
                    player.Play(song.getAbsolutePath());
                    Thread.sleep(1000);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
            }
		}
	}
}

/*
 * display album information
 */
class InfoPanel extends JPanel {
    private JLabel albumName;
    private JLabel artistName;
    private JLabel totalTracks;
    private JLabel totalDuration;

    public InfoPanel() {
        setLayout(new GridLayout(2, 4));

        JLabel name = new JLabel("Album Name:");
        albumName = new JLabel("");
        add (name); add(albumName);

        JLabel art = new JLabel("Album Artist:");
        artistName = new JLabel("");
        add (art); add(artistName);

        JLabel tracks = new JLabel("Tracks Num:");
        totalTracks = new JLabel("");
        add (tracks); add(totalTracks);

        JLabel duration = new JLabel("Duration(sec):");
        totalDuration = new JLabel("");
        add (duration); add(totalDuration);
    }

    public void updateInfo(Album alb) {
        albumName.setText(alb.getAtitle());
        artistName.setText(alb.getAname());
        totalTracks.setText(""+alb.getTrackList().size());
        totalDuration.setText(alb.getTotalDurationString());
    }
}

class PicPanel extends JPanel {

    private JLabel faceImageLabel;

    public PicPanel() {
        faceImageLabel = new JLabel();
        faceImageLabel.setBounds(125, 100, 200, 200);
        setBackground(Color.WHITE);
        setOpaque(false);
        setLayout(null);

        add(faceImageLabel);
    }

    public void paintComponent(Graphics g) {
        super.paintComponent(g);
    }

    public void updatePic(Album alb) {
        ImageIcon icon = new ImageIcon("images/"+alb.getAtitle()+".jpg"); 
        faceImageLabel.setIcon(icon);
        faceImageLabel.setBounds(125, 100, icon.getIconWidth(), icon.getIconHeight());
        add(faceImageLabel);
    }
}

class Mp3Frame extends JFrame {

    /* panels */
    private ListPanel lstpane;
    private PicPanel picpane;
    private ControlPanel ctlpane;
    private InfoPanel infopane;

    AlbumCollection acl; // all albums
    Playlist plt; // the playlist

    /* menu */
    private JMenuBar bar;
    private JMenu openMenu;
    private JMenuItem loadAlbumItem;
    private JMenuItem loadPlistItem;
    private JMenuItem saveAlbumItem;
    private JMenuItem savePlistItem;
    private JMenuItem closeItem;
    private JMenu helpMenu;
    private JMenuItem aboutItem;

    private final int width = 800;
    private final int height = 600;

    /* file open/save */
    private FileDialog openDia, saveDia;

    public Mp3Frame() {
        acl = new AlbumCollection();
        plt = new Playlist(acl);

        initPanel();
        initEvent();
    }

    private void initPanel() {

        setSize(width, height);
        setLayout(null);
        this.setResizable(false);

        openDia = new FileDialog(this, "Open", FileDialog.LOAD);
        saveDia = new FileDialog(this, "Save", FileDialog.SAVE);

        /* add menus */
        bar = new JMenuBar();
        openMenu = new JMenu("File");
        loadAlbumItem = new JMenuItem("Load Albums List");
        loadPlistItem = new JMenuItem("Load Player List");
        saveAlbumItem = new JMenuItem("Save Albums List");
        savePlistItem = new JMenuItem("Save Player List");
        closeItem = new JMenuItem("Quit");

        openMenu.add(loadAlbumItem);
        openMenu.add(loadPlistItem);
        openMenu.add(saveAlbumItem);
        openMenu.add(savePlistItem);
        openMenu.add(closeItem);

        helpMenu = new JMenu("Help");
        aboutItem = new JMenuItem("About");

        helpMenu.add(aboutItem);

        bar.add(openMenu);
        bar.add(helpMenu);

        setJMenuBar(bar);

        int leftWidth = 300;
        int topHeight = 60;
        int botHeight = 100;

        /* add right top details panel */
        infopane = new InfoPanel();
        infopane.setBounds(leftWidth, 0, width-leftWidth, 
                topHeight);
        infopane.setBorder(BorderFactory.createLineBorder(Color.darkGray));
        add(infopane);

        /* add right center pic panel */
        picpane = new PicPanel();
        picpane.setBounds(leftWidth, topHeight, 
                width-leftWidth, height-topHeight-botHeight);
        picpane.setBorder(BorderFactory.createLineBorder(Color.darkGray));
        add(picpane);

        /* add left list panel */
        lstpane = new ListPanel(acl, plt, infopane, picpane);
        lstpane.setBounds(0, 0, leftWidth, height);
        lstpane.setBorder(BorderFactory.createLineBorder(Color.darkGray));
        add(lstpane);

        /* add right bot control panel */
        ctlpane = new ControlPanel();
        ctlpane.setBounds(leftWidth, height-botHeight, 
                width-leftWidth, botHeight);
        ctlpane.setBorder(BorderFactory.createLineBorder(Color.darkGray));
        add(ctlpane);
    }

    private void initEvent() {
        
        loadAlbumItem.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                openDia.setVisible(true);
                String dirpath = openDia.getDirectory();
                String fileName = openDia.getFile();
                
                if (dirpath == null || fileName == null) return;
                File file = new File(dirpath, fileName);

                PlayerFactory.loadAlbums(acl, file);

                lstpane.updateAlbumList();
            }
        });
        
        saveAlbumItem.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                saveDia.setVisible(true);
                String dirpath = saveDia.getDirectory();
                String fileName = saveDia.getFile();

                if (dirpath == null || fileName == null) return;

                File file=new File(dirpath,fileName);
                PlayerFactory.saveAlbums(acl, file);
            }
        });
 
        loadPlistItem.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                openDia.setVisible(true);
                String dirpath = openDia.getDirectory();
                String fileName = openDia.getFile();
                
                if (dirpath == null || fileName == null) return;
                File file = new File(dirpath, fileName);

                PlayerFactory.loadPlaylist(plt, file);
                lstpane.updatePlayList();
            }
        });
        
        savePlistItem.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                saveDia.setVisible(true);
                String dirpath = saveDia.getDirectory();
                String fileName = saveDia.getFile();

                if (dirpath == null || fileName == null) return;

                File file=new File(dirpath,fileName);
                PlayerFactory.savePlaylist(plt, file);
            }
        });


        closeItem.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                System.exit(0);
            }
        });

        aboutItem.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                JOptionPane.showMessageDialog(null, 
                    "Add/Save tracks by file loading\nAdd/Del tracks to your favourites\nView album details by clicking\nStart to listen the songs\nDate: 2017-01-01\nUse It Freely!"
                    );
            }
        });
    }
}

