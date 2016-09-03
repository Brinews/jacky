/*
 * Your name, UPI 
 *
 * Shape data model, use dynamic binding
 */

/*
 *    ===============================================================================
 *    MovingShape.java : The superclass of all shapes.
 *    A shape has a top-left corner (x,y).
 *    A shape defines various properties, including selected, fill colour, border color, width and height.
 *    ===============================================================================
 */

import java.awt.*;
import java.awt.geom.*;

public abstract class MovingShape {

    public int marginWidth, marginHeight; // the margin of the animation panel area
    protected int x, y;                 // the top left coner of shapes
    protected MovingPath path;            // the moving path of shapes
    protected Color borderColor, fillColor;   // the border colour and fill colour of shapes
    protected boolean selected = false;    // draw handles if selected
    protected int shapeHeight, shapeWidth; // height and width properties of shape(stage 1.A)

    /** constuctor to create a shape with default values
     */
    public MovingShape(int sh, int sw) {
        this(10, 20, sh, sw, 800, 500, Color.black, Color.blue, 0); // the default properties
    }

    /** constuctor to create a shape
     * @param x         the x-coordinate of the new shape
     * @param y        the y-coordinate of the new shape
     * @param mw         the margin width of the animation panel
     * @param mh        the margin height of the animation panel
     * @param c        the colour of the new shape
     * @param typeOfPath         the path of the new shape
     */
    public MovingShape(int x, int y, int sh, int sw, int mw, int mh, Color c, Color fc, int pathType) {
        this.x = x;
        this.y = y;
        // stage 1.A
        this.shapeHeight = sh;
        this.shapeWidth = sw;

        marginWidth = mw;
        marginHeight = mh;
        borderColor = c;
        fillColor = fc;
        setPath (pathType);
    }

    /** Return the x-coordinate of the shape.
     * @return the x coordinate
     */
    public int getX() { return x; }

    /** Set the x-coordinate of the shape.
     * @param x     the x value
     */
    public void setX(int x) { this.x = x; }

    /** Return the y-coordinate of the shape.
     * @return the y coordinate
     */
    public int getY() { return y;}

    /** Set the y-coordinate of the shape.
     * @param y     the y value
     */
    public void setY(int y) { this.y = y; }

    /* stage 1.A */

    /** Return the height of the shape.
     * @return the height
     */
    public int getHeight() { return shapeHeight;}

    /** Set the height of the shape.
     * @param sh     the height value
     */
    public void setHeight(int sh) { this.shapeHeight = sh; }

    /** Return the height of the shape.
     * @return the height
     */
    public int getWidth() { return shapeWidth;}

    /** Set the width of the shape.
     * @param sw     the width value
     */
    public void setWidth(int sw) { this.shapeWidth = sw; }

    /** Return the selected property of the shape.
     * @return the selected property
     */
    public boolean isSelected() { return selected; }

    /** Set the selected property of the shape.
     *    When the shape is selected, its handles are shown.
     * @param s     the selected value
     */
    public void setSelected(boolean s) { selected = s; }

    /** Return the border colour of the shape.
     * @return the border colour
     */
    public Color getBorderColor() { return borderColor;}

    /** Set the border colour of the shape.
     * @param c     the border colour
     */
    public void setBorderColor(Color c) { borderColor = c; }

    /** Return the fill colour of the shape.
     * @return the fill colour
     */
    public Color getFillColor() { return fillColor;}

    /** Set the fill colour of the shape.
     * @param fc     the fill colour
     */
    public void setFillColor(Color fc) { fillColor = fc; }

    /**
     * Return a string representation of the shape, containing
     * the String representation of each element.
     */
    public String toString() {
        return "[" + this.getClass().getName() + "," + x + "," + y + ","+ shapeHeight + "," + shapeWidth + "]";
    }

   /** Draw the handles of the shape
     * @param g     the Graphics control
     */
    public void drawHandles(Graphics g) {
        if (isSelected()) {
            //complete this stage 1.A
            int handleSize = 4;
            g.setColor(Color.BLACK);
            Graphics2D g2 = (Graphics2D) g;
            Rectangle2D rect = new Rectangle2D.Double(x, y, handleSize, handleSize);
            g2.draw(rect);
            
            rect = new Rectangle2D.Double(x, y+shapeHeight-handleSize, handleSize, handleSize);
            g2.draw(rect);

            rect = new Rectangle2D.Double(x+shapeWidth-handleSize, y, handleSize, handleSize);
            g2.draw(rect);

            rect = new Rectangle2D.Double(x+shapeWidth-handleSize, y+shapeHeight-handleSize, handleSize, handleSize);
            g2.draw(rect);
        }
    }

    /** Reset the margin for the shape
     * @param w     the margin width
     * @param h     the margin height
     */
    public void setMarginSize(int w, int h) {
        marginWidth = w;
        marginHeight = h;
    }

    /** abstract contains method
     * Returns whether the point p is inside the shape or not.
     * @param p    the mouse point
     */
    public abstract boolean contains(Point p);

    /** abstract draw method
     * draw the shape
     * @param g     the Graphics control
     */
    public abstract void draw(Graphics g);

    /** abstract area method
     * calculate the area of shape
     * @return int     the area of shape
     */
    public abstract int area();

