#include <Windows.h>
#include "glut.h"

int ModeFlag=0;
int ShadeFlag=0;
static const float vertex_list1[][3] = 
{ 
    -1.0f, -1.0f, -1.0f, 
    1.0f, -1.0f, -1.0f, 
    -1.0f, 1.0f, -1.0f, 
    1.0f, 1.0f, -1.0f, 
    -1.0f, -1.0f, 1.0f, 
    1.0f, -1.0f, 1.0f, 
    -1.0f, 1.0f, 1.0f, 
    1.0f, 1.0f, 1.0f, 
}; 

typedef struct ColorRGB
{
	GLfloat r;
	GLfloat g;
	GLfloat b;
};
const ColorRGB colors[4]=
{
	{135.0,226.0,56.0},
	{175.0,226.0,16.0},
	{35.0,216.0,26.0},
	{115.0,76.0,216.0},
};



static const GLint index_list[][2] = 
{ 
    {0, 1},    
    {2, 3},    
    {4, 5},    
    {6, 7},    
    {0, 2},    
    {1, 3},    
    {4, 6},    
    {5, 7},
    {0, 4},
    {1, 5},
    {7, 3},
    {2, 6}
}; 



void DrawCube()
{
	
	
   
	 int i,j;
    
	glBegin(GL_LINES); 
	
    for(i=0; i<12; ++i) 

    {
        for(j=0; j<2; ++j) 

        {
			glColor3ub(colors[i*j%3].r,colors[i*j%3].g,colors[i*j%3].b);//以255为最大
            glVertex3fv(vertex_list1[index_list[i][j]]);     
        }
    }
    glEnd();

	glPushMatrix();
	glTranslatef(5.0,0.0,0.0);
	glRotatef(45, 0, 0, 1);

	glBegin(GL_LINES);
    for(i=0; i<12; ++i) 

    {
        for(j=0; j<2; ++j) 

        {
			glColor3ub(colors[i*j%3].r,colors[i*j%3].g,colors[i*j%3].b);
            glVertex3fv(vertex_list1[index_list[i][j]]);     
        }
    }
	glEnd();
	glPopMatrix();
	
	glPushMatrix();
	glRotatef(45, 0, 0, 1);
	glTranslatef(5.0,0.0,0.0);
	glBegin(GL_LINES); 
	for(i=0; i<12; ++i) 

    {
        for(j=0; j<2; ++j) 

        {
			
			glColor3ub(colors[i*j%3].r,colors[i*j%3].g,colors[i*j%3].b);
            glVertex3fv(vertex_list1[index_list[i][j]]);     
        }
    }
	glEnd();
	glPopMatrix();
	glFlush();
}



void display(void) 
{
    glColor3f(1, 0, 0);
	DrawCube();
    glutSwapBuffers();
}
void init()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(90.0,1.0,0.0,20.0);
    //glFrustum(-1.0,1.0,-1.0,1.0,1.5,20);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0,0.0,10.0,0.0,0.0,0.0,0.0,1.0,0.0);
}

void main(int argc, char **argv) 
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(600,600);
    glutCreateWindow("sample");

	init();

    glutDisplayFunc(display);
    glutIdleFunc(display);

	glutMainLoop();
}

