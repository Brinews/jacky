/**  animate block moving
 * @author x
 * @date Spring 2016
 * @version 1
 * @email x@ucsd.edu
 */
import java.awt.*;
import java.lang.Thread;
import java.lang.Runnable;
import java.util.concurrent.TimeUnit;
import java.awt.event.*;
import java.util.Random;



/** Class that moves tetris. Flip-book style animation
 */
public class ShapeMover implements Runnable
{
    private final int MILLISWAIT=150;
    int yMax;
    Coord startCoord;
    Coord initCoord;
    boolean stop = false;
    GraphicsGrid gdGrid;
    TetrisGrid theGrid;
    TetrisShape curShape = null;
    TetrisWindow win = null;

    /** Constructor
     * @param initial logical location of where to start
     * @param grid reference to logical grid
     */
    public ShapeMover (Coord initial, TetrisGrid grid, GraphicsGrid gd,
            TetrisWindow window)
    {
        yMax = (int) grid.getHeight() - 1; 
        initCoord = new Coord(initial.getRow(), initial.getCol());
        startCoord = initial;
        theGrid = grid;
        gdGrid = gd;
        win = window;
    }

    /** Implement Runnable 
    */
    public void run()
    {
        int inc = 1;
	//theGrid.setRunState(true);

        while (true)  {
            try { TimeUnit.MILLISECONDS.sleep(1);}
            catch (InterruptedException e){};

            if (!theGrid.getRunState()) { 
                curShape = null;
                continue;
            }

            if (curShape == null) {
                startCoord.setRow(initCoord.getRow());
                startCoord.setCol(initCoord.getCol());
                curShape = TetrisShape.randShape();

                /* game over */
                if (theGrid.shapeIntersect(curShape, startCoord.getRow(),
                            startCoord.getCol())) {
                    theGrid.setRunState(false);
                    if (theGrid.getNowScore() > theGrid.getHighScore()) {
                        theGrid.setHighScore(theGrid.getNowScore());
                        win.setScoreText(theGrid.getNowScore(), 
                                theGrid.getHighScore());
                    }
                    win.setGameOverText("             GAME OVER!");
                    curShape = null;
                    continue;
                }
            }

            /* place shape */
            if (theGrid.inBounds(curShape, 
                        startCoord.getRow(), startCoord.getCol())) {
                theGrid.addShape(curShape, 
                        startCoord.getRow(), startCoord.getCol());
                gdGrid.repaint();
            }

            /* wait */
            try { TimeUnit.MILLISECONDS.sleep(1000/theGrid.getSpeedTimer());}
            catch (InterruptedException e){};

            /* clear it */
            if (theGrid.inBounds(curShape, 
                        startCoord.getRow(), startCoord.getCol())) {
                theGrid.removeShape(curShape, 
                        startCoord.getRow(), startCoord.getCol());
                gdGrid.repaint();
            }

            /* updaate coordinate for redraw on next iteration */
            startCoord.setRow(startCoord.getRow() + inc);

            /* tetris stop and stay in grid */
            if (!theGrid.inBounds(curShape, startCoord.getRow(),
                       startCoord.getCol())    
               || theGrid.shapeIntersect(curShape, startCoord.getRow(),
                        startCoord.getCol())) {
                theGrid.addShape(curShape, 
                        startCoord.getRow()-inc, startCoord.getCol());

                /* judge the complete row */
                int deleteRow = -1;
                int acc = 0;
                while ((deleteRow = theGrid.hasCompletedRows()) != -1) {
                    theGrid.deleteOneCompletedRow(deleteRow);
                    acc++;
                }

                /* score update */
                if (acc == 1) {
                    theGrid.addNowScore(100);
                } else if (acc > 1) {
                    theGrid.addNowScore(100*(1<<acc));
                }

                /* speed update */
                if (theGrid.getNowScore() > 0 
                        && theGrid.getNowScore() < 40000) {
                    theGrid.setSpeedTimer(1+theGrid.getNowScore()/2000);
                }

                gdGrid.repaint();
                win.setScoreText(theGrid.getNowScore(), theGrid.getHighScore());

                curShape = null;
            } else if (theGrid.getRunState()) {
                theGrid.addNowScore(10);
                win.setScoreText(theGrid.getNowScore(), theGrid.getHighScore());
            }    

            //System.out.println("NowScore:"+theGrid.getNowScore());
            //System.out.println("HighScore:"+theGrid.getHighScore());
            
        }
    }

    /* how to tell the mover to stop what it is doing
    */
    public void stopMover()
    {
        stop = true;
    }
}

