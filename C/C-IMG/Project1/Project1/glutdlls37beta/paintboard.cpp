#include<GL/glut.h>
#include<stdlib.h>
#include<math.h>
#include<stdio.h>
#include<windows.h>

GLsizei wh = 600, ww = 800,wx,wy, wx1=800, wy1=600;/*display window size*/

char* image;

int count=0;
int sub_menu;

int draw; /* to store draw option*/
int i,j,k;
float a[100][2], b[100][2]; /*to store the vertices of all the polygons drawn*/
int m, n;
int pol;
int flag=0, flag1;
int fill=0;
float textx,texty, textz=0.0;
float a1=0, a2=0, a3=0, b1=0, b2=0, b3=0; /* store vertices of the polygon to be drawn */
int size=1; /* pixel size */

int save=0;


float posx, posy;
void *currentfont;

FILE *fptr;
char fname[20];
int s=0;
int wel=0;


void drawstring(float x,float y,float z,char *string) /* to display text */
{
	char *c;
	glRasterPos3f(x,y,z);

	for(c=string;*c!='\0';c++)
	{
		glutBitmapCharacter(currentfont,*c);
	}
}



void setFont(void *font)
{
	currentfont=font;
}




void drawpoint(int x, int y) /* to draw point */
{
	y=wh-y;

	if(x>wh/10+1 && wh/10<y && y<wh-31)
	{
		glPointSize(size);
		glBegin(GL_POINTS);
		glVertex2f(x, y);

		glEnd();

		glFlush();
	}
}




void paint(int x, int y) /* PAINT BRUSH function */
{
	y=wh-y;



	if(x>wh/10+1 && wh/10<y && y<wh-31)
	{
		glBegin(GL_POLYGON);
		glVertex2f(x, y);
		glVertex2f(x+4+size, y);
		glVertex2f(x+4+size, y+4+size);
		glVertex2f(x, y+4+size);
		glEnd();
		glFlush();
	}

}



void eraser(int x, int y) /* ERASER function */
{
	y=wh-y;

	glColor3f(1, 1, 1);

	if(x>wh/10+1 && wh/10<y && y<wh-31)
	{
		glBegin(GL_POLYGON);
		glVertex2f(x, y);
		glVertex2f(x+5, y);
		glVertex2f(x+5, y+5);
		glVertex2f(x, y+5);
		glEnd();
		glFlush();
	}

}



void reset() /* resets the variables in which vertices are stored,after a polygon is drawn */
{
	a1=0;
	a2=0;
	b1=0;
	b2=0;
	a3=0;
	b3=0;
}



void palette(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) /* to draw the COLOR PALETTE */
{
	glBegin(GL_QUADS);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glVertex2f(x3, y3);
	glVertex2f(x4, y4);
	glEnd();
}



void draw_pixel(GLfloat x, GLfloat y) /*to draw the points of a circle */
{
	if(x>wh/10+1 && wh/10<y && y<wh-31)
	{
		if(fill==1)
			glPointSize(3);
		else
			glPointSize(size);
		glBegin(GL_POINTS);
		glVertex2f(x, y);
		glEnd();
	}
}



void plotpixels(GLfloat p, GLfloat q, GLfloat x, GLfloat y)
{
	draw_pixel(x+p, y+q);
	draw_pixel(-x+p, y+q);
	draw_pixel(x+p, -y+q);
	draw_pixel(-x+p, -y+q);


	draw_pixel(y+p, x+q);
	draw_pixel(-y+p, x+q);
	draw_pixel(y+p, -x+q);
	draw_pixel(-y+p, -x+q);
}


void draw_circle(GLfloat p, GLfloat q, GLfloat r) /* to draw a CIRCLE using MIDPOINT CIRCLE DRAWING algorithm */
{
	GLfloat d=1-r, x=0, y=r;

	while(y>x)
	{
		plotpixels(p, q, x, y);
		if(d<0) d+=2*x+3;
		else
		{
			d+=2*(x-y)+5;
			--y;
		}
		++x;
	}
	plotpixels(p, q, x, y);
}


void draw_pixel1(GLfloat x, GLfloat y)
{

	glBegin(GL_POINTS);
	glVertex2f(x, y);
	glEnd();

}

void plotpixels1(GLfloat p, GLfloat q, GLfloat x, GLfloat y) /* to draw the circle option on the tool bar */
{
	draw_pixel1(x+p, y+q);
	draw_pixel1(-x+p, y+q);
	draw_pixel1(x+p, -y+q);
	draw_pixel1(-x+p, -y+q);


	draw_pixel1(y+p, x+q);
	draw_pixel1(-y+p, x+q);
	draw_pixel1(y+p, -x+q);
	draw_pixel1(-y+p, -x+q);
}

void draw_circle1(GLfloat p, GLfloat q, GLfloat r) /* to draw the circle option on the tool bar */
{
	GLint d=1-r, x=0, y=r;

	while(y>x)
	{
		plotpixels1(p, q, x, y);
		if(d<0) d+=2*x+3;
		else
		{
			d+=2*(x-y)+5;
			--y;
		}
		++x;
	}
	plotpixels1(p, q, x, y);
}




