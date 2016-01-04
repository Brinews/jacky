/*
 *  Project2: Animation of the Solor System 
 *  Project2.cpp
 * 
 * -------------------------------------------------------------
 *  Student Information
 * -------------------------------------------------------------
 *  Student Name:
 *  Student ID:
 *  E-mail:
 * -------------------------------------------------------------
 */
#include <GL\glut.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "Bitmap.h"

#define M_PI    3.1415926
#define M_2PI   2 * M_PI
#define M_PI_2  M_PI / 2

#define PI_RAID		360.0
#define YEAR_DAY 	365
#define DAY_HOUR 	24

#define SOLAR_YEAR	     200	// Rotation Period of the solar system
#define MERCURY_YEAR     360	// Rotation Period of the MERCURY
#define EARTH_YEAR 	     4320   // Rotation Period of the Earth
#define VENUS_YEAR 	     2880   // Rotation Period of the Venus
#define MARS_YEAR 	     8640   // Rotation Period of the Mars
#define MOON_YEAR 	     360	// Rotation Period of the Moon	
#define JUPITER_YEAR     40000	// Rotation Period of the Jupiter
#define SATURN_YEAR      120000 // Rotation Period of the Saturn
#define URANUS_YEAR      36000 // Rotation Period of the Uranus
#define NEPTUNE_YEAR     720000 // Rotation Period of the Neptune
#define PLUTO_YEAR 	     360	// Rotation Period of the Pluto

#define MERCURY_DAY      66		// Self-rotation Period of the Mercury
#define EARTH_DAY 	     11		// Self-rotation Period of the Earth	
#define VENUS_DAY 	     66		// Self-rotation Period of the Venus	
#define MARS_DAY 	     11 	// Self-rotation Period of the Mars	
#define JUPITER_DAY      4		// Self-rotation Period of the Jupiter
#define SATURN_DAY 	     5	    // Self-rotation Period of the Saturn	
#define URANUS_DAY 	     8	    // Self-rotation Period of the Uranus
#define NEPTUNE_DAY      8		// Self-rotation Period of the Neptune
#define PLUTO_DAY 	     66	    // Self-rotation Period of the Pluto

#define	SUN_RADIUS		 0.10	// Radius of the Sun
#define	MERCURY_RADIUS   0.06	// Radius of the Mecury
#define	VENUS_RADIUS	 0.25	// Radius of the Venus
#define	EARTH_RADIUS	 0.25	// Radius of the Earth
#define	MOON_RADIUS		 0.08	// Radius of the Moon
#define	MARS_RADIUS		 0.15	// Radius of the Mars
#define	JUPITER_RADIUS	 0.75	// Radius of the Jupiter
#define	SATURN_RADIUS	 0.65	// Radius of the Saturn
#define	URANUS_RADIUS	 0.40	// Radius of the Uranus
#define	NEPTUNE_RADIUS	 0.30	// Radius of the Neptune
#define	PLUTO_RADIUS	 0.04	// Radius of the Pluto

#define	MERCURY_ORBIT	 0.25	// Orbit Radius of the MERCURY
#define	VENUS_ORBIT		 0.75	// Orbit Radius of the Venus
#define	EARTH_ORBIT		 1.75	// Orbit Radius of the Earth
#define	MOON_ORBIT		 0.35	// Orbit Radius of the Moon
#define	MARS_ORBIT		 2.5	// Orbit Radius of the Mars
#define	JUPITER_ORBIT	 3.8	// Orbit Radius of the Jupiter
#define	SATURN_ORBIT	 6.5	// Orbit Radius of the Saturn
#define	URANUS_ORBIT	 8.5	// Orbit Radius of the Uranus
#define	NEPTUNE_ORBIT	 10.5	// Orbit Radius of the Neptune
#define	PLUTO_ORBIT		 12.0	// Orbit Radius of the Pluto

#define PRECESSION_YEAR  20 	// Self-rotation Period of the Earth Precession
#define PRECESSION_ANGLE 30
#define MAX_STAR 1000

#define TRUE  1
#define FALSE 0

typedef struct tmp_star {
   float pos[3];
   float col[4];
} Star;

Star star[MAX_STAR];

GLuint textureEarth = -1, textureMars = -1, textureVenus = -1,
		 textureMoon = -1;

GLUquadricObj *sun, *mercury, *earth, *moon, *venus, *mars;
GLUquadricObj *jupiter, *saturn, *uranus, *neptune, *pluto;

