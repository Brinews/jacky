// student name: 
// student ID: 
// date: 

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
GLint ploypoint = 0;
GLint ployline=-1;
GLint height;
GLint choose=0;
GLint move_ployline,move_ploypoint;
struct Point
{
	int x;
	int y;
};
struct GLintPointArray
{
	Point point[60];
};
GLintPointArray ploy[60];
void drawPointLine()
{
	for (int i=0;i<=ployline;i++)
	{
		glBegin(GL_LINE_STRIP);
		for(int k=0;k<60;k++)
		{
			if (ploy[i].point[k].x!=0&&ploy[i].point[k].y!=0)
				glVertex2i(ploy[i].point[k].x,ploy[i].point[k].y);
		}
		glEnd();
	}
}
void Mouse(int button, int state, int x, int y)
{
	Point temp;
	if (state==GLUT_DOWN)
		if (button==GLUT_LEFT_BUTTON)
		{
			printf("X = %4.1d , Y = %4.1d \n",x,height-y);
			temp.x=x;
			temp.y=height-y;
			switch (choose)
			{
				case 1:
					ploy[ployline].point[ploypoint].x=temp.x;
					ploy[ployline].point[ploypoint].y=temp.y;
					ploypoint++;
					break;
				case 2:
					{
						for (int i=0;i<60;i++)
						{
							for (int k=0;k<60;k++)
							{       
								int delx=ploy[i].point[k].x-temp.x;
								int dely=ploy[i].point[k].y-temp.y;
								if((delx < 8 && delx > -8)&&(dely < 8 && dely > -8))
								{
									for(int d=k;d<60;d++)
										if (d==59)
											ploy[i].point[d].x=ploy[i].point[d].y=0;
										else
										{
											ploy[i].point[d].x=ploy[i].point[d+1].x;
											ploy[i].point[d].y=ploy[i].point[d+1].y;
										}
									break;
								}
							}
						}
					}break;
				case 3:
					{
						int i,k;
						for (i=0;i<60;i++)
						{
							for(k=0;k<60;k++)
							{
								int move_x=ploy[i].point[k].x-temp.x;
								int move_y=ploy[i].point[k].y-temp.y;
								if((move_x < 8 && move_x > -8)&&(move_y < 8 && move_y > -8))
								{
									move_ployline=i;
									move_ploypoint=k;
									break;
								}
							}
						}
						if (!(i<60 && k<60))
						{
							if (!(move_ployline==-1&&move_ploypoint==-1))
							{
								ploy[move_ployline].point[move_ploypoint].x=temp.x;
								ploy[move_ployline].point[move_ploypoint].y=temp.y;
							}
						}
					}break;
				default:
					break;
			}
		}
	glClear(GL_COLOR_BUFFER_BIT);
	drawPointLine();
	glFlush();
}
void key(unsigned char key, int x, int y)
{
	switch (key) {
		case 'b':
		case 'B':
			choose=1;
			ployline++;
			ploypoint=0;
			printf("Draw begining\n");
			break;
		case 'd':
		case 'D':
			choose=2;
			printf("delete point.\n");
			break;
		case 'm':
		case 'M':
			choose=3;
			move_ployline=move_ploypoint=-1;
			printf("moveing point.\n");
			break;
		case 'r':
		case 'R':
			glClear(GL_COLOR_BUFFER_BIT);
			for (int i=0;i<60;i++)
				for(int k=0;k<60;k++)
					ploy[i].point[k].x=ploy[i].point[k].y=0;
			move_ployline=move_ploypoint=-1;
			ployline=-1;
			ploypoint=0;
			choose=0;
			printf("Re-initialized.\n");
			glFlush();
			break;
		case 'q':
		case 'Q':
		case 27:
			exit(0);
	}
}
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush(); 
}
void init()
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);
}
void reshape(GLsizei w, GLsizei h)
{
	height=(GLint)h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w , 0, h); 
	glMatrixMode(GL_PROJECTION);
	glFlush();
}
int main(int argc, char** argv)
{

	glutInit(&argc,argv); 
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);  
	glutInitWindowSize(500,500);
	glutInitWindowPosition(0,0);
	glutCreateWindow("simple");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(key);
	glutMouseFunc(Mouse);
	init();
	glutMainLoop();
	return 0;
}