void edgedetect(float x1, float y1, float x2, float y2, int *le, int *re) /* to detect edges of the polygon to b e filled */
{
	float mx, x, temp;

	if((y2-y1)<0)
	{
		temp=y1;
		y1=y2;
		y2=temp;

		temp=x1;
		x1=x2;
		x2=temp;
	}

	if((y2-y1)!=0)
		mx=(x2-x1)/(y2-y1);
	else
		mx=x2-x1;

	x=x1;

	for(i=y1;i<=y2;i++)
	{
		if(x<(float)le[i])
			le[i]=(int)x;
		if(x>(float)re[i])
			re[i]=(int)x;
		x+=mx;
	}
}



void scanfill(GLint num1, GLint num2) /* to FILL a Polygon using SCAN LINE ALGORITHM*/
{
	int le[1000], re[1000];
	int p, q;
	int bottom, top;

	for(p=0;p<wh;p++)
	{
		le[p]=ww;
		re[p]=0;
	}

	for(p=num1;p<num2;p++)
	{
		edgedetect(a[p][0], a[p][1], a[p+1][0], a[p+1][1], le, re);
	}

	edgedetect(a[num2][0], a[num2][1], a[num1][0], a[num1][1], le, re);

	for(q=0;q<wh;q++)
		le[q]=le[q]+1;

	bottom=0;
	while(re[bottom]<le[bottom])
		bottom++;

	bottom++;

	top=wh-1;
	while(re[top]<le[top])
		top--;


	for(q=bottom;q<top;q++)
	{
		if(le[q]<=re[q])
		{
			for(p=(int)le[q];p<(int)re[q];p++)
				draw_pixel(p, q);
		}
	}
}


void detect_point(GLint num1, GLint num2, int x, int y) /* to detect which POLYGON TO BE FILLED */
{
	int le[1000], re[1000];
	int p, q;

	for(p=0;p<wh;p++)
	{
		le[p]=ww;
		re[p]=0;
	}

	for(p=num1;p<num2;p++)
	{
		edgedetect(a[p][0], a[p][1], a[p+1][0], a[p+1][1], le, re);
	}

	edgedetect(a[num2][0], a[num2][1], a[num1][0], a[num1][1], le, re);

	for(q=0;q<wh;q++)
	{
		if(le[q]<=re[q])
		{
			for(p=(int)le[q];p<(int)re[q];p++)
			{
				if(x==p && y==q)
					flag=num1;
			}
		}
	}
}



