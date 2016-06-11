/** Build a simple grid of squares and animate block moving
 * @author x
 * @date Spring 2016
 * @version 1
 * @email x@ucsd.edu
 */
import java.awt.*;
import java.util.ArrayList;
import javax.swing.*;
import javax.swing.event.*;
import java.awt.event.*;

/** Helper class that defines the graphics pane
*/
public class GraphicsGrid extends JPanel {
    private static final int PAD=2;
    private int width, height, pixels;
    private int xmax, ymax;
    private ArrayList<Coord> fillCells;

    private TetrisGrid theGrid;
    private Coord startCoord;


    /** Constructor
     * @param w  width (logical number of cells)
     * @param h  height (logical number of cells)
     * @param p  pixels. Cells are p X p pixels
    */
    public GraphicsGrid (int w, int h, int p, TetrisGrid tg, Coord pos) {
        width = w;
        height = h;
        pixels = p;
        theGrid = tg;
        fillCells = tg.getBlockedGrids();
        startCoord = pos;

        KeyHandler listener = new KeyHandler();
        addKeyListener(listener);

        setFocusable(true);
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

        fillCells = theGrid.getBlockedGrids();

        for (Coord fillCell : fillCells) {
            int cellX = fillCell.getCol() * pixels;
            int cellY = fillCell.getRow() * pixels;
            g.setColor(Color.BLUE);
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
        fillCells.add(new Coord(x, y));
        repaint();
    }

    /** Remove a logical cell from being drawn on the grid
     * @param x  logical X coordinate
     * @param y  logical Y coordinate
     */ 
    public synchronized void clearCell(int x, int y) {
        Coord tst = new Coord(x,y);
        for (Coord p : fillCells)
        {
            if (p.equals(tst))
            {
                fillCells.remove(p);
                break;
            }
        }
        repaint();
    }

    private class KeyHandler implements KeyListener {

        /** reaction to user's input
         * overide function
         */
        @Override
        public void keyPressed(KeyEvent event)
        {
            int keyCode = event.getKeyCode();
            TetrisShape curShape;

            curShape = theGrid.getCurrentShape();

            if (keyCode == KeyEvent.VK_H) {
                /* left move */
                if (startCoord.getCol() > 0 && curShape != null
                        && theGrid.inBounds(curShape, 
                            startCoord.getRow(), startCoord.getCol()-1)) {

                    theGrid.removeShape(curShape, 
                            startCoord.getRow(), startCoord.getCol());

                    if (!theGrid.shapeIntersect(curShape, startCoord.getRow(),
                                startCoord.getCol()-1))
                        startCoord.setCol(startCoord.getCol()-1);

                    theGrid.addShape(curShape, 
                            startCoord.getRow(), startCoord.getCol());

                    repaint();
                }
            } else if (keyCode == KeyEvent.VK_L) {
                /* right move */
                if (startCoord.getCol() < theGrid.getWidth()-1 && curShape != null
                        && theGrid.inBounds(curShape, 
                            startCoord.getRow(), startCoord.getCol()+1)) {

                    theGrid.removeShape(curShape, 
                            startCoord.getRow(), startCoord.getCol());

                    if (!theGrid.shapeIntersect(curShape, startCoord.getRow(),
                                startCoord.getCol()+1))
                        startCoord.setCol(startCoord.getCol()+1);

                    theGrid.addShape(curShape, 
                            startCoord.getRow(), startCoord.getCol());

                    repaint();
                }
            } else if (keyCode == KeyEvent.VK_J) {
                /* counter clockwise */
                if (curShape != null) {
                    theGrid.removeShape(curShape, 
                            startCoord.getRow(), startCoord.getCol());
                    curShape.rotateCounterClockwise();
                    if (!theGrid.inBounds(curShape, startCoord.getRow(),
                            startCoord.getCol())
                        || theGrid.shapeIntersect(curShape, startCoord.getRow(),
                            startCoord.getCol())) {
                        curShape.rotateClockwise();
                    }
                    theGrid.addShape(curShape, 
                            startCoord.getRow(), startCoord.getCol());
                }
            } else if (keyCode == KeyEvent.VK_K) {
                /* clockwise */
                if (curShape != null) {
                    theGrid.removeShape(curShape, 
                            startCoord.getRow(), startCoord.getCol());
                    curShape.rotateClockwise();
                    if (!theGrid.inBounds(curShape, startCoord.getRow(),
                                startCoord.getCol()) ||
                        theGrid.shapeIntersect(curShape, startCoord.getRow(),
                            startCoord.getCol())) {
                        curShape.rotateCounterClockwise();
                    }
                    theGrid.addShape(curShape, 
                            startCoord.getRow(), startCoord.getCol());
                }
            } else if (keyCode == KeyEvent.VK_SPACE) {
                if (curShape != null) {
                    while (true) {
                        theGrid.removeShape(curShape, 
                                startCoord.getRow(), startCoord.getCol());

                        startCoord.setRow(startCoord.getRow()+1);

                        if (!theGrid.inBounds(curShape, startCoord.getRow(),
                                    startCoord.getCol())
                            || theGrid.shapeIntersect(curShape, startCoord.getRow(),
                                    startCoord.getCol())) {
                            startCoord.setRow(startCoord.getRow()-1);
                            break;
                        }

                        theGrid.addShape(curShape, 
                                startCoord.getRow(), startCoord.getCol());
                    }
                }
            }
        }

        @Override
        public void keyReleased(KeyEvent event)
        {
        }

        @Override
        public void keyTyped(KeyEvent event)
        {
        }
    }
}

/** Graphics Window
 */
class TetrisWindow extends JFrame 
{
    private int width, height, pixels;
    private GraphicsGrid grid;

