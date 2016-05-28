/* GraphX.hpp
 * Interface for the GraphX class
 *      This is a simple user interface that will allow students to utilize
 *      simple graphics operations in C++ labs.  It is mapped onto the Xlib
 *      graphical library.
 *
 * Fall 2012 Authors:
 *      Steve Jensen
 *      Hengyi Huang
 * Spring 2014 Authors:
 *      Steve Jensen
 *      Tom Postler
 *
 * Version 14.04.18
 */

#ifndef GRAPHX_HPP
#define GRAPHX_HPP

#include <X11/Xlib.h>
#include <string>
using namespace std;

class GraphX
{
public:

    // constructors
    GraphX();               // default: create a 800 pixel wide x 600 pixel high window
    GraphX(int w, int h);   // create a w-pixel wide x h-pixel high window
    ~GraphX();              // destructor: close the x-server connection and remove window

    // drawing primitives
    void setpos(int x, int y);          //  move the pen location to x,y
    void setposition(int x, int y);     //  ^^
    void setx(int x);                   //  move the pen location to x, unchanged y
    void sety(int y);                   //  move the pen location to unchanged x, y
    void plot();                        //  plot current symbol at pen location
    void erase();                       //  erase current symbol at pen location
    void penup()  {pendn = false;}      //  pendown state = false
    void pu(){penup();}                 //  ^^
    void up(){penup();}                 //  ^^
    void pendown(){pendn = true;}       //  pendown state = true
    void pd(){pendown();}               //  ^^
    void down(){pendown();}             //  ^^
    void clear();                       //  fill window with background color
    void clearscreen(){clear();}        //  ^^
    void reset(){clear();}              //  ^^
    void resetscreen(){clear();}        //  ^^
    void delay(double secs)             //  delay start of next GraphX operation
            {delaytime = secs;}
    void screensize(int x, int y);      //  clear and resize the GraphX window
    void screensize(int x, int y, string c)			// ^^ and change color
            {backgroundcolor(c); screensize(x,y);}

    // appearance
    void pencolor(string);         		//  set pen color
    void pensize(int);                  //  set pen size (in pixels)
    void width(int w){pensize(w);}      //  ^^
    void penstyle(string);         		//  solid line, dashes, dots, dash-dot
    void symbol(string);           		//  set the plot symbol
    void symbolsize(int);               //  set the size of the plotted symbol
    void backgroundcolor(string);  		//  set background color

    // scaling
    void scale(int width, int height);  //  set pixel units for x & y
    void origin(int x, int y);          //  set the origin to x units, y units
    void xaxis( );                      //  draw a line through the x axis
    void yaxis( );                      //  draw a line through the y axis

    // status
    bool isopen( ){return openstate;}   //  return state of graphics environment
    bool fail(){return !openstate;}     //  return status of last operation
    bool isdown(){return pendn;}        //  return pen state (down = true)
    int  xcor(){return xloc;}           //  return current pen location
    int  ycor(){return yloc;}           //  return current pen location
    void position(int &x, int &y);      //  return current pen x,y location
    void pos(int &x, int &y);           //  ^^
    int  pensize(){return psize;}       //  return pen size (in pixels)
    int  width(){return pensize();}     //  ^^
    void getscreensize(int &x, int &y)  //  return the current screen size
            {x = winwidth; y = winheight;}
    int  delay(){return delaytime;}     //  return delay between drawing ops

    // extended drawing
    void moverel(int dx, int dy);       //  move relative to current location
    void plot(int x, int y);            //  combine 'move' and 'plot' operations
    void plot(string);             		//  set current symbol and plot it
    void plot(int,int,string);     		//  combine 'move' and 'plot(symbol')

    // input
    string 	input();                	//  return input status
    char    eventkey();                 //  return key event value
    void    eventxy(int &x, int &y);    //  return x,y (scaled appropriately)

private:

    enum GXColor  {BLACK=0, WHITE=1, RED=2, YELLOW=3, BLUE=4, GREEN=5, ORANGE=6,
                   PURPLE=7};

    enum GXLine   {SOLID, DOTS, DASH, LONGDASH, DOTDASH};

    enum GXSym    {CIRCLE, SQUARE, DIAMOND, TRIANGLE, INVTRIANGLE,
                   SOLIDCIRCLE, SOLIDSQUARE, SOLIDDIAMOND, SOLIDTRIANGLE,
                   SOLIDINVTRIANGLE};

    enum GXEvent  {NONE=0, KEYPRESS=1, LEFTBUTTON=2, RIGHTBUTTON=3};

    bool    openstate;                  //  window is opened and OK to use
    int     winwidth;                   //  width of window in pixels
    int     winheight;                  //  height of window in pixels
    int     xloc;                       //  current pen location (scale units)
    int     yloc;                       //  current pen location (scale units)
    double  xscale;                     //  pixels per x unit
    double  yscale;                     //  pixels per y unit
    int     xorigin;                    //  optional xorigin value (scale units)
    int     yorigin;                    //  optional yorigin value (scale units)
    double  delaytime;                  //  delay next operation by (in microseconds)
    GXColor bgcolor;                    //  current background color
    int     psize;                      //  pen size
    GXColor pcolor;                     //  pen color
    GXLine  pstyle;                     //  pen style
    GXSym   psymbol;                    //  plotting symbol
    int     symsize;                    //  size (in pixels) of the plotted symbol
    bool    pendn;                      //  pen 'down' state (true/false)
    GXEvent lastinput;                  //  most recent keyboard/mouse event
    char    kval;                       //  last keyboard event: key value
    int     buttonx;                    //  last button event: x location
    int     buttony;                    //  last button event: y location

    //Xlib related data
    Display *disp;                      //  the display pointer
    int     screen;                     //  default screen identifier
    Window  win;                        //  the main window (added)
    XEvent  event;                      //  event reporting structure
    GC      gc;                         //  graphics context for drawing
    unsigned long pixelvalues[8];       //  color pixel values (mapped at start)

    //Private class functions
    void    wait();                     //  delay a graphics operation
    void    setDefaults();              //  initialize all the default values
    bool    openX(int, int);            //  create and initialize environment
    void    drawSymbol(GXColor);        //  draw current symbol
    void    setLineGC(GXColor);         //  set the Xlib GC information for a line
    int     mapX(int);                  //  return pixel offset given x location
    int     mapY(int);                  //  return pixel offset given y location
    GXColor getColor(string);      		//  string to GXColor conversion
    GXLine  getLine(string);       		//  string to GXLine conversion
    GXSym   getSym(string);        		//  string to GXSym conversion
};

#endif