void display(void) /*DISPLAY function*/
{

	glClearColor (1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT); /*Clears color buffer*/




	if(wel==0 || wel==1)
	{



		/* Welcome Screen */


		glClearColor (0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);


		setFont(GLUT_BITMAP_HELVETICA_18);
		glColor3f(0, 1, 1);
		drawstring(22*wh/45, 42*wh/45 , 0.0, "A Mini Project On");

		drawstring(8*wh/45, 40*wh/45, 0.0, "DESIGN AND IMPLEMENTION OF GRAPHICS EDITOR");



		setFont(GLUT_BITMAP_HELVETICA_12);

		drawstring(25*wh/45, 32*wh/45, 0.0, "BY:");
		drawstring(8*wh/45, 29*wh/45, 0.0, "Name 1");
		drawstring(8*wh/45, 27*wh/45, 0.0, "usn");


		drawstring(35*wh/45, 29*wh/45, 0.0, "Name 2");
		drawstring(35*wh/45, 27*wh/45, 0.0, "usn");



		drawstring(20*wh/45, 20*wh/45, 0.0, "UNDER THE GUIDENCE OF:");

		drawstring(5*wh/45, 17*wh/45, 0.0, "Teacher");
		drawstring(5*wh/45, 15*wh/45, 0.0, "LECTURER, Dept. of CS&E");
		drawstring(5*wh/45, 13*wh/45, 0.0, "College");

		drawstring(38*wh/45, 17*wh/45, 0.0, "Teacher");
		drawstring(38*wh/45, 15*wh/45, 0.0, "LECTURER, Dept. of CS&E");
		drawstring(38*wh/45, 13*wh/45, 0.0, "College");


		wel=1;

		glFlush();
		glClearColor (1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);


	}


	if(wel==2)
	{

		count++;

		if(count>1)
		{

			if(ww!=0 && wh!=0)
			{ wx=wx1;
				wy=wy1;

				wx1=ww;
				wy1=wh;
			}

			glDrawBuffer(GL_FRONT); /* selecting GL_FRONT buffer to write*/
			glRasterPos2f(posx, posy);
			glDrawPixels(wx-wy/10-2, wy-wy/10-wy/20-1, GL_RGB, GL_UNSIGNED_BYTE, image); /* to write pixels i nto the selected buffer */

		}




		glColor3f(0.85, 0.85, 0.85);
		glPointSize(1);
		glLineWidth(1);
		glRectf(0,wh/10+1,wh/10,wh-(wh/20)-1); /* to draw the TOOL BAR */

		glRectf(0, 0, ww, wh/10); /* to draw COLOR AREA */

		glRectf(0,wh-(wh/20), ww, wh); /* to draw MENU BAR*/


		glColor3f(0.5, 0.5, 0.5);
		glBegin(GL_LINES);
		glVertex2f(wh/10, wh/10+2);
		glVertex2f(wh/10, wh-(wh/20)+2);
		glVertex2f(wh/10+1, wh/10+2);
		glVertex2f(wh/10+1, wh-(wh/20)+2);
		glEnd();

		glColor3f(0.5, 0.5, 0.5);
		glBegin(GL_LINES);
		glVertex2f(wh/10+1, wh-(wh/20));
		glVertex2f(ww, wh-(wh/20));
		glVertex2f(wh/10+1, wh-(wh/20)+1);
		glVertex2f(ww, wh-(wh/20)+1);
		glEnd();

		/* to draw COLOR PALETTE */

		glColor3f(0, 0, 0);
		palette(6*wh/60+1, wh/60, 6*wh/60+1, wh/20-1, 8*wh/60-1, wh/20-1, 8*wh/60-1, wh/60);

		glColor3f(1, 1, 1);
		palette(6*wh/60+1, wh/20+1, 6*wh/60+1, wh/12, 8*wh/60-1, wh/12, 8*wh/60-1, wh/20+1);


		glColor3f(0, 0, 1);
		palette(8*wh/60+1, wh/60, 8*wh/60+1, wh/20-1, 10*wh/60-1, wh/20-1, 10*wh/60-1, wh/60);


		glColor3f(1, 0, 1);
		palette(8*wh/60+1, wh/20+1, 8*wh/60+1, wh/12, 10*wh/60-1, wh/12, 10*wh/60-1, wh/20+1);

		glColor3f(1, 1, 0);
		palette(10*wh/60+1, wh/60, 10*wh/60+1, wh/20-1, 12*wh/60-1, wh/20-1, 12*wh/60-1, wh/60);

		glColor3f(1, 0, 0);
		palette(10*wh/60+1, wh/20+1, 10*wh/60+1, wh/12, 12*wh/60-1, wh/12, 12*wh/60-1, wh/20+1);

		glColor3f(0, 1, 0);
		palette(12*wh/60+1, wh/60, 12*wh/60+1, wh/20-1, 14*wh/60-1, wh/20-1, 14*wh/60-1, wh/60);


		glColor3f(0, 1, 1);
		palette(12*wh/60+1, wh/20+1, 12*wh/60+1, wh/12, 14*wh/60-1, wh/12, 14*wh/60-1, wh/20+1);


		glColor3f(0.5, 0.5, 0);
		palette(14*wh/60+1, wh/60, 14*wh/60+1, wh/20-1, 16*wh/60-1, wh/20-1, 16*wh/60-1, wh/60);


		glColor3f(0.1, 0.4, 0.6);
		palette(14*wh/60+1, wh/20+1, 14*wh/60+1, wh/12, 16*wh/60-1, wh/12, 16*wh/60-1, wh/20+1);

		glColor3f(0.4, 0.1, 0.1);
		palette(16*wh/60+1, wh/60, 16*wh/60+1, wh/20-1, 18*wh/60-1, wh/20-1, 18*wh/60-1, wh/60);


		glColor3f(0.9, 0.1, 0.5);
		palette(16*wh/60+1, wh/20+1, 16*wh/60+1, wh/12, 18*wh/60-1, wh/12, 18*wh/60-1, wh/20+1);


		glColor3f(0.5, 0.1, 0.5);
		palette(18*wh/60+1, wh/60, 18*wh/60+1, wh/20-1, 20*wh/60-1, wh/20-1, 20*wh/60-1, wh/60);


		glColor3f(0.3, 0.1, 0.5);
		palette(18*wh/60+1, wh/20+1, 18*wh/60+1, wh/12, 20*wh/60-1, wh/12, 20*wh/60-1, wh/20+1);

		glColor3f(0.0, 0.5, 0.1);
		palette(20*wh/60+1, wh/60, 20*wh/60+1, wh/20-1, 22*wh/60-1, wh/20-1, 22*wh/60-1, wh/60);


		glColor3f(0.9, 0.8, 0.0);
		palette(20*wh/60+1, wh/20+1, 20*wh/60+1, wh/12, 22*wh/60-1, wh/12, 22*wh/60-1, wh/20+1);


		glColor3f(0.7, 0.0, 0.0);
		palette(22*wh/60+1, wh/60, 22*wh/60+1, wh/20-1, 24*wh/60-1, wh/20-1, 24*wh/60-1, wh/60);


		glColor3f(0.5, 0.5, 0.5);
		palette(22*wh/60+1, wh/20+1, 22*wh/60+1, wh/12, 24*wh/60-1, wh/12, 24*wh/60-1, wh/20+1);

		/*end color palette */


		glColor3f(0, 0, 0);


		glBegin(GL_LINE_LOOP); /* to draw TOOL BOX */
		glVertex2f(2, wh-(wh/20)-1);
		glVertex2f(wh/10-2, wh-(wh/20)-1);
		glVertex2f(wh/10-2,14*wh/20);
		glVertex2f(2, 14*wh/20);
		glEnd();


		glBegin(GL_LINES);
		glVertex2f(wh/20, wh-(wh/20)-1);
		glVertex2f(wh/20, 14*wh/20);

		glVertex2f(2, 18*wh/20);
		glVertex2f(wh/10-2, 18*wh/20);

		glVertex2f(2, 17*wh/20);
		glVertex2f(wh/10-2, 17*wh/20);

		glVertex2f(2, 16*wh/20);
		glVertex2f(wh/10-2, 16*wh/20);

		glVertex2f(2, 15*wh/20);
		glVertex2f(wh/10-2, 15*wh/20);


		glEnd();



		glBegin(GL_LINES); /* to draw LINE OPTION on tool bar */
		glVertex2f(4*wh/60, 18.8*wh/20);
		glVertex2f(5*wh/60, 18.2*wh/20);
		glEnd();


		glBegin(GL_LINE_LOOP); /* to draw TRIANGLE OPTION on tool bar */
		glVertex2f(wh/40, 17.8*wh/20);
		glVertex2f(wh/120, 17.2*wh/20);
		glVertex2f(5*wh/120, 17.2*wh/20);
		glEnd();

		glBegin(GL_LINE_LOOP); /* to draw RECTANGLE OPTION on tool bar */
		glVertex2f(7*wh/120, 17.8*wh/20);
		glVertex2f(11*wh/120-2, 17.8*wh/20);
		glVertex2f(11*wh/120-2, 17.2*wh/20);
		glVertex2f(7*wh/120, 17.2*wh/20);
		glEnd();


		glBegin(GL_TRIANGLES); /* to draw PENCIL OPTION on tool bar */
		glVertex2f(3*wh/120-1, 18.10*wh/20);
		glVertex2f(wh/60+2, 18.3*wh/20);
		glVertex2f(2*wh/60-4, 18.3*wh/20);
		glEnd();

		glBegin(GL_LINE_LOOP);
		glVertex2f(wh/60+2, 18.3*wh/20);
		glVertex2f(wh/60+2, 18.8*wh/20);
		glVertex2f(2*wh/60-4, 18.8*wh/20);
		glVertex2f(2*wh/60-4, 18.3*wh/20);
		glEnd();

		glColor3f(1, 1, 1);
		glBegin(GL_QUADS); /* to draw ERASER OPTION on tool bar */
		glVertex2f(4*wh/60, 15.7*wh/20);
		glVertex2f(5*wh/60, 15.7*wh/20);
		glVertex2f(5*wh/60, 15.3*wh/20);
		glVertex2f(4*wh/60, 15.3*wh/20);
		glEnd();

		glColor3f(0, 0, 0);

		glBegin(GL_LINE_LOOP); /* to draw PENCIL OPTION on tool bar */
		glVertex2f(wh/120, 16.2*wh/20);
		glVertex2f(5*wh/120, 16.2*wh/20);
		glVertex2f(5*wh/120, 16.4*wh/20);
		glVertex2f(3*wh/120, 16.4*wh/20);
		glVertex2f(4*wh/120, 16.8*wh/20);
		glVertex2f(2*wh/120, 16.8*wh/20);
		glEnd();


		draw_circle1(9*wh/120, 16.5*wh/20, wh/60); /* to draw CIRCLE OPTION on tool bar */



		for(i=0;i<40;i++) /* to draw AIR BRUSH OPTION on tool bar */
		{
			j=rand()%15;
			k=rand()%15;
			glBegin(GL_POINTS);
			glVertex2f(wh/120+j,15.8*wh/20-k);
			glEnd();
		}

		glColor3f(0, 0.3, 1);
		glBegin(GL_QUADS); /* to draw COLOUR FILL OPTION on tool bar */
		glVertex2f(2*wh/120, 14.6*wh/20);
		glVertex2f(3*wh/120+1, 14.2*wh/20);
		glVertex2f(5*wh/120-2, 14.35*wh/20);
		glVertex2f(4*wh/120-4, 14.75*wh/20);
		glEnd();
		glColor3f(0, 0, 0);

		glPointSize(3);
		glBegin(GL_POINTS);
		glVertex2f(2*wh/120-2, 14.6*wh/20-2);
		glEnd();

		glPointSize(1.5);
		glBegin(GL_POINTS);
		glVertex2f(2*wh/120-2, 14.6*wh/20-6);
		glEnd();

		glPointSize(1);


		glColor3f(0.4, 0.1, 0.1);
		glBegin(GL_QUADS); /* to draw BRUSH OPTION on tool bar */
		glVertex2f(7*wh/120, 14.45*wh/20);
		glVertex2f(10*wh/120, 14.45*wh/20);
		glVertex2f(10*wh/120, 14.55*wh/20);
		glVertex2f(7*wh/120, 14.55*wh/20);
		glEnd();


		glColor3f(0, 0, 0);

		glBegin(GL_LINES);
		glVertex2f(10*wh/120, 14.45*wh/20);
		glVertex2f(11*wh/120, 14.4*wh/20);


		glVertex2f(10*wh/120, 14.55*wh/20);
		glVertex2f(11*wh/120, 14.6*wh/20);

		glVertex2f(10*wh/120, 14.47*wh/20);
		glVertex2f(11*wh/120, 14.47*wh/20);

		glVertex2f(10*wh/120, 14.53*wh/20);
		glVertex2f(11*wh/120, 14.53*wh/20);

		glEnd();



		setFont(GLUT_BITMAP_HELVETICA_12); /* to display text */
		glColor3f(0, 0, 1);
		drawstring(26*wh/60,wh/20,0.0,"Click the right mouse button to change pixel size or quit");

		glColor3f(0, 0, 0);
		drawstring(6*wh/60, 58*wh/60, 0.0, "Open");

		drawstring(12*wh/60, 58*wh/60, 0.0, "Save");

		drawstring(18*wh/60, 58*wh/60, 0.0, "Clear");




		glFlush();


		glReadBuffer(GL_FRONT);


		glReadPixels(wh/10+2, wh/10, ww-wh/10-2, wh-wh/10-wh/20-1,GL_RGB, GL_UNSIGNED_BYTE, image); /* to read pixels from buffer to memory */

		posx=wh/10+2;
		posy=wh/10;
	}
}






