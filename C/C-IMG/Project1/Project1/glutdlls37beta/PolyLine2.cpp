/*
 * Date: 2015-12-09
 * Paint board of OpenGL
 */

#include <GL/glut.h>

// display-window size.
GLsizei winWidth = 300, winHeight = 300;   

#define MAX 300 // max points num
#define DELTA 8 // points distance 

class scrPt {
public:
   GLint x, y;
};

scrPt polylines[MAX]; // to paint points
GLint pnum = 0;  // points total
GLint polynum = -1; // poly shape total
GLint selection = 4; // current actions

typedef void(*funcPtr)(int, int);

/* paint functions */
void do_paint(int, int);
void do_delete(int, int);
void do_move(int, int);
void do_refresh(int, int);

funcPtr paintFunc[4] = {do_paint, do_delete, do_move, do_refresh};

void bindKeyFcn(unsigned char key, int x, int y)
{
	switch (key) {
	case 'b': case 'B':
		selection = 0; polynum++;
		break;
	case 'd': case 'D':
		selection = 1;
		break;
	case 'm': case 'M':
		selection = 2;
		break;
	case 'r': case 'R':
		selection = 3;
		break;
	case 'q': case 'Q': case 27:
		exit(0);
	}
}


void do_paint(int x, int y)
{
	/* add point to array for paint later */
	if (pnum < MAX) {
		/* point(0,0) means no paint */
		if (polynum > 0) {
			polylines[pnum].x = polylines[pnum].y = 0;
			pnum++; polynum = 0;
		}

		polylines[pnum].x = x;
		polylines[pnum].y = y;

		pnum++;
	}
}

void do_delete(int x, int y)
{
	int i, k;
	int dX, dY;

	/* find a near point delete point */
	for (i = 0; i < pnum; i++) {
		dX = polylines[i].x - x; dY = polylines[i].y - y;

		if (dX < DELTA && dX > -DELTA && dY < DELTA && dY > -DELTA) {

			for (k = i; k < pnum-1; k++) {
				polylines[k].x = polylines[k+1].x;
				polylines[k].y = polylines[k+1].y;
			}
			pnum--;
			polylines[pnum].x = polylines[pnum].y = 0;
			break; 
		}
	}
}

void do_move(int x, int y)
{
	int i;
	int dX, dY;

	/* find a near point to move */
	for (i = 0; i < pnum; i++) {
		dX = polylines[i].x - x; dY = polylines[i].y - y;

		if (dX < DELTA && dX > -DELTA && dY < DELTA && dY > -DELTA) {
			/* update to new location */
			polylines[i].x = x; polylines[i].y = y;
			break; 
		}
	}
}

void do_refresh(int x, int y)
{
	int i = 0;
	/* clear points */
	for (i = 0; i < pnum; i++)
		polylines[i].x = polylines[i].y = 0;

	pnum = 0;
}

void init(void)
{
   glClearColor(0.0, 0.0, 0.0, 1.0);   // Set display-window color to blue.
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

void polylineShape()
{
	int i;
	for (i = 0; i < pnum-1; i++) {
		if (polylines[i].x == 0 && polylines[i].y == 0) continue;
		if (polylines[i+1].x == 0 && polylines[i+1].y == 0) continue;
		drawLineSegment(polylines[i], polylines[i+1]);
	}
}

/*
 * action on mouse
 * @param button button type
 * @param action click action
 * @param xMouse mouse of horizon location
 * @param yMouse mouse of vertical location
 */
void bindMouseFcn(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
   if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
	   if (selection < 4)
		   paintFunc[selection](xMouse, winHeight - yMouse);
   }

   glClear(GL_COLOR_BUFFER_BIT);

   /* refresh */
   polylineShape();
   glFlush();
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
   glutKeyboardFunc(bindKeyFcn);
   glutMouseFunc(bindMouseFcn);
   glutMainLoop();

   return 0;
}
