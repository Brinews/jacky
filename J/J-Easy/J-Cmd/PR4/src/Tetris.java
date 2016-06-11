/** Main entry of tetris game
 * @author x
 * @date Spring 2016
 * @version 1
 * @email x@ucsd.edu
 */
import java.awt.*;
import java.awt.event.*;
import java.util.ArrayList;
import javax.swing.*;
import javax.swing.event.*;
import java.io.IOException;

public class Tetris {
    
    private static int width = 10;
    private static int height = 20;
    private static int pixels = 20;

    /** Usage statement
     */
    private static void usage()
    {
        /* all measurements are in pixels */
        System.out.format("Usage: Tetris [blocksize]\n");
        System.exit(-1);
    }

    /** Make this a java program
     * @param args standard command line arguments
     */
    public static void main(String[] args) 
    {
        if (args.length > 0 && args.length != 1) usage();

        try {
            if ( args.length == 1 ) {
                pixels = Integer.parseInt(args[0]);
            }
            if ( pixels <=0 ) usage();
        } catch (NumberFormatException e) {
            usage();
        }

        Coord startPos = new Coord(2, width/2);

        TetrisGrid shapegrid = new TetrisGrid(width,height);
        TetrisWindow window = new TetrisWindow(width,height,pixels,
                shapegrid,startPos);
        GraphicsGrid gdGrid = window.getGraphicsGrid();

        /* Create a thread to move tetris */
        ShapeMover mvRect = new ShapeMover(startPos, shapegrid, gdGrid, window);
        Thread t = new Thread(mvRect);
        t.start();    

        try 
        {
            System.out.format("Hit Return to exit program");
            System.in.read();
            mvRect.stopMover();
        }
        catch (IOException e){}

        window.dispatchEvent(new WindowEvent(window, 
            WindowEvent.WINDOW_CLOSING));

        window.dispose();    
    }
}

// vim: ts=4:sw=4:tw=78