void keys(unsigned char key, int x, int y) /* to get FILENAME from the KEYBOARD */
{
	if(key=='a'||key=='b'||key=='c'||key=='d'||key=='e'||key=='f'||key=='g'||key=='h'||key=='i'||key=='j'||key=='k'||key=='l'||key=='m')
	{
		fname[s]=key;

		glRasterPos3f (textx, texty,textz);

		glColor3f(0, 0, 0);
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, fname[s]); /* to display FILENAME */
		textx=textx+10;

		s++;
	}

	if(key=='n'||key=='o'||key=='p'||key=='q'||key=='r'||key=='s'||key=='t'||key=='u'||key=='v'||key=='w'||key=='x'||key=='y'||key=='z')
	{
		fname[s]=key;

		glRasterPos3f (textx, texty,textz);
		glColor3f(0, 0, 0);

		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, fname[s]);
		textx=textx+10;


		s++;
	}

	if(key=='A'||key=='B'||key=='C'||key=='D'||key=='E'||key=='F'||key=='G'||key=='H'||key=='I'||key=='J'||key=='K'||key=='L'||key=='M')
	{
		fname[s]=key;

		glRasterPos3f (textx, texty,textz);

		glColor3f(0, 0, 0);
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, fname[s]);
		textx=textx+10;
		s++;
	}

	if(key=='N'||key=='O'||key=='P'||key=='Q'||key=='R'||key=='S'||key=='T'||key=='U'||key=='V'||key=='W'||key=='X'||key=='Y'||key=='Z')
	{
		fname[s]=key;
		glRasterPos3f (textx, texty,textz);
		glColor3f(0, 0, 0);

		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, fname[s]);
		textx=textx+10;



		s++;
	}

	if(key=='_'||key=='0'||key=='1'||key=='2'||key=='3'||key=='4'||key=='5'||key=='6'||key=='7'||key=='8'||key=='9')
	{
		fname[s]=key;
		glRasterPos3f (textx, texty,textz);

		glColor3f(0, 0, 0);
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, fname[s]);
		textx=textx+10;



		s++;
	}
	glFlush();



	if(key=='\r' && save==1) /* to SAVE current image on a file */
	{


		fname[s]='\0';




		if ((fptr = fopen(fname,"w")) == NULL)
		{
			fprintf(stderr,"Failed to open file\n");
			return;
		}

		save=0;
		s=0;

		glColor3f(0.85, 0.85, 0.85);
		glRectf(0,wh-(wh/20), ww, wh);

		setFont(GLUT_BITMAP_HELVETICA_12);
		glColor3f(0, 0, 0);
		drawstring(6*wh/60, 58*wh/60, 0.0, "Open");

		drawstring(12*wh/60, 58*wh/60, 0.0, "Save");

		drawstring(18*wh/60, 58*wh/60, 0.0, "Clear");

		fwrite(image,ww*wh*3,1,fptr);

		fclose(fptr);

	}



	if(key=='\r' && save==2)
	{ /* to OPEN an existing image file */

		fname[s]='\0';


		if ((fptr = fopen(fname,"r")) == NULL)
		{
			fprintf(stderr,"Failed to open file\n");
			return;
		}



		save=0;
		s=0;

		fread(image,ww*wh*3,1,fptr);

		fclose(fptr);

		display();
	}

}



