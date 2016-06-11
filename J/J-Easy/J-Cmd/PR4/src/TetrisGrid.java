/** Build a Tetris grid board
 * @author x 
 * @date Spring 2016
 * @version 1
 * @email x@ucsd.edu
 */

import java.util.List;
import java.util.ArrayList;

public class TetrisGrid {

    private static final int EMPTY = 0;
    private static final int OCCUPIED = 1;
    private static final int TEMPOCCUPIED = 2;

    private int width;
    private int height;
    private Integer[][] boardgrid; /* 0: empty, 
                                  1: occupied from previous pieces
                                  2: occupied from current pieces */ 
    private TetrisShape curShape;
    private int speedTimer;

    private int nowScore;
    private int highScore;

    private boolean runState;
    
    /** Constructor
     * @param w width - coordinate
     * @param h height - coordinate
     */
    public TetrisGrid(int w, int h)
    {
        width = w;
        height = h;

        boardgrid = new Integer[height][width];

        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++) {
                boardgrid[i][j] = new Integer(EMPTY);
            }

        curShape = null;
        nowScore = highScore = 0;
        speedTimer = 1;
        runState = false;
    }

    /** new Game start
     */
    public void newStartGame()
    {
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++) {
                boardgrid[i][j] = EMPTY;
            }

        nowScore = 0;
        runState = true;
    }

    /** reset game
     */
    public void resetGame()
    {
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++) {
                boardgrid[i][j] = EMPTY;
            }

        nowScore = 0;
        highScore = 0;
        curShape = null;
        speedTimer = 1;

        runState = false;
    }


    /** Get blocks that are occupied
     * @return blocked List<Coord> of occupied 
     */
    public ArrayList<Coord> getBlockedGrids()
    {
        ArrayList<Coord> blocked = new ArrayList<Coord>();

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (boardgrid[i][j] == OCCUPIED ||
                        boardgrid[i][j] == TEMPOCCUPIED) {
                    blocked.add(new Coord(i, j));
                }
            }
        }

        return blocked;
    }

    /** shape intersect any occupied block in the grid
     * @param shape given Shape
     * @param roffset row position
     * @param coffset col position
     * @return boolean intersect or not
     */
    public boolean shapeIntersect(TetrisShape shape, int roff, int coff)
    {
        Coord[] shapecord = shape.getOrigCoords();

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                for (int k = 0; k < shapecord.length; k++) {
                    if (boardgrid[i][j] != EMPTY
                            && shapecord[k].getRow()+roff == i 
                            && shapecord[k].getCol()+coff == j)
                        return true;
                }
            }
        }

        return false;
    }

    /** shape is completely in bounds
     * @param shape given shape
     * @param roffset row position
     * @param coffset col position
     * @return boolean in bounds or not
     */
    public boolean inBounds(TetrisShape shape, int roffset, int coffset)
    {
        Coord[] shapecord = shape.getOrigCoords();

        for (int k = 0; k < shapecord.length; k++) {
            if (shapecord[k].getRow()+roffset < 0 
                    || shapecord[k].getRow()+roffset >= height
                    || shapecord[k].getCol()+coffset < 0
                    || shapecord[k].getCol()+coffset >= width)
                return false;
        }

        return true;
    }

    /** add shape to grid
     * @param shape given shape
     * @param roffset row-offset
     * @param coffset col-offset
     */
    public synchronized void addShape(TetrisShape shape, int roffset, int coffset)
    {
        curShape = shape;
        Coord[] shapecord = shape.getOrigCoords();

        for (int k = 0; k < shapecord.length; k++) {
            boardgrid[shapecord[k].getRow()+roffset]
                [shapecord[k].getCol()+coffset] = TEMPOCCUPIED;
        }
    }

    /** remove shape from grid
     * @param shape given shape
     * @param roffset row-offset
     * @param coffset col-offset
     */
    public synchronized void removeShape(TetrisShape shape, int roffset, int coffset)
    {
        Coord[] shapecord = shape.getOrigCoords();

        for (int k = 0; k < shapecord.length; k++) {
            boardgrid[shapecord[k].getRow()+roffset]
                [shapecord[k].getCol()+coffset] = EMPTY;
        }
        curShape = null;
    }

    /** return the current shape in grid
     * @return TetrisShape curShape
     */
    public TetrisShape getCurrentShape()
    {
        return curShape;
    }

    /** judge shape hit bottom line

    /** rows have been completed or not
     * @return int 1. has completed rows return row number
     *                2. else return -1
     */
    public int hasCompletedRows()
    {
        int completed = 0;

        for (int i = height-1; i >= 0; i--) {
            completed = 1;
            for (int j = 0; j < width; j++) {
                if (boardgrid[i][j] == EMPTY) completed = 0;
            }

            if (completed == 1) return i;
        }

        return -1;
    }

    /** delete the most bottom completed rows
     * @param rowno to delete row number
     */
    public void deleteOneCompletedRow(int rowno)
    {
        for (int i = rowno; i > 0; i--) {
            for (int j = 0; j < width; j++) {
                boardgrid[i][j] = boardgrid[i-1][j];
            }
        }
    }

    /** getter
     * @return int timer speed
     */
    public int getSpeedTimer()
    {
        return speedTimer;
    }

    /** setter
     * @param timerSpeed set the time slice
     */
    public void setSpeedTimer(int timerSpeed)
    {
        speedTimer = timerSpeed;
    }

    /** getter
     * @return int width
     */
    public int getWidth() 
    {
        return width;
    }

    /** getter
     * @return int height
     */
    public int getHeight()
    {
        return height;
    }

    /** setter
     * @param w width
     */
    public void setWidth(int w)
    {
        width = w;
    }

    /** setter
     * @param h height
     */
    public void setHeight(int h)
    {
        height = h;
    }

    /** setter
     * @param score acquired score
     */
    public void addNowScore(int sc)
    {
        nowScore += sc;
    }

    /**
     * getter nowScore
     * @return int nowscore
     */
    public int getNowScore()
    {
        return nowScore;
    }

    /**
     * setter
     * @param sc set max score
     */
    public void setHighScore(int hs)
    {
        highScore = hs;
    }

    /**
     * getter 
     * @return int high score
     */
    public int getHighScore()
    {
        return highScore;
    }


    /** getter
     * @return Boolean runState
     */
    public boolean getRunState()
    {
        return runState;
    }

    /** setter
     * @param state boolean
     */
    public void setRunState(boolean state)
    {
        runState = state;
    }

    /** tostring
     * @return String litaral
     */
    @Override
    public String toString()
    {
        String board = "Grid:(" + height + "," + width + ")\n";

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                board += boardgrid[i][j].toString();
            }
            board += "\n";
        }

        return board;
    }
}
