/*
 * CodeTime: 2015-12-10
 */

#include <GL/glut.h>
#include <stdio.h>

#define MINDIST 9

GLsizei winWidth = 400, winHeight = 300;   // Initial display-window size.

/*
 * keyboard command state
 */
typedef enum {
	KeyBegin,
	KeyDelete,
	KeyMove,
	KeyRefresh,
	KeyQuit,
	KeyVoid,
} State;

/*
 * init  key state
 */
State nowKeyPressed = KeyVoid;

/*
 * point class
 */
class scrPt {
public:
   GLint x, y;
   scrPt *nextPt;

   /* constructor */
   scrPt(): nextPt(NULL) {}
};

scrPt polylines[60]; /* max 60 shapes */
scrPt *lastpolypoint = &polylines[0];

/* 
 * polylines number
 */
GLint lineNum = 0;

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
 * draw line 
 */
void plotOneLine(scrPt *endPt1, scrPt *endPt2)
{
   glBegin(GL_LINE_STRIP);
      glVertex2i(endPt1->x, endPt1->y);
      glVertex2i(endPt2->x, endPt2->y);
   glEnd();
}

/*
 * draw lines
 */
void plotLines()
{
	for (int i = 0; i < lineNum; i++) {
		scrPt *ptPtr = polylines[i].nextPt;

		/* draw one poly-lines */
		while (ptPtr != NULL) {
			scrPt *npt = ptPtr->nextPt;
			if (npt == NULL) break;
			plotOneLine(ptPtr, npt);

			ptPtr = ptPtr->nextPt;
		}
	}
}

/*
 * add a new line into current list
 */
void addNewLine(int x, int y)
{
	scrPt *newline = new scrPt();
	newline->nextPt = NULL;
	newline->x = x;
	newline->y = y;

	lastpolypoint->nextPt = newline;
	lastpolypoint = newline;
}

/*
 * free all lines
 */
void deleteAllLines()
{
	for (int i = 0; i < lineNum; i++) {
		scrPt *ptPtr = polylines[i].nextPt;
		while (ptPtr != NULL) {
			scrPt *tmp = ptPtr->nextPt;
			delete ptPtr;
			ptPtr = tmp;
		}
	}
}

/*
 * repaint overload
 */
void mouseResponse(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
   // calculate the distance
   int dX, dY;
   // current location
   int x, y;
   int i, k;

   x = xMouse;
   y = winHeight - yMouse;
   
   /*
	* left button click
	*/
   if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {

	  if (nowKeyPressed == KeyBegin) {
		  addNewLine(x, y);
	  }

	  if (nowKeyPressed == KeyDelete) {

		  /* 
		   * delete the clicked point 
		   */

		  for (i = 0; i < lineNum; i++) {
			  scrPt *prev = &polylines[i];
			  scrPt *ptPtr = polylines[i].nextPt;

			  int found = 0;

			  /*
			   * get all the points
			   */
			  while (ptPtr != NULL) {
				  dX = ptPtr->x - x;
				  dY = ptPtr->y - y;

				  /* location near point */
				  if (dX < MINDIST && dX > -MINDIST && dY < MINDIST && dY > -MINDIST) {

					  /* free the found node */
					  prev->nextPt = ptPtr->nextPt;
					  delete ptPtr;

					  /* one poly-line-shape is removed */
					  if (prev == &polylines[i]
							  && prev->nextPt == NULL) {
						  for (k = i; k < lineNum-1; k++)
							  polylines[k] = polylines[k+1];
						  lineNum--;
						  polylines[lineNum].nextPt = NULL;
					  }

					  found = 1;

					  break;
				  }

				  prev = ptPtr;
				  ptPtr = ptPtr->nextPt;
			  }

			  if (found == 1) break;
		  }
	  }

	  if (nowKeyPressed == KeyMove) {

		  for (i = 0; i < lineNum; i++) {
			  int found = 0;
			  scrPt *ptPtr = polylines[i].nextPt;

			  /*
			   * review all points
			   */
			  while (ptPtr != NULL) {
				  dX = ptPtr->x - x;
				  dY = ptPtr->y - y;

				  /* get clicked point */
				  if (dX < MINDIST && dX > -MINDIST && dY < MINDIST && dY > -MINDIST) {

					  ptPtr->x = x;
					  ptPtr->y = y;

					  found = 1;

					  break; 
				  }

				  ptPtr = ptPtr->nextPt;
			  }

			  if (found == 1) break;
		  }
	  }
   }

   if (nowKeyPressed == KeyQuit) {
	   deleteAllLines();
	   exit(0);
   }

   /* repaint */
   glClear(GL_COLOR_BUFFER_BIT);
   plotLines();
   glFlush();
}

void keyboardResponse(unsigned char key, int x, int y)
{
	switch (key) {
	case 'b': case 'B':
		nowKeyPressed = KeyBegin; /* add new polylines-shape */

		if (lineNum < 60) {
			lastpolypoint = &polylines[lineNum];
			lineNum++;
		}

		break;
	case 'd': case 'D': /* delete a clicked point and its line */
		nowKeyPressed = KeyDelete;
		break;
	case 'm': case 'M': /* move a clicked point and its line */
		nowKeyPressed = KeyMove;
		break;
	case 'r': case 'R': /* clear all the points and refresh paint board */
		nowKeyPressed = KeyRefresh;
		deleteAllLines();
		lastpolypoint = &polylines[0];
		lineNum = 0;
		glFlush();
		break;
	case 'q': case 'Q': case 27: /* quit */
		nowKeyPressed = KeyQuit;
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

   /* key board response */
   glutKeyboardFunc(keyboardResponse);

   /* mouse click response */
   glutMouseFunc(mouseResponse);

   glutMainLoop();

   return 0;
}
