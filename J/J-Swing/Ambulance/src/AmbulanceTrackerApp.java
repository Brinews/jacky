/*
 * Your name, UPI 
 *
 * Main control of actions
 */

/*
 *  =============================================================
 *  AmbulanceTrackerApp.java : Extends JFrame and contains a panel
 *  where control the main logic
 *  ==============================================================
 */

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.ArrayList;

import javax.swing.border.*;
import javax.swing.event.*;

class MainFramePanel extends JPanel {
    JButton patientButton, ambulanceButton, exitButton;
    JLabel titleLabel;
    ActionListener clkAction;
    ControlPanel app;
    
    public MainFramePanel(ControlPanel ata) {
    	app = ata;
    	setLayout(new BorderLayout(150, 30));
    	
    	JPanel entryPane = new JPanel();
    	entryPane.setLayout(new GridLayout(4, 1, 0, 30));
    	
    	clkAction = new ActionListener() {
    		public void actionPerformed(ActionEvent event) {
    			JButton src = (JButton) event.getSource();
    			if (src == patientButton) {
    				app.changeLayout("2");
    			} else if (src == ambulanceButton) {
    				app.changeLayout("1");
    			} else if (src == exitButton) {
    				System.exit(0);
    			}
    		}
    	};
    	
    	titleLabel = new JLabel("    Ambulance Tracker System");
    	titleLabel.setFont(new Font("Serif", Font.BOLD, 40));
    	
    	patientButton = new JButton("Patients");
    	patientButton.addActionListener(clkAction);
    	entryPane.add(patientButton);
    	
    	ambulanceButton = new JButton("Ambulances");
    	ambulanceButton.addActionListener(clkAction);
    	entryPane.add(ambulanceButton);
    	
    	exitButton = new JButton("Exit");
    	exitButton.addActionListener(clkAction);
    	entryPane.add(exitButton);
    	
    	add(titleLabel, BorderLayout.NORTH);
    	add(new JLabel("  "), BorderLayout.EAST);
    	add(new JLabel("  "), BorderLayout.WEST);
    	add(entryPane, BorderLayout.CENTER);
    }
}

public class AmbulanceTrackerApp extends JFrame {
	ControlPanel controlPanel;
	
    /** main method
     */
    public static void main(String[] args) {
    	new AmbulanceTrackerApp();
    }

    /** constructor to initialise components
     */
    public AmbulanceTrackerApp() {
        super("Ambulance Tracking Application");
        controlPanel = new ControlPanel();
        
        add(controlPanel);
        
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(600, 480);
        Dimension d = Toolkit.getDefaultToolkit().getScreenSize();
        Dimension frameSize = getSize();
        setLocation((d.width - frameSize.width) / 2, (d.height - frameSize.height) / 2);
        setVisible(true);
    }
}

class ControlPanel extends JPanel {
	MainFramePanel panel;  // panel for main frame
    AmbulanceList ambPane;
    PatientsList patPane;
    PatientUpdate patAddPane;
    AmbulanceUpdate ambAddPane;
    
    CardLayout card = new CardLayout();
    JPanel viewPanel = new JPanel(card);
    
	public ControlPanel(){
		panel = new MainFramePanel(this);
        ambPane = new AmbulanceList(this);
        patPane = new PatientsList(this);
        patAddPane = new PatientUpdate(ambPane, patPane, this);
        ambAddPane = new AmbulanceUpdate(ambPane, patPane, this);
        patPane.setAddPane(patAddPane);
        ambPane.setAddPane(ambAddPane);
        
        setLayout(new BorderLayout());
        
        //patAddPane.selectPat(patPane.getMaxId());
        
        viewPanel.add("0", panel); 
        viewPanel.add("1", ambPane);
        viewPanel.add("2",patPane);
        viewPanel.add("3",patAddPane);
        //ambAddPane.selectAmb("A7");
        viewPanel.add("4",ambAddPane);
        
        add(new JLabel(""), BorderLayout.NORTH);
    
        add(viewPanel, BorderLayout.CENTER);
        setVisible(true);
	}

	public void paintComponent(Graphics g)
	{
		super.paintComponent(g);
	}
	
	public void changeLayout(String id) {
		card.show(viewPanel, id);
	}
}