float angle_global = 0;
float year_mercury, year_earth = 0, year_venus = 0, year_mars = 0, year_moon = 0;
float year_jupiter = 0, year_saturn = 0, year_uranus = 0, year_neptune = 0, year_pluto = 0;
float day_mercury = 0, day_earth = 0, day_venus = 0, day_mars = 0;
float day_jupiter = 0, day_saturn = 0, day_uranus = 0, day_neptune = 0, day_pluto = 0;

static float incStep = 0.8;
static GLenum globalR = GL_TRUE;
static GLenum selfR = GL_TRUE;

float precession = 0;

GLfloat Rotate_x = 0, Rotate_y = 60;
GLfloat Translate_x = 0.0, Translate_y = 0.0;
GLfloat Zoom = 0.0, Scale = 1.0;
GLfloat dx, dy, old_x = 0.0, old_y = 0.0;
GLfloat old_dx = 0, old_dy = 0;
GLfloat dr, theta = 0;
GLfloat new_x, new_y;
int G_mousedown_L = FALSE, G_mousedown_R = FALSE;

float winwidth, winheight;
int _ScreenMode = 3;

void TransformXY(int ix, int iy, float *x, float *y)
{
   *x = 2.0 * (float) ix / (float) winwidth - 1.0;
   *y = -2.0 * (float) iy / (float) winheight + 1.0;
}

// Capture the BMP file
GLubyte* TextureLoadBitmap(char *filename, int *w, int *h) // I - Bitmap file to load
{
   BITMAPINFO *info; // Bitmap information
   void		  *bits; // Bitmap pixel bits
   GLubyte	  *rgb;	 // Bitmap RGB pixels
   GLubyte    err = '0';

   // Try to load the bitmap and converting it to RGB...
   bits = LoadDIBitmap(filename, &info);
   if (bits == NULL) 
	  return (NULL);
   
   rgb = ConvertRGB(info, bits);
   
   if (rgb == NULL) {
      free(info);
      free(bits);
   };

   printf("%s: %d %d\n", filename, info->bmiHeader.biWidth, info->bmiHeader.biHeight);
   printf("read %s successfully\n", filename);
   *w = info->bmiHeader.biWidth;
   *h = info->bmiHeader.biHeight;

   // Free the bitmap and RGB images, then return 0 (no errors).
   free(info);
   free(bits);
   
   return (rgb);
}

// Mouse callback for handling mouse click
void Mouse(int button, int state, int ix, int iy)
{
   float x, y;

   // Transform mouse position from screen coordinate to world 2D coordinate
   TransformXY(ix, iy, &x, &y);

   // Save old cursor location when mouse is clicked
   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
	  G_mousedown_L = TRUE;
	  old_x = x;
	  old_y = y;
   }
   else if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
	  G_mousedown_L = FALSE;
	  old_dx = dx;
	  old_dy = dy;
	  dx = 0;
	  dy = 0;
   }
   if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
	  G_mousedown_R = TRUE;
	  old_x = x;
	  old_y = y;
   }
   else if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
	  G_mousedown_R = FALSE;
   }
   glutPostRedisplay();
}

void Drag(int ix, int iy)
{  
   // Transform mouse position from screen coordinate to world 2D coordinate
   TransformXY(ix, iy, &new_x, &new_y);

   // Increment rotation angle for right mouse dragging
   if (G_mousedown_L) {
	  Rotate_y += (new_y - old_y) * 30;
	  old_x = new_x;
	  old_y = new_y;
   }

   // Increment scale for left mouse dragging
   if (G_mousedown_R) {
	  Scale += new_y - old_y;
	  if (Scale < 0.2) Scale = (float) 0.2;
	  if (Scale > 3.0) Scale = (float) 3.0;
	  old_x = new_x;
	  old_y = new_y;
   }
   glutPostRedisplay();
}

static void Key_Global()
{
	globalR = !globalR;
}

static void Key_Toggle()
{
	selfR = !selfR;
}

static void Key_Reset()
{
	year_venus = YEAR_DAY;
	year_mars = YEAR_DAY;
	year_earth= YEAR_DAY;
	year_moon = YEAR_DAY;
}


// Keyboard callback for handling keyboard event
void Keyboard(unsigned char key, int x, int y)
{
   // Add code here to control the animation interactively
   switch (key) {
	   case 'g':
		   Key_Global();
		   break;
	   case 't':
		   Key_Toggle();
		   break;
	   case 'r':
		   Key_Reset();
		   break;
   }
}
// Special key-stroke callback function
void Special(int key, int x, int y)
{
   if (key == GLUT_KEY_F1) {
	  _ScreenMode ++;
	  if (_ScreenMode > 3) _ScreenMode = 0;
	  if (_ScreenMode == 0)
		 glutReshapeWindow(350, 200);
	  else if(_ScreenMode == 1)
		 glutReshapeWindow(700, 400);
	  else if(_ScreenMode == 2)
	 	 glutReshapeWindow(900, 600);
	  else if(_ScreenMode == 3)
	 	 glutReshapeWindow(150, 75);
   }
}