void myMouse(int btn, int state, int x, int y)
{
	GLfloat r, r1, r2;
	int num1=0, num2=0;

	int i1, i2;
	int flag2;

	flag2=0;


	if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
	{
		/* to SELECT A COLOR */


		if(wel==1)
		{
			wel=2;
			display();
		}


		if(6*wh/60<x && x<8*wh/60 && wh/60<wh-y && wh-y<wh/20)
			glColor3f(0, 0, 0);


		else if(6*wh/60<x && x<8*wh/60 && wh/20<wh-y && wh-y<wh/12)
			glColor3f(1, 1, 1);


		else if(8*wh/60<x && x<10*wh/60 && wh/60<wh-y && wh-y<wh/20)
			glColor3f(0, 0, 1);


		else if(8*wh/60<x && x<10*wh/60 && wh/20<wh-y && wh-y<wh/12)
			glColor3f(1, 0, 1);


		else if(10*wh/60<x && x<12*wh/60 && wh/60<wh-y && wh-y<wh/20)
			glColor3f(1, 1, 0);


		else if(10*wh/60<x && x<12*wh/60 && wh/20<wh-y && wh-y<wh/12)
			glColor3f(1, 0, 0);


		else if(12*wh/60<x && x<14*wh/60 && wh/60<wh-y && wh-y<wh/20)
			glColor3f(0, 1, 0);


		else if(12*wh/60<x && x<14*wh/60 && wh/20<wh-y && wh-y<wh/12)
			glColor3f(0, 1, 1);


		else if(14*wh/60<x && x<16*wh/60 && wh/60<wh-y && wh-y<wh/20)
			glColor3f(0.5, 0.5, 0);

		else if(14*wh/60<x && x<16*wh/60 && wh/20<wh-y && wh-y<wh/12)
			glColor3f(0.1, 0.4, 0.6);

		else if(16*wh/60<x && x<18*wh/60 && wh/60<wh-y && wh-y<wh/20)
			glColor3f(0.4, 0.1, 0.1);

		else if(16*wh/60<x && x<18*wh/60 && wh/20<wh-y && wh-y<wh/12)
			glColor3f(0.9, 0.1, 0.5);


		else if(18*wh/60<x && x<20*wh/60 && wh/60<wh-y && wh-y<wh/20)
			glColor3f(0.5, 0.1, 0.5);

		else if(18*wh/60<x && x<20*wh/60 && wh/20<wh-y && wh-y<wh/12)
			glColor3f(0.3, 0.1, 0.5);

		else if(20*wh/60<x && x<22*wh/60 && wh/60<wh-y && wh-y<wh/20)
			glColor3f(0.0, 0.5, 0.1);

		else if(20*wh/60<x && x<22*wh/60 && wh/20<wh-y && wh-y<wh/12)
			glColor3f(0.9, 0.8, 0.0);


		else if(22*wh/60<x && x<24*wh/60 && wh/60<wh-y && wh-y<wh/20)
			glColor3f(0.7, 0.0, 0.0);


		else if(22*wh/60<x && x<24*wh/60 && wh/20<wh-y && wh-y<wh/12)
			glColor3f(0.5, 0.5, 0.5);


		/*end select color */


		/* to select WHAT TO DRAW */

		if(2<x && x<wh/20 && 18*wh/20<wh-y && wh-y<wh-(wh/20)-1) /* selected option is PENCIL*/
			draw=1;


		else if(wh/20<x && x<wh/10-2 && 18*wh/20<wh-y && wh-y<wh-(wh/20)-1) /* selected option is LINE */
		{

			reset();
			draw=2;
		}


		else if(2<x && x<wh/20 && 17*wh/20<wh-y && wh-y<18*wh/20) /* selected option i s TRIANGLE */

		{
			reset();
			draw=3;
		}


		else if(wh/20<x && x<wh/10-2 && 17*wh/20<wh-y && wh-y<18*wh/20) /* selected option is RECTANGLE */
		{

			reset();
			draw=4;
		}


		else if(2<x && x<wh/20 && 16*wh/20<wh-y && wh-y<17*wh/20) /* selected option is POLYGON */
		{

			reset();
			draw=5;
		}


		else if(wh/20<x && x<wh/10-2 && 16*wh/20<wh-y && wh-y<17*wh/20) /* selected option is CIRCLE */
		{
			reset();
			draw=6;
		}


		else if(2<x && x<wh/20 && 15*wh/20<wh-y && wh-y<16*wh/20) /* selected option is AIRBRUSH */
		{

			reset();
			draw=7;
		}


		else if(wh/20<x && x<wh/10-2 && 15*wh/20<wh-y && wh-y<16*wh/20) /* selected option is ERASER */
		{
			reset();
			draw=8;
		}



		else if(2<x && x<wh/20 && 14*wh/20<wh-y && wh-y<15*wh/20) /* selected option is COLOR FILL */
		{

			reset();
			draw=9;
		}


		else if(wh/20<x && x<wh/10-2 && 14*wh/20<wh-y && wh-y<15*wh/20) /* selected option is PAINT BRUSH */
		{
			reset();
			draw=10;
		}





		if(draw==1) /* to draw using a PENCIL */
		{
			if(pol==1)
			{
				m++;
				a[m][0]='$';
				a[m][1]='$';

				pol=0;
			}

			if(x>wh/10+1 && wh/10<wh-y && wh-y<wh-31)
				glutMotionFunc(drawpoint);

		}


		else if(draw==2) /* to draw a LINE */
		{
			if(pol==1)
			{
				m++;
				a[m][0]='$';
				a[m][1]='$';

				pol=0;
			}


			if(x>wh/10+1 && wh/10<wh-y && wh-y<wh-31)
			{
				drawpoint(x, y);

				a2=a1;
				b2=b1;

				a1=x;
				b1=wh-y;


				if(a1>wh/10+1 && wh/10<b1 && b1<wh-31)
				{
					if(a2>wh/10+1 && wh/10<b2 && b2<wh-31)
					{
						glLineWidth(size);
						glBegin(GL_LINES);
						glVertex2f(a2, b2);
						glVertex2f(a1, b1);
						glEnd();

						reset();

					}
				}
			}

		}


		else if(draw==3) /* to draw a TRIANGLE */
		{
			if(pol==1)
			{
				m++;
				a[m][0]='$';
				a[m][1]='$';

				pol=0;
			}



			if(x>wh/10+1 && wh/10<wh-y && wh-y<wh-31)
			{
				drawpoint(x, y);


				a3=a2;
				b3=b2;

				a2=a1;
				b2=b1;

				a1=x;
				b1=wh-y;





				if(a1>wh/10+1 && wh/10<b1 && b1<wh-31)
				{
					if(a2>wh/10+1 && wh/10<b2 && b2<wh-31)
					{
						if(a3>wh/10+1 && wh/10<b3 && b3<wh-31)

						{
							glLineWidth(size);
							glBegin(GL_LINE_LOOP);
							glVertex2f(a3, b3);
							glVertex2f(a2, b2);
							glVertex2f(a1, b1);
							glEnd();

							m++;
							a[m][0]=a3;
							a[m][1]=b3;

							m++;
							a[m][0]=a2;
							a[m][1]=b2;

							m++;
							a[m][0]=a1;
							a[m][1]=b1;

							m++;
							a[m][0]='$';
							a[m][1]='$';






							reset();
						}
					}
				}
			}

		}


		else if(draw==4) /* to draw a RECTANGLE */
		{

			if(pol==1)
			{
				m++;
				a[m][0]='$';
				a[m][1]='$';

				pol=0;
			}

			if(x>wh/10+1 && wh/10<wh-y && wh-y<wh-31)
			{
				drawpoint(x, y);

				a2=a1;
				b2=b1;

				a1=x;
				b1=wh-y;


				if(a1>wh/10+1 && wh/10<b1 && b1<wh-31)
				{
					if(a2>wh/10+1 && wh/10<b2 && b2<wh-31)
					{
						glLineWidth(size);
						glBegin(GL_LINE_LOOP);
						glVertex2f(a2, b2);
						glVertex2f(a1, b2);
						glVertex2f(a1, b1);
						glVertex2f(a2, b1);
						glEnd();


						m++;
						a[m][0]=a2;
						a[m][1]=b2;

						m++;
						a[m][0]=a1;
						a[m][1]=b2;

						m++;
						a[m][0]=a1;
						a[m][1]=b1;


						m++;
						a[m][0]=a2;
						a[m][1]=b1;

						m++;
						a[m][0]='$';
						a[m][1]='$';


						reset();

					}
				}
			}

		}


		else if(draw==5) /* to draw a POLYGON */
		{

			pol=1;
			if(x>wh/10+1 && wh/10<wh-y && wh-y<wh-31)
			{
				drawpoint(x, y);

				a2=a1;
				b2=b1;

				a1=x;
				b1=wh-y;

				m++;
				a[m][0]=x;
				a[m][1]=wh-y;




				if(a1>wh/10+1 && wh/10<b1 && b1<wh-31)
				{
					if(a2>wh/10+1 && wh/10<b2 && b2<wh-31)
					{
						glLineWidth(size);
						glBegin(GL_LINES);
						glVertex2f(a2, b2);
						glVertex2f(a1, b1);
						glEnd();
					}
				}

			}



		}

		else if(draw==6) /* to draw a CIRCLE */
		{
			if(pol==1)
			{
				m++;
				a[m][0]='$';
				a[m][1]='$';

				pol=0;
			}


			if(x>wh/10+1 && wh/10<wh-y && wh-y<wh-31)
			{
				drawpoint(x, y);

				a2 = a1;
				b2 = b1;

				a1 = x;
				b1 = wh - y;

				if (a1>wh/10+1 && wh/10 < b1 && b1 < wh - 31) {
					if (a2 > wh/10+1 && wh/10 < b2 && b2 < wh - 31) {
						r = sqrt((a1-a2)*(a1-a2) + (b1-b2)*(b1-b2));
						draw_circle(a2, b2, r);

						n++;
						b[n][0] = a2;
						b[n][1] = b2;

						n++;
						b[n][0] = a1;
						b[n][2] = b1;

						n++;
						b[n][0] = '$';
						b[n][1] = '$';

						reset();
					}
				}
			}
		}
		else if (draw == 7) /* to draw AIRBRUSH */
		{
			if (pol == 1) {
				m++;
				a[m][0] = '$';
				a[m][1] = '$';

				pol = 0;
			}

			if (x > wh/10+1 && wh/10 < wh - y && wh - y < wh - 31) {
				for (i = 0; i < 60; i++) {
					j = rand()%17;
					k = rand()%17;
					drawpoint(x+j, y+k);
				}
			}
		}
		else if (draw == 9) /* to FILL A POLYGON with the selected color */
		{
			if (pol == 1) {
				m++;
				a[m][0] = '$';
				a[m][1] = '$';

				pol = 0;
			}

			if(x>wh/10+1 && wh/10<wh-y && wh-y<wh-31)
			{
				y=wh-y;


				flag1=0;
				for(i=1;i<=m;i++)
				{
					num2=i-1;
					for(i1=i;a[i1][0]!='$';i1++)
					{
						num2++;
					}
					num1=i;

					detect_point(num1, num2, x, y);

					if(flag!=0)
					{
						scanfill(num1, num2);
						flag=0;
						flag1=1;
					}
					i=num2+1;

				}

				if(flag1==0 && flag2==0)
				{

					for(i=1;i<=n;i++)
					{
						num2=i-1;
						for(i1=i;b[i1][0]!='$';i1++)
						{
							num2++;
						}
						num1=i;

						r1=sqrt((b[num1][0]-b[num2][0])*(b[num1][0]-b[num2][0])+(b[num1][1]-b[num2][1])*(b[num1][1]-b[num2][1]));
						r2=sqrt((b[num1][0]-x)*(b[num1][0]-x)+(b[num1][1]-y)*(b[num1][1]- y));


						if(r2<r1)
						{



							for(i2=0;i2<r1;i2=i2+2)
							{
								fill=1;
								draw_circle(b[num1][0], b[num1][1], i2);
							}
							fill=0;

							flag2=1;
						}

						i=num2+1;
					}
				}

			}
		}




		if(draw==8) /* to ERASE */

		{
			if(pol==1)
			{
				m++;
				a[m][0]='$';
				a[m][1]='$';

				pol=0;
			}

			if(x>wh/10+1 && wh/10<wh-y && wh-y<wh-31)
				glutMotionFunc(eraser);
		}

		else if(draw==10)
		{
			if(pol==1)
			{
				m++;
				a[m][0]='$';
				a[m][1]='$';

				pol=0;
			}


			if(x>wh/10+1 && wh/10<wh-y && wh-y<wh-31)

				glutMotionFunc(paint);

		}





		else if(x>wh/10+1 && wh/10<wh-y && wh-y<wh-31)
			glutMotionFunc(drawpoint);


		if(x>=6*wh/60 && x<=12*wh/60 && wh-y>=57*wh/60 && wh-y<=wh) /* to OPEN an existing FILE */
		{

			save=2;

			setFont(GLUT_BITMAP_9_BY_15);
			glColor3f(0, 0, 0);

			drawstring(40*wh/60, 58*wh/60, 0.0, "Enter filename: ");
			textx=60*wh/60;
			texty=58*wh/60;




		}



		else if(x>=12*wh/60 && x<=18*wh/60 && wh-y>=57*wh/60 && wh-y<=wh) /* to SAVE the current image on to a FILE */
		{
			save=1;

			setFont(GLUT_BITMAP_9_BY_15);
			glColor3f(0, 0, 0);

			drawstring(40*wh/60, 58*wh/60, 0.0, "Enter filename: ");
			textx=60*wh/60;
			texty=58*wh/60;



		}


		else if(x>=18*wh/60 && x<=24*wh/60 && wh-y>=57*wh/60 && wh-y<=wh) /* to CLEAR the DRAWING AREA */
		{

			for(i=1;i<=m;i++)
				a[i][0]=a[i][1]=0;




			for(i=1;i<=n;i++)
				b[i][0]=b[i][1]=0;

			m=n=0;


			count=0;
			display();
		}




		palette(wh/30, 2*wh/30, 2*wh/30, 2*wh/30, 2*wh/30, wh/30, wh/30, wh/30); /* to display SELECTED COLOUR */

		glFlush();


		glReadBuffer(GL_FRONT);


		glReadPixels(wh/10+2, wh/10, ww-wh/10-2, wh-wh/10-wh/20-1,GL_RGB, GL_UNSIGNED_BYTE, image);

		posx=wh/10+2;
		posy=wh/10;



	}

	if(btn==GLUT_LEFT_BUTTON && state==GLUT_UP)
	{
		glReadBuffer(GL_FRONT);


		glReadPixels(wh/10+2, wh/10, ww-wh/10-2, wh-wh/10-wh/20-1,GL_RGB, GL_UNSIGNED_BYTE, image);

		posx=wh/10+2;
		posy=wh/10;
	}
}