    /** Set the path of the shape.
     * @param pathID     the integer value of the path
     *    MovingPath.FALLING is the falling path
     */
    public void setPath(int pathID) {
        switch (pathID) {
            case MovingPath.FALLING : {
                path = new FallingPath();
                break;
            }
            case MovingPath.BOUNCING : {
                path = new BouncingPath();
                break;
            }
        }
    }

    /** move the shape by the path
     */
    public void move() {
        path.move();
    }

    // Inner class ===================================================================== Inner class
    /*
     *    ===============================================================================
     *    MovingPath : The superclass of all paths. It is an inner class.
     *    A path can change the current position of the shape.
     *    ===============================================================================
     */

    public abstract class MovingPath {
        public static final int FALLING = 0; // The Id of the moving path
        public static final int BOUNCING = 1; // The Id of the moving path
        protected int deltaX, deltaY; // moving distance

        /** constructor
         */
        public MovingPath() { }

        /** abstract move method
        * move the shape according to the path
        */
        public abstract void move();
    }

    /*
     *  ===============================================================================
     *  FallingPath : A falling path.
     *  ===============================================================================
     */
    public class FallingPath extends MovingPath {
        private double am = 0, stx =0, sinDeltax = 0;

        /** constructor to initialise values for a falling path
        */
        public FallingPath() {
            am = Math.random() * 20; //set amplitude variables
            stx = 0.5; //set step variables
            deltaY = 5;
            sinDeltax = 0;
       }

       /** move the shape
       */
       public void move() {
           sinDeltax = sinDeltax + stx;
           x = (int) Math.round(x + am * Math.sin(sinDeltax));
           y = y + deltaY;
           if (y > marginHeight) // if it reaches the bottom of the frame, start again from the top
               y = 0;
       }
    }
    /*
     *  ===============================================================================
     *  BouncingPath : A Bouncing path.
     *  ===============================================================================
     */
     //complete this
    public class BouncingPath extends MovingPath {
        private double am = 0, stx =0, sinDeltax = 0;

        /** constructor to initialise values for a bouncing path
        */
        public BouncingPath() {
            am = Math.random() * 20; //set amplitude variables
            stx = 0.5; //set step variables
            deltaY = 5;
            sinDeltax = 0;
       }

       /** move the shape
       */
       public void move() {
           sinDeltax = sinDeltax + stx;
           x = (int) Math.round(x + am * Math.sin(sinDeltax));
           y = y + deltaY;
           if (y > marginHeight || y < 0) {// if it reaches the bottom of the frame, start again from the top
               //y = 0;
               deltaY = -deltaY;
           }
       }
    }
}

class MovingRectangle extends MovingShape {
    protected int isSquare;

    public MovingRectangle(int sh, int sw) {
        super(sh, sw);
    }

    public MovingRectangle(int x, int y, int sh, int sw, int mw, int mh, Color c, Color fc, int pathType) {
        super(x, y, sh, sw, mw, mh, c, fc, pathType);
    }

    /** contains method
     * Returns whether the point p is inside the shape or not.
     * @param p    the mouse point
     */
    public boolean contains(Point p) {

        if (p.getX() >= x && p.getX() <= x+shapeWidth
              && p.getY() >= y && p.getY() <= y+shapeHeight)
            return true;

        return false;
    }

    /** draw method
     * draw the shape
     * @param g     the Graphics control
     */
    public void draw(Graphics g) {
        Rectangle2D rect = new Rectangle2D.Double(x, y, shapeWidth, shapeHeight);
        g.setColor(borderColor);
        ((Graphics2D)g).draw(rect);
        g.setColor(fillColor);

        rect = new Rectangle2D.Double(x+1, y+1, shapeWidth-1, shapeHeight-1);
        ((Graphics2D)g).fill(rect);

        if (isSelected()) {
            drawHandles(g);
        }
    }

    /** area method
     * calculate the area of shape
     * @return int     the area of rectangle
     */
    public int area() {
        return shapeWidth * shapeHeight;
    }
}

class MovingSquare extends MovingShape {

    public MovingSquare(int sh, int sw) {
        super(sh, sw);
    }

    public MovingSquare(int x, int y, int sh, int sw, int mw, int mh, Color c, Color fc, int pathType) {
        super(x, y, sh, sw, mw, mh, c, fc, pathType);
    }

    /** contains method
     * Returns whether the point p is inside the shape or not.
     * @param p    the mouse point
     */
    public boolean contains(Point p) {

        if (p.getX() >= x && p.getX() <= x+shapeWidth
              && p.getY() >= y && p.getY() <= y+shapeHeight)
            return true;

        return false;
    }

    /** draw method
     * draw the shape
     * @param g     the Graphics control
     */
    public void draw(Graphics g) {
        Rectangle2D rect = new Rectangle2D.Double(x, y, shapeWidth, shapeHeight);
        g.setColor(borderColor);
        ((Graphics2D)g).draw(rect);
        g.setColor(fillColor);

        rect = new Rectangle2D.Double(x+1, y+1, shapeWidth-1, shapeHeight-1);
        ((Graphics2D)g).fill(rect);
    }

    /** Set the width of the shape.
     * @param sw     the width value
     */
    public void setWidth(int sw) { 
        shapeWidth = sw; 
        shapeHeight = sw;
    }

    /** Set the height of the shape.
     * @param sh     the height value
     */
    public void setHeight(int sw) { 
        shapeHeight = sw;
        shapeWidth = sw; 
    }

    /** area method
     * calculate the area of shape
     * @return int     the area of square
     */
    public int area() {
        return shapeWidth * shapeWidth;
    }
}