// Reshape callback
void Reshape(int w, int h)
{
   winwidth  = w;
   winheight = h;
   glViewport(0, 0, w, h);      // Viewport transformation
   glMatrixMode(GL_PROJECTION);	// Projection transformation
   glLoadIdentity(); 

   gluPerspective(45, (GLfloat) w / (GLfloat) h, 1.0, 40.0); 
   glutPostRedisplay();

   glMatrixMode( GL_MODELVIEW );
}

void DrawSphere()
{
	for (int i = 0; i < MAX_STAR; i++) {
		glPointSize(1.0f);

		glBegin(GL_POINTS);
		glColor3f(rand()/RAND_MAX, rand()/RAND_MAX, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(star[i].pos[0], star[i].pos[1], star[i].pos[2]);
		glEnd();
	}
}

// Display callback
void Display(void)
{
	// Clear the window color before drawing is performed
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

   // Clear the current matrix (Modelview)
   glLoadIdentity();

   glTranslatef ( 0.0, 0.0, -4.0 );

   // draw sphere
   glPushMatrix();
   	 glTranslatef(0.0, 0.0, 0.0);
	 glRotatef( PI_RAID*year_earth/YEAR_DAY, 0.0, 1.0, 0.0 );
	 glScalef(Scale, Scale, Scale);
	 glDisable(GL_COLOR_MATERIAL);
	 DrawSphere();
   glPopMatrix();

   // Draw the sun
   glPushMatrix();						
   glRotatef( 15.0, 1.0, 0.0, 0.0 );

   glEnable(GL_COLOR_MATERIAL);
   glColor3f( 1.0, 1.0, 1.0);			
   glLightModeli( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
   glutWireSphere(SUN_RADIUS, 10, 10 );
   glClearColor( 0.0, 0.0, 0.0, 0.0 );
   glDisable(GL_COLOR_MATERIAL);

   glPopMatrix();						


   // Draw the Venus
   glPushMatrix();						

   	 glRotatef( PI_RAID*year_venus/YEAR_DAY, 0.0, 1.0, 0.0 );
	 glRotatef(-90, 1.0, 0.0, 0.0);
	 glBindTexture(GL_TEXTURE_2D, textureVenus);
	 glTranslatef(VENUS_ORBIT, 0.0, 0.0 );

	 glPushMatrix();						
	   glRotatef( PI_RAID*day_venus/DAY_HOUR, 0.0, 0.0, 1.0 );
	   glScalef(Scale, Scale, Scale);
	   gluSphere(venus, VENUS_RADIUS, 10, 10);
	 glPopMatrix();						

   glPopMatrix();						

   // Draw the Mars
   glPushMatrix();						

     glRotatef( PI_RAID*year_mars/YEAR_DAY, 0.0, 1.0, 0.0 );
	 glRotatef(-90, 1.0, 0.0, 0.0);
	 glBindTexture(GL_TEXTURE_2D, textureMars);
	 glTranslatef(MARS_ORBIT, 0.0, 0.0 );

	 glPushMatrix();						
	   glRotatef( PI_RAID*day_mars/DAY_HOUR, 0.0, 0.0, 1.0 );
	   glScalef(Scale, Scale, Scale);
	   gluSphere(mars, MARS_RADIUS, 10, 10);
	 glPopMatrix();						

   glPopMatrix();						

   glPushMatrix();
   	    glRotatef(-15.0, 0.0, 0.0, 1.0);
   		glRotatef( PI_RAID*year_earth/YEAR_DAY, 0.0, 1.0, 0.0 );
   		glTranslatef(EARTH_ORBIT, 0.0, 0.0 );

		glScalef(Scale, Scale, Scale);

		glBegin(GL_LINES);
		glColor3f( 0.2, 0.2, 1.0 );
		glVertex3f(0.0f, -EARTH_RADIUS*2.0f, 0.0f);
		glVertex3f(0.0f, EARTH_RADIUS*2.0f, 0.0f);
		glEnd();
   
   glPopMatrix();

   // draw earth
   glRotatef(-15, 0.0, 0.0, 1.0);
   glRotatef( PI_RAID*year_earth/YEAR_DAY, 0.0, 1.0, 0.0 );
   glRotatef(-90, 1.0, 0.0, 0.0);

   glTranslatef(EARTH_ORBIT, 0.0, 0.0 );
   glPushMatrix();						
   	   glRotatef(-15, 0.0, 0.0, 1.0);
	   glRotatef( PI_RAID*day_earth/DAY_HOUR, 0.0, 0.0, 1.0 );
	   glBindTexture(GL_TEXTURE_2D, textureEarth);
	   glScalef(Scale, Scale, Scale);
	   gluSphere(earth, EARTH_RADIUS, 10, 10);

   glPopMatrix();						

   // draw moon
   glPushMatrix();						
   glRotatef( -PI_RAID*year_moon/YEAR_DAY, 0.0, 0.0, 1.0 );
   glBindTexture(GL_TEXTURE_2D, textureMoon);
   glTranslatef(MOON_ORBIT, 0.0, 0.0 );
   glScalef(Scale, Scale, Scale);
   gluSphere(moon, MOON_RADIUS, 10, 10);
   glPopMatrix();						


   // Flush the pipeline, and swap the buffers
   glFlush();
   glutSwapBuffers();

   glutPostRedisplay();
}

/* load texture images from bmp file */
void LoadImageTexture(GLuint *textureId, GLUquadricObj **obj, char *filename)
{
	int w, h;

	glGenTextures(3, textureId);
	glBindTexture(GL_TEXTURE_2D, *textureId);

	GLubyte *img = TextureLoadBitmap(filename, &w, &h);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, w, h, GL_RGB, GL_UNSIGNED_BYTE, img);

	*obj = gluNewQuadric();
	gluQuadricDrawStyle(*obj, GLU_FILL);
	gluQuadricTexture(*obj, GL_TRUE);
}

void init()
{
	int i;

	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_shininess[] = {50.0};
	GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_TEXTURE_2D);

	// Clear screen color
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

	// Texture mapping setting for Microsoft's OpenGL implementation
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);

	// Texture mapping parameters for filter and repeatance
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


	LoadImageTexture(&textureEarth, &earth, "earth.bmp");
	LoadImageTexture(&textureMoon, &moon, "moon.bmp");
	LoadImageTexture(&textureMars, &mars, "mars.bmp");
	LoadImageTexture(&textureVenus, &venus, "venus.bmp");

	globalR = GL_TRUE;
	selfR = GL_TRUE;

	incStep = 1.0;

	/* yield stars */
	srand(time(NULL));
	for (i = 0; i < MAX_STAR; i++) {
		star[i].pos[0] = 5.0*rand()/RAND_MAX-2.0;
		star[i].pos[1] = 5.0*rand()/RAND_MAX-2.0;
		star[i].pos[2] = sqrt(3.1*3.1-star[i].pos[0]*star[i].pos[0]-
				star[i].pos[1]*star[i].pos[1]);

		if (rand()%2 == 0) star[i].pos[2] *= -1;
	}
}