void myReshape(GLsizei w, GLsizei h) /* RESHAPE FUNCTION */
{

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (GLdouble)w, 0.0, (GLdouble)h, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);

	glViewport(0,0,w,h);

	glFlush();


	ww = w;
	wh = h;


	glutPostRedisplay();
}


void point_size(int id) /* to CHANGE THE PIXEL SIZE */
{
	switch(id)
	{
		case 1:size++;
			   break;

		case 2:if(size>1)
				   size--;
			   break;
	}
}


void right(int id) /* to QUIT the editor */
{
	if(id==2)
		exit(0);
}

int main(int argc, char **argv)
{

	image = (char *)malloc(3*1450*900*sizeof(char)); /* to ALLOCATE MEMORY required to SAVE the file */


	a[0][0]='$';
	a[0][1]='$';
	m=0;

	b[0][0]='$';
	b[0][1]='$';
	n=0;


	glutInit(&argc,argv);
	glutInitDisplayMode (GLUT_SINGLE| GLUT_RGB);

	glutInitWindowSize(ww,wh);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("graphics editor");

	/*TO CREATE MENU */
	sub_menu=glutCreateMenu(point_size);
	glutAddMenuEntry("increase", 1);
	glutAddMenuEntry("decrease", 2);



	glutCreateMenu(right);
	glutAddSubMenu("Point Size", sub_menu);
	glutAddMenuEntry("Quit",2);

	glutAttachMenu(GLUT_RIGHT_BUTTON);


	glutReshapeFunc(myReshape);

	glutDisplayFunc(display);

	glutMouseFunc(myMouse);
	glutKeyboardFunc(keys);


	glutMainLoop();
}