    private JLabel nowScoreLabel;
    private JLabel highScoreLabel;

    private JButton newGameButton;
    private JButton resetButton;

    private JSlider speedSlider;
    private ChangeListener chgListener;
    private ActionListener clkListener;

    private TetrisGrid theGrid;
    private Coord startCoord;

    /** Constructor
     * @param w  width (logical number of cells)
     * @param h  height (logical number of cells)
     * @param p  pixels. Cells are p X p pixels
     */
    public TetrisWindow (int w, int h, int p, TetrisGrid sg, Coord startPos)
    {
        super();
        width = w;
        height = h;
        pixels = p;
        theGrid = sg;
        grid = new GraphicsGrid(width, height, pixels, sg, startPos);

        startCoord = startPos;
        
        JPanel topPanel = new JPanel();
        topPanel.setLayout(new GridLayout(2, 1));

        nowScoreLabel = new JLabel("Score:             ");
        highScoreLabel = new JLabel("High Score:        ");
        topPanel.add(nowScoreLabel);
        topPanel.add(highScoreLabel);

        Container pane = getContentPane();
        pane.setLayout(new BorderLayout());

        JPanel botPanel = new JPanel();
        newGameButton = new JButton("New Game");
        resetButton = new JButton("Reset!");
        speedSlider = new JSlider(1, 20, 1);

        chgListener = new ChangeListener() {
            public void stateChanged(ChangeEvent event) {
                JSlider source = (JSlider) event.getSource();
                theGrid.setSpeedTimer(source.getValue());
            }
        };

        speedSlider.addChangeListener(chgListener);

        clkListener = new ActionListener() {
            public void actionPerformed(ActionEvent event) {
                JButton src = (JButton) event.getSource();
                if (src == newGameButton) {
                    theGrid.newStartGame();
                    startCoord.setRow(2);
                    startCoord.setCol(width/2);
                    grid.repaint();
                } else if (src == resetButton) {
                    theGrid.resetGame();
                    startCoord.setRow(2);
                    startCoord.setCol(width/2);
                    speedSlider.setValue(1);
                    setScoreText(0,0);
                    grid.repaint();
                }
            }
        };

        newGameButton.addActionListener(clkListener);
        resetButton.addActionListener(clkListener);

        newGameButton.setFocusable(false);
        resetButton.setFocusable(false);
        speedSlider.setFocusable(false);

        botPanel.add(newGameButton);
        botPanel.add(resetButton);
        botPanel.add(speedSlider);

        JPanel eastPanel = new JPanel();
        eastPanel.add(new JLabel("                           "));

        pane.add(topPanel, BorderLayout.NORTH);
        pane.add(eastPanel, BorderLayout.WEST);
        pane.add(grid,BorderLayout.CENTER);
        pane.add(botPanel, BorderLayout.SOUTH);
        //pane.add(newGameButton, BorderLayout.SOUTH);

        pack();
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setVisible(true);
    }

    /** reference to the logical grid of cells 
     * @return grid reference
     */
    public GraphicsGrid getGraphicsGrid()
    {
        return grid;
    }

    public void setScoreText(int nowscore, int highscore)
    {
        nowScoreLabel.setText("Score:              "+nowscore);
        highScoreLabel.setText("High Score:       "+highscore);
    }

    public void setGameOverText(String str)
    {
        nowScoreLabel.setText(nowScoreLabel.getText()+str);
    }
}

// vim: ts=4:sw=4:tw=78
