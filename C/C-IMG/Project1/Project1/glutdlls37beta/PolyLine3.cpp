/*
 * Date: 2015-12-10
 */
#include <GL/glut.h>
#include <vector>

using namespace std;

GLint nowChoose = 5; // do nothing

GLsizei winWidth = 800, winHeight = 600;   // Initial display-window size.

class scrPt {
	public:
		GLint x, y;
};

/*
 * one polyline shape is compsite of max 60 lines,
 * the pointNum indicate the lines number.
 */
class ptArray {
	public:
		scrPt pts[60];
		GLint pointNum;
};

/*
 * use dynamic storage to store the many poly-shapes
 */
vector<ptArray> polyshape; // polylines

GLint polyNum = -1;

void stateChange(unsigned char key, int x, int y)
{
	switch (key) {
		case 'b':
			nowChoose = 0;
			ptArray pta;
			polyshape.push_back(pta);
			polyNum++;
			polyshape[polyNum].pointNum = 0;
			break;
		case 'd':
			nowChoose = 1;
			break;
		case 'm':
			nowChoose = 2;
			break;
		case 'r':
			nowChoose = 3;
			polyNum = 0;
			polyshape.clear();
			break;
		case 'q':
			nowChoose = 4;
			break;
	}
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

void drawLineSegment(scrPt endPt1, scrPt endPt2)
{
	glPointSize(5);
	glLineWidth(5);

	glBegin(GL_LINES);
		glVertex2i(endPt1.x, endPt1.y);
		glVertex2i(endPt2.x, endPt2.y);
	glEnd();
}

void drawAllLines()
{
	int i, k;
	for (k = 0; k <= polyNum; k++) {
		for (i = 0; i < polyshape[k].pointNum-1; i++) {
			drawLineSegment(polyshape[k].pts[i], polyshape[k].pts[i+1]);
		}
	}
}

void mouseActions(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
	int x, y;
	int i, k, m;

	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
		if (nowChoose == 0) {
			x = xMouse;
			y = winHeight - yMouse;

			polyshape[polyNum].pts[polyshape[polyNum].pointNum].x = x;
			polyshape[polyNum].pts[polyshape[polyNum].pointNum].y = y;

			polyshape[polyNum].pointNum++;
		}
	}

	/* delete */
	if (nowChoose == 1) {

		x = xMouse;
		y = winHeight - yMouse;

		/* find a near point to delete */
		for (m = 0; m <= polyNum; m++) {
			for (i = 0; i < polyshape[m].pointNum; i++) {
				int deltaX = polyshape[m].pts[i].x - x;
				int deltaY = polyshape[m].pts[i].y - y;

				if (deltaX < 10 && deltaX > -10 
						&& deltaY < 10 && deltaY > -10) {

					for (k = i; k < polyshape[m].pointNum-1; k++) {
						polyshape[m].pts[k] = polyshape[m].pts[k+1];
					}

					polyshape[m].pointNum--;
					polyshape[m].pts[polyshape[m].pointNum].x = 0;
					polyshape[m].pts[polyshape[m].pointNum].y = 0;

					break;
				}
			}
		}
	}

	/* move */
	if (nowChoose == 2) {

		x = xMouse;
		y = winHeight - yMouse;

		/* find a near point to move */
		for (m = 0; m <= polyNum; m++) {
			for (i = 0; i < polyshape[m].pointNum; i++) {
				int deltaX = polyshape[m].pts[i].x - x;
				int deltaY = polyshape[m].pts[i].y - y;

				if (deltaX < 10 && deltaX > -10 
						&& deltaY < 10 && deltaY > -10) {

					polyshape[m].pts[i].x = x;
					polyshape[m].pts[i].y = y;

					break;
				}
			}
		}
	}

	if (nowChoose == 4) exit(0);

	glClear(GL_COLOR_BUFFER_BIT);
	/* draw all lines */
	drawAllLines();

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
	glutKeyboardFunc(stateChange);
	glutMouseFunc(mouseActions);
	glutMainLoop();

	return 0;
}
