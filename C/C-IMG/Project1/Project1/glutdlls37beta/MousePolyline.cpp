#include <GL/glut.h>

#define MAXPN	600

typedef enum {
	PAINT_BEGIN,
	PAINT_DELETE,
	PAINT_MOVE,
	PAINT_REFRESH,
	PAINT_QUIT,
	PAINT_LOOP,
} Action;

Action curact = PAINT_LOOP; // do nothing

GLsizei winWidth = 400, winHeight = 300;   // Initial display-window size.
GLint endPtCtr = 0;                   // Initialize line endpoint counter.

class scrPt {
public:
   GLint x, y;
};

scrPt poly[MAXPN]; // global points array
GLint pointNum = 0;
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

void drawLineSegment(scrPt endPt1, scrPt endPt2)
{
   glBegin(GL_LINE_STRIP);
      glVertex2i(endPt1.x, endPt1.y);
      glVertex2i(endPt2.x, endPt2.y);
   glEnd();
}

void polyLinesDraw()
{
	int i;
	for (i = 0; i < pointNum-1; i++) {
		if (poly[i].x == 0 && poly[i].y == 0) continue;
		if (poly[i+1].x == 0 && poly[i+1].y == 0) continue;
		drawLineSegment(poly[i], poly[i+1]);
	}
}

void polyline(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
   scrPt endPt1;
   int i, k;
   
   if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {

	  if (curact == PAINT_BEGIN) {

		  if (pointNum < MAXPN) {

			  endPt1.x = xMouse;
			  endPt1.y = winHeight - yMouse;

			  if (lineNum > 0) {
				  poly[pointNum].x = poly[pointNum].y = 0;
				  pointNum++;
				  lineNum = 0;
			  }

			  poly[pointNum].x = endPt1.x;
			  poly[pointNum].y = endPt1.y;

			  pointNum++;
		  }
	  }

	  if (curact == PAINT_DELETE) {

		  endPt1.x = xMouse;
		  endPt1.y = winHeight - yMouse;

		  /* find to delete point */
		  for (i = 0; i < pointNum; i++) {
			  int deltaX = poly[i].x - endPt1.x;
			  int deltaY = poly[i].y - endPt1.y;

			  if (deltaX < 7 && deltaX > -7 
				&& deltaY < 7 && deltaY > -7) {

				  for (k = i; k < pointNum-1; k++) {
					  poly[k] = poly[k+1];
				  }

				  pointNum--;
				  poly[pointNum].x = poly[pointNum].y = 0;

				  break; /* only find one point to delete */
			  }
		  }
	  }

	  if (curact == PAINT_MOVE) {

		  endPt1.x = xMouse;
		  endPt1.y = winHeight - yMouse;

		  for (i = 0; i < pointNum; i++) {
			  int deltaX = poly[i].x - endPt1.x;
			  int deltaY = poly[i].y - endPt1.y;

			  if (deltaX < 7 && deltaX > -7 
				&& deltaY < 7 && deltaY > -7) {

				  poly[i].x = endPt1.x;
				  poly[i].y = endPt1.y;

				  break; /* only find one point to move */
			  }
		  }
	  }
   }
   
   if (curact == PAINT_QUIT)        // Quit the program.
      exit(0);


   glClear(GL_COLOR_BUFFER_BIT);

   /* draw all line */
   polyLinesDraw();

   glFlush();
}

void keyAction(unsigned char key, int x, int y)
{
	switch (key) {
	case 'b':
		curact = PAINT_BEGIN;
		lineNum++;
		break;
	case 'd':
		curact = PAINT_DELETE;
		break;
	case 'm':
		curact = PAINT_MOVE;
		break;
	case 'r':
		curact = PAINT_REFRESH;
		pointNum = 0;
		glFlush();
		break;
	case 'q':
		curact = PAINT_QUIT;
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
   glutKeyboardFunc(keyAction);
   glutMouseFunc(polyline);
   glutMainLoop();

   return 0;
}
