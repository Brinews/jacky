/* 
 * Date: 2015-12-12
 */

#include <GL/glut.h>
#include <stdio.h>
#include <list>
#include <iterator>

using namespace std;

#define DLTA 9

GLsizei winWidth = 400, winHeight = 300;   // Initial display-window size.

/*
 * mouse clicked point
 */
class scrPt {
public:
   GLint x, y;
   /* override function */
   bool operator==(const scrPt &p) {
	   return this->x == p.x && this->y == p.y;
   }
};

list<scrPt> polylist;

/*
 * command keys
 */
typedef enum {
	KEY_BEG,
	KEY_DEL,
	KEY_MOV,
	KEY_REF,
	KEY_QUI,
	KEY_N,
} State;

/*
 * there is no keys pressed initially
 */
State curState = KEY_N;

/* 
 * poly line number
 */
GLint moreLines = -1;

bool myNearPoint(int dX, int dY)
{
	if (dX < DLTA && dX > -DLTA && dY < DLTA && dY > -DLTA) return true;
	return false;
}

void init(void)
{
   glClearColor(0.0, 0.0, 1.0, 1.0);   // Set display-window color to blue.
   glMatrixMode(GL_PROJECTION);
   gluOrtho2D(0.0, 200.0, 0.0, 150.0);
}

void displayFcn(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glFlush();
}

void winReshapeFcn(GLint newWidth, GLint newHeight)
{
   /*  Reset viewport and projection parameters  */
   glViewport(0, 0, newWidth, newHeight);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0, GLdouble (newWidth), 0.0, GLdouble (newHeight));

   /*  Reset display-window size parameters.  */
   winWidth  = newWidth;
   winHeight = newHeight;
}

/*
 * single line draw
 * @param endPt1 start point
 * @param endPt2 end point
 */
void myLinePlot(scrPt endPt1, scrPt endPt2)
{
   glBegin(GL_LINE_STRIP);
      glVertex2i(endPt1.x, endPt1.y);
      glVertex2i(endPt2.x, endPt2.y);
   glEnd();
}

/*
 * poly-lines draw
 */
void myPolylinesPlot()
{
	list<scrPt>::iterator it, cur;

	/* draw all the lines, except the (0,0) additional point */
	for (it = polylist.begin(); it != polylist.end();) {

		cur = it;
		it++;

		if (it == polylist.end()) break; /* end */
		if ((*it).x == 0 && (*it).y == 0) { continue; }
		if ((*cur).x == 0 && (*cur).y == 0) { continue; }

		myLinePlot(*cur, *it);
	}
}

/*
 * add a line into polylist
 */
void myLineAdd(int x, int y)
{
	scrPt newline;
	newline.x = x; newline.y = y;
	/* add to list */
	polylist.push_back(newline);
}

/*
 * repaint all the shapes
 */
void myPolyLines(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
   int dX, dY;
   int x, y; /* mouse clicked position */

   x = xMouse;
   y = winHeight - yMouse;
   
   /*
	* left button pressed
	*/
   if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {

	  if (curState == KEY_BEG) {

		  /* a new poly-line shape added */
		  if (moreLines == 1) {
			  myLineAdd(0, 0); moreLines = 0;
		  }

		  myLineAdd(x, y);
	  }

	  if (curState == KEY_DEL) {

		  /* remove a point
		   * travese all points and find a closest one to remove
		   */
		  list<scrPt>::iterator it;
		  for (it = polylist.begin(); it != polylist.end(); it++) {
			  dX = (*it).x - x;
			  dY = (*it).y - y;

			  /* find a close point */
			  if (myNearPoint(dX, dY)) {

				  /* remove a node */
				  polylist.remove(*it);
				  break;
			  }
		  }
	  }

	  if (curState == KEY_MOV) {

		  list<scrPt>::iterator it;

		  /*
		   * find the closest point to move
		   */
		  for (it = polylist.begin(); it != polylist.end(); it++) {
			  dX = (*it).x - x;
			  dY = (*it).y - y;

			  /* location near point */
			  if (myNearPoint(dX, dY)) {
				  (*it).x = x; (*it).y = y;
				  break; 
			  }
		  }
	  }
   }
   
   if (curState == KEY_QUI) {
	   polylist.clear();
	   exit(0);
   }

   /* repaint */
   glClear(GL_COLOR_BUFFER_BIT);
   myPolylinesPlot();
   glFlush();
}

/*
 * key pressed action
 */
void myKeyPressed(unsigned char key, int x, int y)
{
	switch (key) {
	case 'b': case 'B':
		curState = KEY_BEG;
		moreLines++;
		break;
	case 'd': case 'D':
		curState = KEY_DEL;
		break;
	case 'm': case 'M':
		curState = KEY_MOV;
		break;
	case 'r': case 'R':
		curState = KEY_REF;
		polylist.clear();
		break;
	case 'q': case 'Q':
		curState = KEY_QUI;
		break;
	}
}

int main (int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowPosition(100, 100);
   glutInitWindowSize(winWidth, winHeight);
   glutCreateWindow("Draw Interactive Polyline");
   init();
   glutDisplayFunc(displayFcn);
   glutReshapeFunc(winReshapeFcn);
   glutKeyboardFunc(myKeyPressed);
   glutMouseFunc(myPolyLines);
   glutMainLoop();

   return 0;
}
