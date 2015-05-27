
package ajpbasicmotion;

import ajpbasicmotion.utils.*;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

/**
 *
 * @author s
 */
class MainPanel extends JPanel
{
	ClassMap options;         // maps sub-program names
	JComboBox frameChoice;    // the drop down GUI component
	String descr =            // textual description
		"some example programs to demonstrate simple animated graphics...\n"+
		"(1) using constructors & paint loops & (2) using XOR mode       \n\n"+

		"mostly these examples do not give us the results we would like,     \n"+
		"the graphics mess up & there are unwanted delays in opening/closing \n"+
		"examine the code & try to explain what is happening & why.          \n"+
		"correct it if you can \n\n"+
		
		"The BasicMotion 'wrapper' uses some easy stuff from the Java    \n"+
		"reflections pack. Check this out.";

    public MainPanel( Container holder )
    {
	    options = new ClassMap();

		//____ links to subpanels ___________
	    options.put( "GraphicInConstructor",   "ajpbasicmotion.pt1.GraphicInConstructor" );
	    options.put( "GraphicInPaintLoop1",    "ajpbasicmotion.pt1.GraphicInPaintLoop1" );
	    options.put( "GraphicInPaintLoop2",    "ajpbasicmotion.pt1.GraphicInPaintLoop2" );
		options.put( "GraphicInPaintLoop3",    "ajpbasicmotion.pt1.GraphicInPaintLoop3" );
		options.put( "GraphicViaRepaintCalls", "ajpbasicmotion.pt1.GraphicViaRepaintCalls" );
		options.put( "RedrawInBusyPaintLoop",  "ajpbasicmotion.pt1.RedrawInBusyPaintLoop" );
		options.put( "UsingXORsimple",         "ajpbasicmotion.xor.UsingXORsimple" );
		options.put( "UsingXORsimpleSmooth",   "ajpbasicmotion.xor.UsingXORsimpleSmooth" );
		options.put( "UsingXORmultiBall_1",    "ajpbasicmotion.xor.UsingXORmultiBall_1");
		options.put( "UsingXORmultiBall_2",    "ajpbasicmotion.xor.UsingXORmultiBall_2");
		options.put( "UsingXORmultiBall_3",    "ajpbasicmotion.xor.UsingXORmultiBall_3");
		//-----------------------------------

	    frameChoice = new JComboBox(options.keySet().toArray());
	    frameChoice.setSelectedIndex(0);		// 1st one is default

		Button runBtn = new Button( "run" );
	    runBtn.addActionListener( new ActionListener()
	    {    public void actionPerformed( ActionEvent e )
		    {	String choice = (String)frameChoice.getSelectedItem();
		    	Object win = options.getAsInstance( choice );
				// assume win is a JFrame or JPanel
				// if win is a JFrame this is enough, if its a JPanel then wrap it
				if (win instanceof JPanel)
					new PanelWrapper( (JPanel)win );
			}
	    });

		// editor pane for text display
		JEditorPane text = new JEditorPane();
		text.setText( descr );
		text.setEditable( false );

		// GUI layout
		setLayout( new BorderLayout() );
		add( text, BorderLayout.CENTER );
		JPanel subPane = new JPanel();
		add( subPane, BorderLayout.SOUTH );
		subPane.add( frameChoice );
		subPane.add( runBtn );
    }
}

public class BasicMotion
{
    public static void main(String []args)
    {   // give title for title bar of window
        JFrame frame = new JFrame("AJP BasicMotion");
        MainPanel panel = new MainPanel( frame );
        frame.add(panel);

        // exit the program when window is closed
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        frame.setSize( 500, 300 ); // suggest width and height of frame in pixels
        frame.setVisible(true);
    }
}


