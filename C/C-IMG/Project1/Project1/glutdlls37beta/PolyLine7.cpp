/* 
 * Date: 2015-12-11
 * HowTo: keys(b, d, m, r, q), mouse draw
 */

#include <GL/glut.h>
#include <stdio.h>
#include <list>
#include <iterator>
#include <map>

using namespace std;

#define DIST 9

GLsizei winWidth = 400, winHeight = 300;   // Initial display-window size.

/*
 * mouse clicked point
 */
class scrPt {
public:
   GLint x, y;
   bool operator==(const scrPt &p) {
	   return this->x == p.x && this->y == p.y;
   }
};

list<scrPt> myPolyList;

/*
 * command keys
 */
typedef enum {
	KEY_ADD,
	KEY_DEL,
	KEY_MOV,
	KEY_REF,
	KEY_QUI,
	KEY_NIL,
} State;

/*
 * there is no keys pressed initially
 */
State choosenkey = KEY_NIL;

/* 
 * poly line number
 */
GLint anotherPoly = 1;

typedef void(*funcPtr)(int, int);

/* paint functions */
void mpl_add(int, int);
void mpl_delete(int, int);
void mpl_move(int, int);
void mpl_refresh(int, int);
void mpl_quit(int, int);

map<State, funcPtr> funcMap;

/*
 * add to myPolyList
 */
void mpl_addline(int x, int y)
{
	scrPt nline;
	nline.x = x; nline.y = y;
	myPolyList.push_back(nline);
}

void mpl_add(int x, int y)
{
	/* a new poly-line shape added */
	if (anotherPoly == 3) {
		mpl_addline(0, 0); anotherPoly = 2;
	}

	mpl_addline(x, y);
}

bool isPointSelected(int deltax, int deltay)
{
	if (deltax < DIST && deltax > -DIST && deltay < DIST && deltay > -DIST) return true;
	return false;
}

void mpl_delete(int x, int y)
{
	int deltax, deltay;
	/*
	 * find a close point to remove
	 */
	list<scrPt>::iterator it;
	for (it = myPolyList.begin(); it != myPolyList.end(); it++) {
		deltax = (*it).x - x;
		deltay = (*it).y - y;
		if (isPointSelected(deltax, deltay)) {
			myPolyList.remove(*it);
			break;
		}
	}
}

void mpl_move(int x, int y)
{
	int deltax, deltay;
	list<scrPt>::iterator it;

	/*
	 * find the closest point to move
	 */
	for (it = myPolyList.begin(); it != myPolyList.end(); it++) {
		deltax = (*it).x - x;
		deltay = (*it).y - y;

		if (isPointSelected(deltax, deltay)) {
			(*it).x = x; (*it).y = y;
			break; 
		}
	}
}

void mpl_refresh(int x, int y)
{
	myPolyList.clear();
}

void mpl_quit(int x, int y)
{
	myPolyList.clear();
	exit(0);
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

void mpl_linedraw(scrPt endPt1, scrPt endPt2)
{
   glBegin(GL_LINE_STRIP);
      glVertex2i(endPt1.x, endPt1.y);
      glVertex2i(endPt2.x, endPt2.y);
   glEnd();
}

/*
 * poly-lines plot once a time
 */
void mpl_linesdraw()
{
	list<scrPt>::iterator it, cur;

	/* (0,0) is marked point */
	for (it = myPolyList.begin(); it != myPolyList.end();) {

		/* get next node */
		cur = it; it++;

		if (it == myPolyList.end()) break;

		if ((*it).x == 0 && (*it).y == 0) { continue; }
		if ((*cur).x == 0 && (*cur).y == 0) { continue; }
		mpl_linedraw(*cur, *it);
	}
}



/*
 * repaint
 */
void mpl_polylines(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
   int x, y; /* mouse position */

   x = xMouse;
   y = winHeight - yMouse;
   
   /*
	* left button operation 
	*/
   if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
	   if (choosenkey < KEY_NIL) funcMap[choosenkey](x, y);
   }
   
   /* repaint */
   glClear(GL_COLOR_BUFFER_BIT);
   mpl_linesdraw();
   glFlush();
}

/*
 * key pressed and change state
 */
void mpl_keypress(unsigned char key, int x, int y)
{
	switch (key) {
	case 'b': case 'B':
		choosenkey = KEY_ADD;
		anotherPoly++;
		break;
	case 'd': case 'D':
		choosenkey = KEY_DEL;
		break;
	case 'm': case 'M':
		choosenkey = KEY_MOV;
		break;
	case 'r': case 'R':
		choosenkey = KEY_REF;
		break;
	case 'q': case 'Q':
		choosenkey = KEY_QUI;
		break;
	}
}

void mpl_funcInit()
{
	funcMap[KEY_ADD] = mpl_add;
	funcMap[KEY_DEL] = mpl_delete;
	funcMap[KEY_MOV] = mpl_move;
	funcMap[KEY_REF] = mpl_refresh;
	funcMap[KEY_QUI] = mpl_quit;
}

int main (int argc, char** argv)
{
   mpl_funcInit();

   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowPosition(100, 100);
   glutInitWindowSize(winWidth, winHeight);
   glutCreateWindow("Draw Interactive Polyline");
   init();
   glutDisplayFunc(displayFcn);
   glutReshapeFunc(winReshapeFcn);
   glutKeyboardFunc(mpl_keypress);
   glutMouseFunc(mpl_polylines);
   glutMainLoop();

   return 0;
}
