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

#define LAYERS		10
#define SPLITS		10
#define SPEED		15.0

#define PI_DEGREE		360.0
#define DAYS_OF_YEAR 	365
#define DAY_HOUR 		24

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

typedef void (*ModelView)(void);

typedef struct tmp_star {
   float pos[3];
   float col[4];
} Star;

Star star[MAX_STAR];

void BuildSphere();
void BuildSun();
void BuildVenus();
void BuildMars();
void BuildEarth();
void BuildMoon();
void BuildSaturn();

ModelView planet[6] = {BuildSphere, BuildSun, BuildVenus,
 					BuildMars, BuildEarth, 
					BuildMoon};

GLuint textureEarth = -1, textureMars = -1, textureVenus = -1,
		 textureMoon = -1, textureSaturn = -1, textureUranus;

GLUquadricObj *sun, *mercury, *earth, *moon, *venus, *mars;
GLUquadricObj *jupiter, *saturn, *uranus, *neptune, *pluto;

float angle_global = 0;
float year_mercury, year_earth = 0, year_venus = 0, year_mars = 0, year_moon = 0;
float year_jupiter = 0, year_saturn = 0, year_uranus = 0, year_neptune = 0, year_pluto = 0;
float day_mercury = 0, day_earth = 0, day_venus = 0, day_mars = 0;
float day_jupiter = 0, day_saturn = 0, day_uranus = 0, day_neptune = 0, day_pluto = 0;

float earth_incline = 0;

static float incStep = 0.4;
static float earthStep = 0.01;

static GLenum gControl = GL_TRUE;
static GLenum rControl = GL_TRUE;

float precession = 0;

GLfloat Rotate_x = 90, Rotate_y = 30;
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

static void G_key_control()
{
	gControl = !gControl;
}

static void T_key_control()
{
	rControl = !rControl;
}

static void R_key_control()
{
	year_venus = DAYS_OF_YEAR; year_mars = DAYS_OF_YEAR;
	year_earth= DAYS_OF_YEAR; year_moon = DAYS_OF_YEAR;
}


