/*
 * Your name, UPI 
 *
 * Animation of shapes moving and changing.
 */

/*
 *    ======================================================================
 *    AnimationPanel.java : Moves shapes around on the screen according to different paths.
 *    It is the main drawing area where shapes are added and manipulated.
 *    It also contains a popup menu to clear all shapes.
 *    ======================================================================
 */

import javax.swing.*;
import java.awt.*;
import java.util.*;
import java.awt.event.*;

public class AnimationPanel extends JComponent implements Runnable {
    private Thread animationThread = null;    // the thread for animation
    private ArrayList<MovingShape> shapes;        // the ArrayList which stores a list of shapes
    private int currentXPos=10, currentYPos=20,
        currentShapeType=0, // the current shape type
        currentPath=0;        // the current path type
    private int currentHeight = 20, currentWidth = 50;
    private Color currentBorderColor = Color.black;  // the current border colour of a shape
    private Color currentFillColor = Color.blue;  // the current fill colour of a shape
    private int delay = 30;         // the current animation speed
    JPopupMenu popup;                // popup menu

     /** Constructor of the AnimationPanel
        */
    public AnimationPanel() {
        shapes = new ArrayList<MovingShape>(); //create the ArrayList to store shapes
        Insets insets = getInsets();
        int marginWidth = getWidth() - insets.left - insets.right;
        int marginHeight = getHeight() - insets.top - insets.bottom;
        popup = new JPopupMenu(); //create the popup menu
        makePopupMenu();
        // add the mouse event to handle popup menu
        addMouseListener( new MouseAdapter() {
            public void mousePressed(MouseEvent e) {
                maybeShowPopup(e);
            }

            public void mouseReleased(MouseEvent e) {
                maybeShowPopup(e);
            }

            private void maybeShowPopup(MouseEvent e) {
                if (e.isPopupTrigger()) {
                    popup.show(e.getComponent(), e.getX(), e.getY());
                }
            }
            public void mouseClicked( MouseEvent e ) {
                if (animationThread != null)   // if the animation has started, then
                    for (MovingShape currentShape: shapes)
                        if ( currentShape.contains( e.getPoint()) ) {  // if the mousepoint is within a shape, then set the shape to be selected/deselected
                            //System.out.println("selected");
                            currentShape.setSelected( ! currentShape.isSelected() );
                        }
            }
        });
    }

    /** create a new shape
     */
    protected void createNewShape() {
        // get the margin of the frame
        Insets insets = getInsets();
        int marginWidth = getWidth() - insets.left - insets.right;
        int marginHeight = getHeight() - insets.top - insets.bottom;
        // create a new shape dependent on all current properties and the mouse position
        switch (currentShapeType) {
            case 0: {
                //create a new rectangle - complete this
                MovingShape ms = new MovingRectangle(currentXPos, currentYPos, currentHeight, currentWidth, marginWidth, marginHeight, currentBorderColor, currentFillColor, currentPath);
                shapes.add(ms);
                break;
            }
            case 1: {
                MovingShape ms = new MovingSquare(currentXPos, currentYPos, currentWidth, currentWidth, marginWidth, marginHeight, currentBorderColor, currentFillColor, currentPath);
                shapes.add(ms);
                break;
            }
        }
    }

    /** 
     * @return list ArrayList<MovingShape>
     */
    public ArrayList<MovingShape> getAllShapes() {
        return shapes;
    }

    /** caculate total area of all shapes
     * @param shapeList list of shapes
     * @return int sum area
     */
    public static < T extends ArrayList<MovingShape> > int calculateArea(T shapeList) {
        int area = 0;

        for (int i = 0; i < shapeList.size(); i++) {
            area += shapeList.get(i).area();
        }

        return area;
    }

    /** set the current shape type
     * @param s    the new shape type
     */
    public void setCurrentShapeType(int s) {
        currentShapeType = s;
    }

    /** reset the margin size of all shapes in the ArrayList
     */
    public void resetMarginSize() {
        Insets insets = getInsets();
        int marginWidth = getWidth() - insets.left - insets.right;
        int marginHeight = getHeight() - insets.top - insets.bottom ;
        //complete this
        for (int i = 0; i < shapes.size(); i++) {
            shapes.get(i).setMarginSize(marginWidth, marginHeight);
        }
    }

    /** set the current path type and the path type for all currently selected shapes
     * @param t    the new path type
     */
    public void setCurrentPathType(int t) {
        currentPath = t;
        //complete this
        for (int i = 0; i < shapes.size(); i++) {
            if (shapes.get(i).isSelected() && 
                  ((currentShapeType == 0 && shapes.get(i) instanceof MovingRectangle)
                   ||(currentShapeType == 1 && shapes.get(i) instanceof MovingSquare))) {
                shapes.get(i).setPath(t);
            }
        }
    }

    /** set the current x and the x for all currently selected shapes
     * @param s    the new x value
     */
    public void setCurrentXPos(int x) {
        currentXPos = x;
        //complete this
        for (int i = 0; i < shapes.size(); i++) {
            if (shapes.get(i).isSelected() &&
                  ((currentShapeType == 0 && shapes.get(i) instanceof MovingRectangle)
                   ||(currentShapeType == 1 && shapes.get(i) instanceof MovingSquare))) {
                shapes.get(i).setX(x);
            }
        }

    }

