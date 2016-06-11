/** Build a simple grid of squares and animate a block moving
 * from side-to-side 
 * @author Philip Papadopoulos
 * @date Spring 2016
 * @version 1
 * @email ppapadopoulos@ucsd.edu
 */
import java.awt.*;
import java.util.ArrayList;
import javax.swing.*;
import java.io.IOException;
import java.lang.Thread;
import java.lang.Runnable;
import java.util.concurrent.TimeUnit;
import java.awt.event.*;


public class GameGrid {
    
    private static int width = 800;
    private static int height = 400;
    private static int pixels = 20;

    /** Usage statement
     */
    private static void usage()
    {
        /* all measurements are in pixels */
        System.out.format("Usage: GameGrid [ width height pixels ]\n");
        System.exit(-1);
    }
    /** Make this a java program
     * @param args standard command line arguments
     */
    public static void main(String[] args) 
    {
        
        if (args.length > 0 && args.length != 3) usage();
        try 
        {
        
            if ( args.length == 3 )
            {
                width = Integer.parseInt(args[0]);
                height = Integer.parseInt(args[1]);
                pixels = Integer.parseInt(args[2]);
            }
            if ( width <=0 || height <= 0 || pixels <=0 || pixels > height
                    || pixels > width ) usage();
        }
        catch (NumberFormatException e)
        {
            usage();
        }

        MyWindow window = new MyWindow(width,height,pixels);
        Grid grid = window.getGrid();
        int xmax = width/pixels;
        int ymax = height/pixels;

        /* Fill in the corner cells */
        grid.fillCell(0, 0);
        grid.fillCell(xmax-1, 0);
        grid.fillCell(0, ymax-1);
        grid.fillCell(xmax-1, ymax-1);

        /* Create a thread to move a cell side-to-side */
        Mover mvRect = new Mover(new Point(xmax/2,ymax/2), grid);

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

/** Graphics Window
 */
class MyWindow extends JFrame 
{
    private int width, height, pixels;
    private int xmax, ymax;
    private Grid grid;

    /** Constructor
     * @param w  width (logical number of cells)
     * @param h  height (logical number of cells)
     * @param p  pixels. Cells are p X p pixels
    */
    public MyWindow (int w, int h, int p)
    {
        super();
        width = w;
        height = h;
        pixels = p;
        xmax = width / pixels;
        ymax = height / pixels;
        grid = new Grid( xmax, ymax, pixels);
        Container pane = getContentPane();
        pane.setLayout(new BorderLayout());
        pane.add(grid,BorderLayout.CENTER);
        pack();
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setVisible(true);
    }
    /** reference to the logical grid of cells 
     * @return grid reference
     */
    public Grid getGrid()
    {
        return grid;
    }
}

/** Helper class that defines the graphics pane
*/
class Grid extends JPanel {
    private static final int PAD=2;
    private int width, height, pixels;
    private int xmax, ymax;
    private ArrayList<Point> fillCells;

    /** Constructor
     * @param w  width (logical number of cells)
     * @param h  height (logical number of cells)
     * @param p  pixels. Cells are p X p pixels
    */
    public Grid (int w, int h, int p) {
        width = w;
        height = h;
        pixels = p;
        fillCells = new ArrayList<Point>();
    }

    /** Return the logical dimensions of the grid 
     * @return logical dimension
     */
    public Dimension getLogicalSize()
    {
        return new Dimension(width,height);
    }

    /** Return the pixel dimensions of the grid, with 2 pixels of pad
     */
    @Override
    public Dimension getPreferredSize()
    {
        return new Dimension(width*pixels + PAD, height*pixels + PAD); 
    }

    /** how to repaint when requested to do so by the Dispatch Thread */
    @Override
    protected synchronized void paintComponent(Graphics g) {
        super.paintComponent(g);
        for (Point fillCell : fillCells) {
            int cellX = fillCell.x * pixels;
            int cellY = fillCell.y * pixels;
            g.setColor(Color.RED);
            g.fillRect(cellX, cellY, pixels, pixels);
        }

        g.setColor(Color.BLACK);
        g.drawRect(0, 0,  width*pixels, height*pixels);

        for (int i = 0; i < width*pixels; i += pixels) {
            g.drawLine(i, 0, i, height*pixels);
        }

        for (int i = 0; i < height*pixels; i += pixels) {
            g.drawLine(0, i, width*pixels, i);
        }
    }

    /** add a logical cell to be drawn on the grid
     * @param x  logical X coordinate
     * @param y  logical Y coordinate
     */ 
    public synchronized void fillCell(int x, int y) {
        fillCells.add(new Point(x, y));
        repaint();
    }

    /** Remove a logical cell from being drawn on the grid
     * @param x  logical X coordinate
     * @param y  logical Y coordinate
     */ 
    public synchronized void clearCell(int x, int y) {
        Point tst = new Point(x,y);
        for (Point p : fillCells)
        {
            if (p.equals(tst))
            {
                fillCells.remove(p);
                break;
            }
        }
        repaint();
    }
}

/** Class that moves a cell from side to side. Flip-book style animation
 */
class Mover implements Runnable
{
    
    private final int MILLISWAIT=150;
    int xMax;
    Point startPoint;
    boolean stop = false;
    Grid theGrid;
    /** Constructor
     * @param initial logical location of where to start
     * @param grid reference to logical grid
     */
    public Mover (Point initial, Grid grid)
    {
        xMax = (int) grid.getLogicalSize().getWidth() - 1; 
        startPoint = initial;
        theGrid = grid;
    }
    /** Implement Runnable 
    */
    public void run()
    {    
        int inc = 1;
        while (!stop)
        {
            /* place cell */
            theGrid.fillCell(startPoint.x, startPoint.y);
            /* wait */
             try { TimeUnit.MILLISECONDS.sleep(MILLISWAIT);}
                    catch (InterruptedException e){};
            if (startPoint.x == xMax)
                inc = -1;
            if (startPoint.x == 0)
                inc = 1;
            /* clear it */
            theGrid.clearCell(startPoint.x, startPoint.y);
            /* updaate coordinate for redraw on next iteration */
            startPoint.x += inc;
        }
    }
    /* how to tell the mover to stop what it is doing
    */
    public void stopMover()
    {
        stop = true;
    }
}
    

// vim: ts=4:sw=4:tw=78