// Keyboard callback for handling keyboard event
void Keyboard(unsigned char key, int x, int y)
{
   // Add code here to control the animation interactively
   switch (key) {
	   case 'g': case 'G': G_key_control();
		   break;
	   case 't': case 'T': T_key_control();
		   break;
	   case 'r': case 'R': R_key_control();
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

void randStars() /* add color-control */
{
	srand(time(NULL));

	for (int i = 0; i < MAX_STAR; i++) {
		glPointSize(1.0f);

		glColor3f(1.0*rand()/RAND_MAX, 1.0*rand()/RAND_MAX, 1.0*rand()/RAND_MAX);

		glBegin(GL_POINTS);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(star[i].pos[0], star[i].pos[1], star[i].pos[2]);
		glEnd();
	}
}

void BuildSphere()
{
   // Build sphere
   glPushMatrix();
   	 glTranslatef(0.0, 0.0, 0.0);
	 glRotatef( PI_DEGREE*year_earth/DAYS_OF_YEAR, 0.0, 1.0, 0.0 );
	 glScalef(Scale, Scale, Scale);
	 randStars();
   glPopMatrix();
}

void BuildSun()
{
   // Draw the sun
   glPushMatrix();						
	   glRotatef( -Rotate_x, 1.0, 0.0, 0.0 );
	   glColor3f( 1.0, 0.2, 0.2);			
	   glutWireSphere(SUN_RADIUS, LAYERS, 10 );
	   glColor3f( 1.0, 1.0, 1.0);			
   glPopMatrix();
}

void BuildVenus()
{
   // Draw the Venus
   glPushMatrix();						

   	 glRotatef( PI_DEGREE*year_venus/DAYS_OF_YEAR, 0.0, 1.0, 0.0 );
	 glRotatef(-Rotate_x, 1.0, 0.0, 0.0);
	 glBindTexture(GL_TEXTURE_2D, textureVenus);
	 glTranslatef(VENUS_ORBIT*Scale, 0.0, 0.0 );

	 glPushMatrix();						
	   glRotatef( PI_DEGREE*day_venus/DAY_HOUR, 0.0, 0.0, 1.0 );
	   glScalef(Scale, Scale, Scale);
	   gluSphere(venus, VENUS_RADIUS, LAYERS, SPLITS);
	 glPopMatrix();						

   glPopMatrix();						
}

void BuildMars()
{
   // Draw the Mars
   glPushMatrix();						

     glRotatef( PI_DEGREE*year_mars/DAYS_OF_YEAR, 0.0, 1.0, 0.0 );
	 glRotatef(-Rotate_x, 1.0, 0.0, 0.0);
	 glBindTexture(GL_TEXTURE_2D, textureMars);
	 glTranslatef(MARS_ORBIT*Scale, 0.0, 0.0 );

	 glPushMatrix();						
	   glRotatef( PI_DEGREE*day_mars/DAY_HOUR, 0.0, 0.0, 1.0 );
	   glScalef(Scale, Scale, Scale);
	   gluSphere(mars, MARS_RADIUS, LAYERS, SPLITS);
	 glPopMatrix();						

   glPopMatrix();						
}

void BuildUranus()
{
   // Draw the Uranus
   glPushMatrix();						

     glRotatef( PI_DEGREE*year_uranus/DAYS_OF_YEAR, 0.0, 1.0, 0.0 );
	 glRotatef(0, 1.0, 0.0, 0.0);
	 glBindTexture(GL_TEXTURE_2D, textureUranus);
	 glTranslatef(URANUS_ORBIT*Scale, 0.0, 0.0 );

	 glPushMatrix();						
	   glRotatef( PI_DEGREE*day_uranus/DAY_HOUR, 0.0, 0.0, 1.0 );
	   glScalef(Scale, Scale, Scale);
	   gluSphere(uranus, URANUS_RADIUS, LAYERS, SPLITS);
	 glPopMatrix();						

   glPopMatrix();						
}

void BuildSaturn()
{
   int i;
   // Draw the saturn
   glPushMatrix();						

	 glRotatef( PI_DEGREE*year_saturn/DAYS_OF_YEAR, 0.0, 1.0, 0.0 );
	 glBindTexture(GL_TEXTURE_2D, textureSaturn);

	 glTranslatef(SATURN_ORBIT*Scale, 0.0, 0.0 );

	 glPushMatrix();						

		 glBegin(GL_QUAD_STRIP);

		    for(i=0; i <= 360; i++)
			{
				glVertex3f(sin(i*3.1416/180)*0.8*Scale, cos(i*3.1416/180)*0.8*Scale, 0 );
				glVertex3f(sin(i*3.1416/180)*1.0*Scale, cos(i*3.1416/180)*1.0*Scale, 0 );
			}
		 glEnd();

		 glRotatef( PI_DEGREE*day_saturn/DAY_HOUR, 0.0, 0.0, 1.0 );
		 glScalef(Scale, Scale, Scale);
		 gluSphere(saturn, SATURN_RADIUS, LAYERS, SPLITS);

	 glPopMatrix();						

   glPopMatrix();						
}

void BuildEarth()
{
   glPushMatrix();
   	    glRotatef(-earth_incline, 0.0, 0.0, 1.0);
   		glRotatef( PI_DEGREE*year_earth/DAYS_OF_YEAR, 0.0, 1.0, 0.0 );
   		glTranslatef(EARTH_ORBIT*Scale, 0.0, 0.0 );

		glScalef(Scale, Scale, Scale);

		glBegin(GL_LINES);
		glVertex3f(0.0f, -EARTH_RADIUS*2.0f, 0.0f);
		glVertex3f(0.0f, EARTH_RADIUS*2.0f, 0.0f);
		glEnd();
   
   glPopMatrix();

   // Build earth
   glRotatef(-earth_incline, 0.0, 0.0, 1.0);
   glRotatef( PI_DEGREE*year_earth/DAYS_OF_YEAR, 0.0, 1.0, 0.0 );
   glRotatef(-Rotate_x, 1.0, 0.0, 0.0);

   glTranslatef(EARTH_ORBIT*Scale, 0.0, 0.0 );

   glPushMatrix();						
   	   glRotatef(-earth_incline, 0.0, 0.0, 1.0);
	   glRotatef( PI_DEGREE*day_earth/DAY_HOUR, 0.0, 0.0, 1.0 );
	   glBindTexture(GL_TEXTURE_2D, textureEarth);
	   glScalef(Scale, Scale, Scale);
	   gluSphere(earth, EARTH_RADIUS, LAYERS, SPLITS);
   glPopMatrix();						
}

void BuildMoon()
{
   // Build moon, clock-wise
   glPushMatrix();						
   glRotatef(-PI_DEGREE*year_moon/DAYS_OF_YEAR, 0.0, 0.0, 1.0 );
   glBindTexture(GL_TEXTURE_2D, textureMoon);
   glTranslatef(MOON_ORBIT*Scale, 0.0, 0.0 );
   glScalef(Scale, Scale, Scale);
   gluSphere(moon, MOON_RADIUS, LAYERS, SPLITS);
   glPopMatrix();						
}

// Display callback
void Display(void)
{
	int i;
	// Clear the window color before Building is performed
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

   // Clear the current matrix (Modelview)
   glLoadIdentity();
   glTranslatef ( 0.0, 0.0, -4.0 );
   glRotatef(Rotate_y, 1.0, 0.0, 0.0);

   for (i = 0; i < 6; i++) 
	   planet[i]();

   BuildSaturn();
   BuildUranus();

   // Flush the pipeline, and swap the buffers
   glFlush();
   glutSwapBuffers();

   glutPostRedisplay();
}

/* load texture images from bmp file */
void TextureLoad(GLuint *textureId, GLUquadricObj **obj, char *filename)
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

/*
 * GL setting
 */
void LightSetting()
{	
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
}

void init()
{
	int i;

	LightSetting();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_COLOR_MATERIAL);

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

	TextureLoad(&textureEarth, &earth, "earth.bmp");
	TextureLoad(&textureMoon, &moon, "moon.bmp");
	TextureLoad(&textureMars, &mars, "mars.bmp");
	TextureLoad(&textureVenus, &venus, "venus.bmp");
	TextureLoad(&textureSaturn, &saturn, "saturn.bmp");
	TextureLoad(&textureUranus, &uranus, "uranus.bmp");

	gControl = GL_TRUE;
	rControl = GL_TRUE;

	earthStep = 0.01;
	earth_incline = SPEED;
	incStep = 0.4;

	/* stars */
	srand(time(NULL));
	for (i = 0; i < MAX_STAR; i++) {
		star[i].pos[0] = 10.0*rand()/RAND_MAX-5;
		star[i].pos[1] = 10.0*rand()/RAND_MAX-5;
		star[i].pos[2] = sqrt(7.1*7.1-star[i].pos[0]*star[i].pos[0]- star[i].pos[1]*star[i].pos[1]);

		if (rand()%2 == 0) star[i].pos[2] *= -1;
	}
}

void Idle(void)
{
	// Update the animation state
   if (rControl == GL_TRUE) {
	   day_earth += incStep;
	   day_venus += incStep*EARTH_DAY/VENUS_DAY;
	   day_mars += incStep*EARTH_DAY/MARS_DAY;
	   day_saturn += incStep*EARTH_DAY/SATURN_DAY;
	   day_uranus += incStep*EARTH_DAY/URANUS_DAY;

	   day_earth = day_earth - ((int)(day_earth/DAY_HOUR))*DAY_HOUR;
	   day_venus = day_venus - ((int)(day_venus/DAY_HOUR))*DAY_HOUR;
	   day_mars = day_mars - ((int)(day_mars/DAY_HOUR))*DAY_HOUR;
	   day_saturn = day_saturn - ((int)(day_saturn/DAY_HOUR))*DAY_HOUR;
	   day_uranus = day_uranus - ((int)(day_uranus/DAY_HOUR))*DAY_HOUR;

	   earth_incline += earthStep;
	   if (earth_incline > SPEED || earth_incline < -SPEED) earthStep = -earthStep;
   }

   if (gControl == GL_TRUE) {
	   year_earth += incStep/DAY_HOUR;
	   year_venus += incStep*EARTH_YEAR/VENUS_YEAR/DAY_HOUR;
	   year_mars += incStep*EARTH_YEAR/MARS_YEAR/DAY_HOUR;
	   year_saturn += incStep*EARTH_YEAR/SATURN_YEAR/DAY_HOUR;
	   year_uranus += incStep*EARTH_YEAR/URANUS_YEAR/DAY_HOUR;

	   year_earth = year_earth - ((int)(year_earth/DAYS_OF_YEAR))*DAYS_OF_YEAR;
	   year_moon = 12.0 * year_earth;
	   year_mars = year_mars - ((int)(year_mars/DAYS_OF_YEAR))*DAYS_OF_YEAR;
	   year_venus = year_venus - ((int)(year_venus/DAYS_OF_YEAR))*DAYS_OF_YEAR;
	   year_saturn = year_saturn - ((int)(year_saturn/DAYS_OF_YEAR))*DAYS_OF_YEAR;
	   year_uranus = year_uranus - ((int)(year_uranus/DAYS_OF_YEAR))*DAYS_OF_YEAR;
   }

   glutPostRedisplay();
}

int main(int argc, char** argv)
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

	return 0;
}