    /** set the current y and the y for all currently selected shapes
     * @param y    the new y value
     */
    public void setCurrentYPos(int y) {
        currentYPos = y;
        //complete this
        for (int i = 0; i < shapes.size(); i++) {
            if (shapes.get(i).isSelected() &&
                  ((currentShapeType == 0 && shapes.get(i) instanceof MovingRectangle)
                   ||(currentShapeType == 1 && shapes.get(i) instanceof MovingSquare))) {
                shapes.get(i).setY(y);
            }
        }
    }

    /** set the current height and the height for all currently selected shapes
     * @param height    the new height value
     */
    public void setCurrentHeight(int height) {
        currentHeight = height;
        //complete this
        for (int i = 0; i < shapes.size(); i++) {
            if (shapes.get(i).isSelected() &&
                  ((currentShapeType == 0 && shapes.get(i) instanceof MovingRectangle)
                   ||(currentShapeType == 1 && shapes.get(i) instanceof MovingSquare))) {
                shapes.get(i).setHeight(height);
            }
        }
    }

    /** set the current width and the width for all currently selected shapes
     * @param width    the new width value
     */
    public void setCurrentWidth(int width) {
        currentWidth = width;
        //complete this
        for (int i = 0; i < shapes.size(); i++) {
            if (shapes.get(i).isSelected() &&
                  ((currentShapeType == 0 && shapes.get(i) instanceof MovingRectangle)
                   ||(currentShapeType == 1 && shapes.get(i) instanceof MovingSquare))) {
                shapes.get(i).setWidth(width);
            }
        }
    }

    /** set the current border colour and the border colour for all currently selected shapes
     * @param bc    the new border colour value
     */
    public void setCurrentBorderColor(Color bc) {
        currentBorderColor = bc;
        //complete this
        for (int i = 0; i < shapes.size(); i++) {
            if (shapes.get(i).isSelected() &&
                  ((currentShapeType == 0 && shapes.get(i) instanceof MovingRectangle)
                   ||(currentShapeType == 1 && shapes.get(i) instanceof MovingSquare))) {
                shapes.get(i).setBorderColor(bc);
            }
        }
    }

    /** set the current fill colour and the border colour for all currently selected shapes
     * @param bc    the new fill colour value
     */
    public void setCurrentFillColor(Color fc) {
        currentFillColor = fc;
        //complete this
        for (int i = 0; i < shapes.size(); i++) {
            if (shapes.get(i).isSelected() &&
                  ((currentShapeType == 0 && shapes.get(i) instanceof MovingRectangle)
                   ||(currentShapeType == 1 && shapes.get(i) instanceof MovingSquare))) {
                shapes.get(i).setFillColor(fc);
            }
        }
    }

    /** get the current x position in the top left corner
     * @return x - the x value
     */
    public int getCurrentXPos() {
        return currentXPos;
    }

    /** get the current y position in the top left corner
     * @return y - the y value
     */
    public int getCurrentYPos() {
        return currentYPos;
    }

    /** get the current height
     * @return currentHeight
     */
    public int getCurrentHeight() {
        return currentHeight;
    }

    /** get the current width
     * @return currentWidth
     */
    public int getCurrentWidth() {
        return currentWidth;
    }

    /** get the current fill colour
     * @return currentFillColor - the fill colour value
     */
    public Color getCurrentFillColor() {
        return currentFillColor;
    }

    /** get the current border colour
     * @return currentBorderColor - the border colour value
     */
    public Color getCurrentBorderColor() {
        return currentBorderColor;
    }

   // you don't need to make any changes after this line ______________

    /** remove all shapes from the ArrayList
     */
    public void clearAllShapes() {
        shapes.clear();
    }

    /**    update the painting area
     * @param g    the graphics control
     */
    public void update(Graphics g){
        paint(g);
    }

    /**    move and paint all shapes within the animation area
     * @param g    the Graphics control
     */
    public void paintComponent(Graphics g) {
        for (MovingShape currentShape: shapes) {
            currentShape.move();
            currentShape.draw(g);
        }
    }

    /** create the popup menu for our animation program
     */
    protected void makePopupMenu() {
        JMenuItem menuItem;
     // clear all
        menuItem = new JMenuItem("Clear All");
        menuItem.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                clearAllShapes();
            }
        });
        popup.add(menuItem);
     }

    /** change the speed of the animation
     * @param newValue     the speed of the animation in ms
     */
    public void adjustSpeed(int newValue) {
        if (animationThread != null) {
            stop();
            delay = newValue;
            start();
        }
    }

    /**    When the "start" button is pressed, start the thread
     */
    public void start() {
        animationThread = new Thread(this);
        animationThread.start();
    }

    /**    When the "stop" button is pressed, stop the thread
     */
    public void stop() {
        if (animationThread != null) {
            animationThread = null;
        }
    }

    /** run the animation
     */
    public void run() {
        Thread myThread = Thread.currentThread();
        while(animationThread==myThread) {
            repaint();
            pause(delay);
        }
    }

    /** Sleep for the specified amount of time
     */
    private void pause(int milliseconds) {
        try {
            Thread.sleep((long)milliseconds);
        } catch(InterruptedException ie) {}
    }
}
