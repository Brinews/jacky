#include <GL/glut.h>
#include <stdio.h>

/*
 * keyboard command
 */
typedef enum {
	KEY_B,
	KEY_D,
	KEY_M,
	KEY_R,
	KEY_Q,
	KEY_Z,
} State;

/*
 * init key state 
 */
State currentstate = KEY_Z;

GLsizei winWidth = 400, winHeight = 300;   // Initial display-window size.
GLint endPtCtr = 0;                   // Initialize line endpoint counter.

/*
 * point struct 
 */
class scrPt {
public:
   GLint x, y;
   scrPt *next;

   scrPt() {
	   x = y = 0;
	   next = NULL;
   }
};

scrPt poly; // global points list
scrPt *polytail = &poly;

/* 
 * poly line number
 */
GLint lineNum = -1;

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
 * draw a line use opengl function
 */
void linePlot(scrPt *endPt1, scrPt *endPt2)
{
   glBegin(GL_LINE_STRIP);
      glVertex2i(endPt1->x, endPt1->y);
      glVertex2i(endPt2->x, endPt2->y);
   glEnd();
}

/*
 * draw all the lines
 */
void polyLinesDraw()
{
	scrPt *ptr = poly.next;

	while (ptr != NULL) {
		scrPt *npt = ptr->next;

		if (npt == NULL) break;

		if (ptr->x == 0 && ptr->y == 0) {
			ptr = ptr->next;
			continue;
		}
		if (npt->x == 0 && npt->y == 0) {
			ptr = ptr->next;
			continue;
		}

		linePlot(ptr, npt);

		ptr = ptr->next;
	}
}

/*
 * add a new line into list
 */
void addNewLine(int x, int y)
{
	scrPt *newline = new scrPt();
	newline->next = NULL;
	newline->x = x;
	newline->y = y;

	polytail->next = newline;
	polytail = newline;
}

/*
 * free all lines
 */
void freeLines(scrPt *ptr)
{
	while (ptr != NULL) {
		scrPt *tmp = ptr->next;
		delete ptr;
		ptr = tmp;
	}
}

/*
 * repaint overload
 */
void polyDraw(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
   int dX, dY;
   scrPt endPt1;

   endPt1.x = xMouse;
   endPt1.y = winHeight - yMouse;
   
   /*
	* left button click
	*/
   if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {

	  if (currentstate == KEY_B) {

		  if (lineNum > 0) {
			  addNewLine(0, 0);
			  lineNum = 0;
		  }

		  addNewLine(endPt1.x, endPt1.y);
	  }

	  if (currentstate == KEY_D) {

		  /* delete a selected point */

		  scrPt *prev = &poly;
		  scrPt *ptr = poly.next;

		  /*
		   * travese all point 
		   */
		  while (ptr != NULL) {
			  dX = ptr->x - endPt1.x;
			  dY = ptr->y - endPt1.y;

			  /* location near point */
			  if (dX < 7 && dX > -7 && dY < 7 && dY > -7) {

				  if (ptr == polytail) {
					  polytail = prev;
				  }

				  /* free the found node */
				  prev->next = ptr->next;
				  delete ptr;

				  break;
			  }

			  prev = ptr;
			  ptr = ptr->next;
		  }
	  }

	  if (currentstate == KEY_M) {

		  scrPt *ptr = poly.next;

		  /*
		   * travese all point 
		   */
		  while (ptr != NULL) {
			  dX = ptr->x - endPt1.x;
			  dY = ptr->y - endPt1.y;

			  /* location near point */
			  if (dX < 7 && dX > -7 && dY < 7 && dY > -7) {

				  ptr->x = endPt1.x;
				  ptr->y = endPt1.y;

				  break; 
			  }

			  ptr = ptr->next;
		  }
	  }
   }
   
   if (currentstate == KEY_Q) {
	   freeLines(poly.next);
	   poly.next = NULL;
	   exit(0);
   }

   /* repaint */
   glClear(GL_COLOR_BUFFER_BIT);
   polyLinesDraw();
   glFlush();
}

void keyState(unsigned char key, int x, int y)
{
	switch (key) {
	case 'b': case 'B':
		currentstate = KEY_B;
		lineNum++;
		break;
	case 'd': case 'D':
		currentstate = KEY_D;
		break;
	case 'm': case 'M':
		currentstate = KEY_M;
		break;
	case 'r': case 'R':
		currentstate = KEY_R;
		freeLines(poly.next);
		poly.next = NULL;
		polytail = &poly;
		glFlush();
		break;
	case 'q': case 'Q':
		currentstate = KEY_Q;
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
   glutKeyboardFunc(keyState);
   glutMouseFunc(polyDraw);
   glutMainLoop();

   return 0;
}