void Idle(void)
{
	// Update the animation state
   if (selfR == GL_TRUE) {
	   day_earth += incStep;
	   day_venus += incStep*EARTH_DAY/VENUS_DAY;
	   day_mars += incStep*EARTH_DAY/MARS_DAY;

	   day_earth = day_earth - ((int)(day_earth/DAY_HOUR))*DAY_HOUR;
	   day_venus = day_venus - ((int)(day_venus/DAY_HOUR))*DAY_HOUR;
	   day_mars = day_mars - ((int)(day_mars/DAY_HOUR))*DAY_HOUR;
   }

   if (globalR == GL_TRUE) {
	   year_earth += incStep/DAY_HOUR;
	   year_venus += incStep*EARTH_YEAR/VENUS_YEAR/DAY_HOUR;
	   year_mars += incStep*EARTH_YEAR/MARS_YEAR/DAY_HOUR;

	   year_earth = year_earth - ((int)(year_earth/YEAR_DAY))*YEAR_DAY;
	   year_moon = 12.0 * year_earth;
	   year_mars = year_mars - ((int)(year_mars/YEAR_DAY))*YEAR_DAY;
	   year_venus = year_venus - ((int)(year_venus/YEAR_DAY))*YEAR_DAY;
   }

   glutPostRedisplay();
}

void main(int argc, char** argv)
{
	// Initialization of GLUT Library
	glutInitWindowSize(800, 450);
	glutInitWindowPosition(100, 100);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	// Create a window with title specified
	glutCreateWindow("Project2: Solor System Animation");

	// Register different callback functions for GLUT to response 
	// with different events, e.g. window sizing, mouse click or
	// keyboard stroke
	glutReshapeFunc(Reshape);
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);
	glutIdleFunc(Idle);
	glutMouseFunc(Mouse);
	glutMotionFunc(Drag);
	init();	// not GLUT call, initialize several parameters

	// Enter the GLUT event processing loop which never returns.
	// It will call different registered callback according to different events.
	glutMainLoop();
}
