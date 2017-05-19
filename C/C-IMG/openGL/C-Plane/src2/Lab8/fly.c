/**************************
 * fly insect
 **************************/

#include <windows.h>
#include <gl/gl.h>

#include <stdlib.h>
#include <GL/glut.h>

#define HEAD_RADIUS 1.7
#define BODY_RADIUS 1.6
#define TAIL1_RADIUS 2.0
#define EYE_RADIUS 1.3
#define TAIL_RADIUS 0.9
#define TAIL_HEIGHT 2.8
#define MOUTH_RADIUS 0.4
#define MOUTH_HEIGHT 0.4

GLint menu_id2 = 0;
GLint materialID = 1;
GLint lightID =1;
GLboolean Sun = GL_FALSE;
GLboolean SPOTTING = GL_FALSE;

GLdouble circularAngle =0;
GLdouble jumpAngle =0;
GLdouble flyZ =0;

GLUquadricObj *p; // pointer to quadric pect
GLfloat theta[14] = {0.0, 0.0,0.0,0.0};
GLfloat light0_pos[4] = {-2.0, 2.0,2.0, 1.0};//light position using spot plight source
GLfloat light1_pos[4] = {3.0, 3.0, -7.0, 0.0};//light position using distant sun light source

//light source
typedef struct lightingStruct {
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat direction[4];
	GLfloat cutoff;
} lightingStruct;

lightingStruct whiteLighting = {
	{1.0, 1.0, 1.0, 1.0},
	{1.0, 1.0, 1.0, 1.0},
	{1.0, 1.0, 1.0, 1.0},
	{1.0, 0.0, 0.0, 1.0},
	5.0f
};

//green lighting properties
lightingStruct greenLighting = {
	{0.0, 1.0, 0.0, 1.0},
	{0.0, 1.0, 0.0, 1.0},
	{0.0, 1.0, 0.0, 1.0},
	{-1.0, 0.0, 0.0, 1.0},
	5.0f
};
//magenta lighting properties
lightingStruct magentaLighting = {
	{1.0, 1.0, 1.0, 1.0},
	{1.0, 0.0, 1.0, 1.0},
	{1.0, 1.0, 1.0, 1.0},
	{-1.0, 0.0, 0.0, 1.0},
	5.0f
};

//light sphere
typedef struct lightSphereStruct {
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat emission[4];
} lightSphereStruct;

////white sphere properties
lightSphereStruct white_emission = {
	{1.0, 1.0, 1.0, 1.0},
	{1.0, 1.0, 1.0, 1.0},
	{1.0, 1.0, 1.0, 1.0},
	{0.5, 0.5, 0.5, 1.0}
};
////green sphere properties
lightSphereStruct green_emission = {
	{0.0, 1.0, 0.0, 1.0},
	{0.0, 1.0, 0.0, 1.0},
	{0.0, 1.0, 0.0, 1.0},
	{0.0, 0.5, 0.0, 1.0}
};
////magenta sphere properties
lightSphereStruct magenta_emission = {
	{1.0, 0.0, 1.0, 1.0},
	{1.0, 0.0, 1.0, 1.0},
	{1.0, 0.0, 1.0, 1.0},
	{0.5, 0.0, 0.5, 1.0}
};



lightSphereStruct *currentLightSphere;
lightingStruct *currentLightings1;
lightingStruct *currentLightings0;
lightingStruct *currentLightings;

//material property
typedef struct materialStruct {
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shininess;
} materialStruct;


//colorful material
materialStruct colorMaterials_body = {
	{0.0f, 0.0f, 0.0f, 1.0f},
    {0.1f, 0.5f, 0.8f, 1.0f},
    {0.0f, 0.0f, 0.0f, 1.0f},
    20.0f
};

materialStruct colorMaterials_tail = {
	{0.3f, 0.3f, 0.3f, 1.0f},
    {0.1f, 0.5f, 0.8f, 1.0f},
    {0.3f, 0.3f, 0.3f, 1.0f},
    20.0f
};

materialStruct colorMaterials_leg = {
	{0.5f, 0.5f, 0.0f, 1.0f},
	{0.5f, 0.5f, 0.1f, 1.0f},
	{1.0f, 0.9f, 0.8f, 1.0f},
	20.0f
};

materialStruct colorMaterials_mouth = {
	{0.5f, 0.5f, 0.0f, 1.0f},
	{0.5f, 0.5f, 0.1f, 1.0f},
	{1.0f, 0.9f, 0.8f, 1.0f},
	20.0f
};

materialStruct colorMaterials_eye1 = {
	{0.3f, 0.3f, 0.3f, 1.0f},
    {0.3f, 0.3f, 0.3f, 1.0f},
    {0.3f, 0.3f, 0.6f, 1.0f},
    20.0f
};

materialStruct colorMaterials_eye2 = {
	{0.0f, 0.0f, 0.0f, 1.0f},
	{0.0f, 0.0f, 0.0f, 1.0f},
	{1.0f, 1.0f, 1.0f, 1.0f},
	20.0f    
};

materialStruct colorMaterials_wing = {
    {1.0, 0.5f, 0.5f, 1.0f},
    {0.1f, 0.5f, 0.8f, 1.0f},
    {0.0f, 0.0f, 0.0f, 1.0f},
    20.0f
};

//monochrome material
materialStruct monochromeMaterials_body = {  
	{0.1f, 0.1f, 0.1f, 1.0f},
    {1.0f, 1.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, 1.0f, 1.0f},
    20.0f
};

materialStruct monochromeMaterials_leg = {
	{0.1f, 0.1f, 0.1f, 1.0f},
	{0.1f, 0.1f, 0.1f, 1.0f},
	{0.5f, 0.5f, 0.5f, 1.0f},
	20.0f
};

//woddy material
materialStruct woddyMaterials_body = {
	{0.2f, 0.2f, 0.2f, 1.0f},
	{0.392f, 0.3058f, 0.129f, 1.0f},
	{0.0f, 0.0f, 0.0f, 1.0f},
	20.0f
};

materialStruct woddyMaterials_tail = {
	{0.2f, 0.2f, 0.2f, 1.0f},
	{0.914, 0.506, 0.15, 1.0},
	{0.5f, 0.5f, 0.5f, 1.0f},
	20.0f
};

materialStruct woddyMaterials_leg = {
	{0.2f, 0.2f, 0.2f, 1.0f},
	{0.933, 0.737, 0.498, 1.0},
	{0.5f, 0.5f, 0.5f, 1.0f},
	20.0f
};

materialStruct *currentMaterials_body;
materialStruct *currentMaterials_tail;
materialStruct *currentMaterials_eye1;
materialStruct *currentMaterials_eye2;
materialStruct *currentMaterials_leg;
materialStruct *currentMaterials_mouth;
materialStruct *currentMaterials_body;
materialStruct *currentMaterials_wing;


//create head
void head()
{
    glPushMatrix();
    glutSolidSphere(HEAD_RADIUS,100.0,100.0);//plot a sphere as the head of fly
    glPopMatrix();
}

void body()
{
    glPushMatrix();
    glutSolidSphere(BODY_RADIUS,100.0,100.0);//plot a sphere as the head of fly
    glPopMatrix();
}

void fly_tail()
{
    glPushMatrix();
    glutSolidSphere(TAIL1_RADIUS,100.0,100.0);//plot a sphere as the head of fly
    glPopMatrix();
}


//create eyes
void eye()
{	
    glPushMatrix();
    glutSolidSphere(EYE_RADIUS, 100, 8);//plot a sphere as the eye of fly
    glPopMatrix();
}

//create mouth
void mouth()
{
    glPushMatrix();
    glutSolidCone(MOUTH_RADIUS,MOUTH_HEIGHT,100.0,8.0);//plot scaled sphere as the mouth of fly
    glPopMatrix();
}

#define AUX_LEG_LEN 1.5
#define AUX_FOOT_LEN 1.6
#define AUX_LEG_RADIUS 0.2
#define AUX_FOOT_RADIUS 0.1
#define KNEE_RADIUS 0.13
#define LOW_LEG_LEN 3.0
#define LOW_LEG_LOW_RADIUS 0.3
#define LOW_LEG_UP_RADIUS 0.2
#define MIDDLE_LEG_LEN 2.8
#define MIDDLE_LEG_LOW_RADIUS 0.2
#define MIDDLE_LEG_UP_RADIUS 0.1
#define UP_LEG_LEN 1.6
#define UP_LEG_LOW_RADIUS 0.1
#define UP_LEG_UP_RADIUS 0.07
#define FOOT_LEN 0.8
#define FOOT_RADIUS 0.07


void fly_auxleftLeg()
{
   glPushMatrix();
   glTranslatef (0.0, 0.5*AUX_LEG_LEN, 0.0);
   glRotatef(-45.0, 1.0, 0.0, 0.0);
   glRotatef(-45.0, 0.0, 1.0, 0.0);
   glRotatef(45.0, 0.0, 0.0, 1.0);
   gluCylinder(p, AUX_LEG_RADIUS, 0.5*AUX_LEG_RADIUS, AUX_LEG_LEN, 30, 30);
   glPopMatrix();
}

void fly_knee()
{
   glPushMatrix();
   gluSphere(p, KNEE_RADIUS, 20, 20);
   glPopMatrix();
}

void fly_auxfoot()
{
   glPushMatrix();
   glTranslatef (0.0, -0.45, 0.0);
   glRotatef(90.0, 1.0, 0.0, 0.0);
   glutWireCone(AUX_FOOT_RADIUS, AUX_FOOT_LEN, 30, 30);
   glPopMatrix();
}

void fly_auxRightLeg()
{
   glPushMatrix();
   glTranslatef (0.0, 0.5*AUX_LEG_LEN, 0.0);
   glRotatef(45.0, 1.0, 0.0, 0.0);
   glRotatef(-135.0, 0.0, 1.0, 0.0);
   glRotatef(-45.0, 0.0, 0.0, 1.0);
   gluCylinder(p, AUX_LEG_RADIUS, 0.5*AUX_LEG_RADIUS, AUX_LEG_LEN, 30, 30);
   glPopMatrix();
}

void fly_leftLowLeg()
{
   glPushMatrix();
   glRotatef(-30.0, 0.0, 1.0, 0.0);
   glRotatef(-50.0, 1.0, 0.0, 0.0);
   gluCylinder(p, LOW_LEG_LOW_RADIUS, LOW_LEG_UP_RADIUS, LOW_LEG_LEN, 30, 30);
   glPopMatrix();
}

void fly_leftMidLeg()
{
   glPushMatrix();
   glRotatef(-35.0, 0.0, 1.0, 0.0);
   glRotatef(45.0, 1.0, 0.0, 0.0);
   gluCylinder(p, MIDDLE_LEG_LOW_RADIUS, MIDDLE_LEG_UP_RADIUS, MIDDLE_LEG_LEN, 30, 30);
   glPopMatrix();
}

void fly_leftUpLeg()
{
   glPushMatrix();
   glRotatef(-40.0, 0.0, 1.0, 0.0);
   glRotatef(75.0, 1.0, 0.0, 0.0);
   gluCylinder(p, UP_LEG_LOW_RADIUS, UP_LEG_UP_RADIUS, UP_LEG_LEN, 30, 30);
   glPopMatrix();
}

void fly_leftfoot()
{
   glPushMatrix();
   glRotatef(-50.0, 0.0, 1.0, 0.0);
   glRotatef(25.0, 1.0, 0.0, 0.0);
   glutWireCone(FOOT_RADIUS, FOOT_LEN, 30, 30);
   glPopMatrix();
}


void fly_wing()
{
   glPushMatrix();
   glScalef (0.7, 0.1, 4.7);
   gluSphere(p, 1, 20, 20);
   glPopMatrix();
}

void leftLowLeg()
{
   glPushMatrix();
   glRotatef(-30.0, 0.0, 1.0, 0.0);
   glRotatef(-50.0, 1.0, 0.0, 0.0);
   gluCylinder(p, LOW_LEG_LOW_RADIUS, LOW_LEG_UP_RADIUS, LOW_LEG_LEN, 30, 30);
   glPopMatrix();
}

void leftMiddleLeg()
{
   glPushMatrix();
   glRotatef(-35.0, 0.0, 1.0, 0.0);
   glRotatef(45.0, 1.0, 0.0, 0.0);
   gluCylinder(p, MIDDLE_LEG_LOW_RADIUS, MIDDLE_LEG_UP_RADIUS, MIDDLE_LEG_LEN, 30, 30);
   glPopMatrix();
}

void leftUpLeg()
{
   glPushMatrix();
   glRotatef(-40.0, 0.0, 1.0, 0.0);
   glRotatef(75.0, 1.0, 0.0, 0.0);
   gluCylinder(p, UP_LEG_LOW_RADIUS, UP_LEG_UP_RADIUS, UP_LEG_LEN, 30, 30);
   glPopMatrix();
}

void leftFoot()
{
   glPushMatrix();
   glRotatef(-50.0, 0.0, 1.0, 0.0);
   glRotatef(25.0, 1.0, 0.0, 0.0);
   glutWireCone(FOOT_RADIUS, FOOT_LEN, 30, 30);
   glPopMatrix();
}



//the whole left leg
void leftLeg()
{
   glTranslatef(-0.7, 0.0, 1.0);
   leftLowLeg();
   
   glTranslatef(-0.9, 2.25, 1.5);
   leftMiddleLeg();
   
   glTranslatef(-1.1, -1.9, 1.58);
   leftUpLeg();
   
   glTranslatef(-0.22, -1.45, 0.27);
   leftFoot();
}


//create spotLight sphere
void lightSphere()
{
	if(SPOTTING){
		glDisable(GL_LIGHT0);
		glPushMatrix();
			glPushAttrib(GL_LIGHTING_BIT);
		if(Sun) {
            glDisable(GL_LIGHT1);
		}
        glTranslatef(-2.0, 2.0,2.0);
        glMaterialfv(GL_FRONT, GL_EMISSION, currentLightings0 -> diffuse);
        glutSolidSphere(0.2, 100, 8);//plot an sphere
        glPopMatrix();
        glPopAttrib();
        if(Sun){
			glEnable(GL_LIGHT1);
		}
		glEnable(GL_LIGHT0);
	}
}

void setSPOTTINGlight(){
    if(SPOTTING){
        glLightfv(GL_LIGHT0,GL_POSITION, light0_pos);

        glLightfv(GL_LIGHT0, GL_AMBIENT, currentLightings0 -> ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, currentLightings0 -> diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, currentLightings0 -> specular);

        lightSphere();
    }
}
void setSunlight(){
    if(Sun){
        glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
        glLightfv(GL_LIGHT1, GL_AMBIENT, currentLightings1 -> ambient);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, currentLightings1 -> diffuse);
        glLightfv(GL_LIGHT1, GL_SPECULAR, currentLightings1 -> specular);
    }
}

void fly()//create the image
{
    if(materialID==1)
    {
        currentMaterials_body = &colorMaterials_body;
        currentMaterials_tail = &colorMaterials_tail;
        currentMaterials_leg = &colorMaterials_leg;
        currentMaterials_mouth = &colorMaterials_mouth;
        currentMaterials_eye1 = &colorMaterials_eye1;
        currentMaterials_eye2 = &colorMaterials_eye2;
        currentMaterials_wing = &colorMaterials_wing;
    }

    if(materialID==2)//metalic material
    {
        currentMaterials_body = &monochromeMaterials_body;
        currentMaterials_tail = &monochromeMaterials_body;
        currentMaterials_leg = &monochromeMaterials_leg;
        currentMaterials_mouth = &monochromeMaterials_leg;
        currentMaterials_eye1 = &colorMaterials_eye1;
        currentMaterials_eye2 = &colorMaterials_eye2;
        currentMaterials_wing = &monochromeMaterials_body;
    }

    if(materialID==3) //woddy material
    {
        currentMaterials_body = &woddyMaterials_body;
        currentMaterials_tail = &woddyMaterials_tail;
        currentMaterials_leg = &woddyMaterials_leg;
        currentMaterials_mouth = &woddyMaterials_tail;
        currentMaterials_eye1 = &colorMaterials_eye1;
        currentMaterials_eye2 = &colorMaterials_eye2;
        currentMaterials_wing = &woddyMaterials_leg;
    }

    glTranslatef(0, jumpAngle, flyZ);

    //head
    glPushMatrix();
    glScalef(1.4, 1, 1);
    //glColor3ub (177, 177, 177);
    glMaterialfv(GL_FRONT, GL_AMBIENT, currentMaterials_body -> ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, currentMaterials_body -> diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, currentMaterials_body -> specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, &currentMaterials_body -> shininess);

    head();
    glPopMatrix();

    //left-eye
    glPushMatrix();
	glRotatef(theta[11],0.0, 1.0, 0.0);
    glTranslatef(HEAD_RADIUS*0.7, HEAD_RADIUS*0.3, HEAD_RADIUS*0.4);

    glMaterialfv(GL_FRONT, GL_AMBIENT, currentMaterials_eye1 -> ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, currentMaterials_eye1 -> diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, currentMaterials_eye1 -> specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, &currentMaterials_eye1 -> shininess);
    //glColor3ub (255, 176, 176);
    eye();
    glPopMatrix();

    glPushMatrix();
	glRotatef(theta[11],0.0, 1.0, 0.0);
    glScalef(0.6, 0.6, 0.6);
    glTranslatef(HEAD_RADIUS*2.0, HEAD_RADIUS*0.8, HEAD_RADIUS*0.8);
    //glColor3ub (30, 30, 30);

    glMaterialfv(GL_FRONT, GL_AMBIENT, currentMaterials_eye2 -> ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, currentMaterials_eye2 -> diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, currentMaterials_eye2 -> specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, &currentMaterials_eye2 -> shininess);
    eye();
    glPopMatrix();

    //right-eye
    glPushMatrix();
	glRotatef(theta[12],0.0, 1.0, 0.0);
    glTranslatef(-HEAD_RADIUS*0.7, HEAD_RADIUS*0.3, HEAD_RADIUS*0.4);
    //glColor3ub (255, 176, 176);

	glMaterialfv(GL_FRONT, GL_AMBIENT, currentMaterials_eye1 -> ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, currentMaterials_eye1 -> diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, currentMaterials_eye1 -> specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, &currentMaterials_eye1 -> shininess);
    eye();
    glPopMatrix();

    glPushMatrix();
	glRotatef(theta[12],0.0, 1.0, 0.0);
    glScalef(0.6, 0.6, 0.6);
    glTranslatef(-HEAD_RADIUS*2.0, HEAD_RADIUS*0.8, HEAD_RADIUS*0.8);
    //glColor3ub (30, 30, 30);

    glMaterialfv(GL_FRONT, GL_AMBIENT, currentMaterials_eye2 -> ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, currentMaterials_eye2 -> diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, currentMaterials_eye2 -> specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, &currentMaterials_eye2 -> shininess);
    eye();
    glPopMatrix();

    //body

    glPushMatrix();
    glTranslatef(0, 0, -HEAD_RADIUS*1.5);
    glScalef(1.2, 1.1, 1.3);
    //glColor3ub (90, 90, 90);
    body();
    glPopMatrix();

    // tail

    glPushMatrix();
	glRotatef(theta[1],0.0, 1.0, 1.0);
    glTranslatef(0, 0, -HEAD_RADIUS*3.5);
    glScalef(1.0, 1.1, 2.3);
    //glColor3ub (115, 115, 115);

	glMaterialfv(GL_FRONT, GL_AMBIENT, currentMaterials_tail -> ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, currentMaterials_tail -> diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, currentMaterials_tail -> specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, &currentMaterials_tail -> shininess);
    fly_tail();
    glPopMatrix();


    //mouth
    glPushMatrix();
    glRotated(180.0, 0,1,0);
    glTranslated(0, 0, -HEAD_RADIUS*1.15); 
    //glColor3ub (255, 162, 0);

	glMaterialfv(GL_FRONT, GL_AMBIENT, currentMaterials_mouth -> ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, currentMaterials_mouth -> diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, currentMaterials_mouth -> specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, &currentMaterials_mouth -> shininess);
    mouth();
    glPopMatrix();

    // leg
    glPushMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, currentMaterials_leg -> ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, currentMaterials_leg -> diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, currentMaterials_leg -> specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, &currentMaterials_leg -> shininess);
    //glColor3ub (112, 112, 112);
    glTranslatef(-0.4, -2.0, 1.0);
    fly_auxleftLeg();

    glTranslatef(-1.0, 1.95, 0.7);
    fly_auxfoot();

    glTranslatef(0.0, -0.5, 0.0);
    fly_knee();
    glPopMatrix();

    glPushMatrix();
    glRotatef(30, 0, 1.0, 0);
    //glColor3ub (112, 112, 112);
    glTranslatef(0.4, -2.0, 1.0);
    fly_auxleftLeg();

    glTranslatef(-1.0, 1.95, 0.7);
    fly_auxfoot();

    glTranslatef(0.0, -0.5, 0.0);
    fly_knee();
    glPopMatrix();

    //left first leg
    glPushMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, currentMaterials_leg -> ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, currentMaterials_leg -> diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, currentMaterials_leg -> specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, &currentMaterials_leg -> shininess);
    glRotatef(130, 0, 1, 0);

    glTranslatef(1.5, -2.0, -0.5);
    fly_leftLowLeg();

    glTranslatef(-0.9, 2.25, 1.5);
    fly_leftMidLeg();

    glTranslatef(-1.1, -1.9, 1.58);
    fly_leftUpLeg();

    glTranslatef(-0.22, -1.45, 0.27);
    fly_leftfoot();

    glPopMatrix();

    // right
    glPushMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, currentMaterials_leg -> ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, currentMaterials_leg -> diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, currentMaterials_leg -> specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, &currentMaterials_leg -> shininess);
    glRotatef(-30, 0, 1, 0);

    glTranslatef(-1.5, -1.5, -1.0);
    fly_leftLowLeg();

    glTranslatef(-0.9, 2.25, 1.5);
    fly_leftMidLeg();

    glTranslatef(-1.1, -1.9, 1.58);
    fly_leftUpLeg();

    glTranslatef(-0.22, -1.45, 0.27);
    fly_leftfoot();

    glPopMatrix();

    //left Third leg
    glPushMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, currentMaterials_leg -> ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, currentMaterials_leg -> diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, currentMaterials_leg -> specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, &currentMaterials_leg -> shininess);
    //glColor3ub(112, 112, 112);
    glRotatef(-65, 0, 1, 0);
    glTranslatef(0, -1.0, -2.4);
    leftLeg();
    glPopMatrix();

    glPushMatrix();
    //glColor3ub(112, 112, 112);
    glRotatef(65, 0, 1, 0);
    glTranslatef(3.0, -1.0, -2.4);
    leftLeg();
    glPopMatrix();

    // wing
    glPushMatrix();
    glRotatef(theta[6], 1, 0, 0);

	glMaterialfv(GL_FRONT, GL_AMBIENT, currentMaterials_wing -> ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, currentMaterials_wing -> diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, currentMaterials_wing -> specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, &currentMaterials_wing -> shininess);
    //glColor3ub(0, 102, 153);

    glRotatef(170-theta[6], 0, 1, 0);
    glTranslatef(0.2, 2.0, 5.7);
    fly_wing();

    glTranslatef(2.0, 0.0, 0);
    glRotatef(20, 0, 1, 0);
    fly_wing();

    glPopMatrix();

}

void display()
{ 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//Clear color
    glMatrixMode(GL_MODELVIEW);//Using Modelling view mode
    glLoadIdentity();//initialization
    gluLookAt(-1.0, -1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	setSPOTTINGlight();
	setSunlight();

	glRotatef(circularAngle, 0.0, 1.0, 0.0);//Requirement rotation

    fly();

    glutSwapBuffers();
}

void init()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    p = gluNewQuadric();

	currentLightings = &whiteLighting;
	currentLightings0 = &whiteLighting;
	currentLightings1 = &whiteLighting;

	glEnable(GL_LIGHTING);//enable light
	glEnable(GL_LIGHT0);//enable light source 0
	glEnable(GL_LIGHT1);

    /*
	glLightfv(GL_LIGHT0,GL_POSITION, light0_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, currentLightings -> ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, currentLightings -> diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, currentLightings -> specular);
    */

}

void reshape(int w, int h)
{
    GLfloat ratio;

    if(0==h) h=1;

    glViewport (0, 0, w, h);//set viewport
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    ratio = (GLfloat)w / (GLfloat)h;
    if (w <= h)
        glOrtho(-10.0, 10.0, -10.0/ratio, 10/ratio, 20, -20);
    else  
        glOrtho(-10.0*ratio, 10*ratio, -10, 10, 20, -20);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

GLboolean jumpping = GL_FALSE;
GLboolean flying = GL_FALSE;
GLboolean rotating = GL_FALSE;

GLint menu_id = 0;

void mymenu(int menu)
{
    if (menu < 4) {
        if(menu == 1)//For flying 
            flying = !flying;
        if(menu == 2)//For jumping 
            jumpping = !jumpping;
        if(menu == 3)//For rotating 
            rotating = !rotating;

        materialID = menu;
    } else {
        materialID = menu-3;
    }
}

GLfloat JUMP_RATE = 0.17;
GLfloat SWING_RATE =0.1;

#define FLYING_RATE 0.09


GLint t1_direction = 1;
GLint t2_direction = 1;
GLint t3_direction = 1;
GLint f1_direction = 1;
GLint f2_direction = -1;
GLint f3_direction = 1;
GLint f4_direction = -1;
GLint b1_direction = 1;
GLint b2_direction = 1;
GLint b3_direction = 1;
GLint m_direction = 1;
GLint e1_direction = 1;
GLint e2_direction = 1;


void idle()
{
    //Do the jumpping
    if (jumpping) 
	{
		jumpAngle += JUMP_RATE;
        if (jumpAngle > 1.0 || jumpAngle < 0.0) {
            JUMP_RATE *= -1;
        }
	}

	if (flying)  
	{
        flyZ += FLYING_RATE/2;
        if (flyZ > 13.0) {
            flyZ -= 15;
        }

        theta[6] += SWING_RATE;
        if (theta[6] > 6 || theta[6] < 2) {
            SWING_RATE *= -1;
        }

		if (t1_direction > 0)
		{
			theta[1] += FLYING_RATE;
			if (theta[1] > 8) 
			{
				t1_direction = -1;
				theta[1] -= FLYING_RATE;
			}
		} else {
			theta[1] -= FLYING_RATE;
		    if (theta[1] < -8) 
			{
				t1_direction = 1;
				theta[1] += FLYING_RATE;
			}
		}
		
		if (t2_direction > 0)
		{

			theta[2] += FLYING_RATE*0.9;
			if (theta[2] > 7) 
			{
				t2_direction = -1;
				theta[2] -= FLYING_RATE*0.9;
			}
		} else {
			theta[2] -= FLYING_RATE*0.9;
		    if (theta[2] < -7) 
			{
				t2_direction = 1;
				theta[2] += FLYING_RATE*0.9;
			}
		}

		if (t3_direction > 0)
		{
			theta[3] += FLYING_RATE*0.8;
			if (theta[3] > 6) 
			{
				t3_direction = -1;
				theta[3] -= FLYING_RATE*0.8;
			}
		} else { 
            theta[3] -= FLYING_RATE*0.8;
		    if (theta[3] < -6) 
			{
				t3_direction = 1;
				theta[3] += FLYING_RATE*0.8;
			}
		}

		if (f1_direction > 0) 
		{
			theta[4] += FLYING_RATE;
			if (theta[4] > 13) 
			{
				f1_direction = -1;
				theta[4] -= FLYING_RATE;
			}
		} else {
			theta[4] -= FLYING_RATE;
			if (theta[4] < -13) 
			{
				f1_direction = 1;
				theta[4] += FLYING_RATE;
			}
		}

		if (f2_direction > 0) 
		{
			theta[5] += FLYING_RATE;
			if (theta[5] > 13) 
			{
				f2_direction = -1;
				theta[5] -= FLYING_RATE;
			}
		} else {
			theta[5] -= FLYING_RATE;
			if (theta[5] < -13) 
			{
				f2_direction = 1;
				theta[5] += FLYING_RATE;
			}
		}

		//flying left eye
		if(e1_direction > 0)
		{
			theta[11] += FLYING_RATE*0.15;
			if(theta[11] >3)
			{
				e1_direction = -1;
				theta[11] -= FLYING_RATE*0.15;
			}
		} else {
			theta[11] -= FLYING_RATE*0.15;
			if(theta[11] < -3)
			{
				e1_direction = 1;
				theta[11] += FLYING_RATE*0.15;
			}
		}

		//flying right eye
		if(e2_direction > 0)
		{
			theta[12] += FLYING_RATE*0.15;
			if(theta[12] >3)
			{
				e2_direction = -1;
				theta[12] -= FLYING_RATE*0.15;
			}
		} else {
			theta[12] -= FLYING_RATE*0.15;
			if(theta[12] < -3)
			{
				e2_direction = 1;
				theta[12] += FLYING_RATE*0.15;
			}
		}
	}

	//Rotating 
	if (rotating)
	{
		circularAngle += 0.09;//rotating the whole object
		if (circularAngle > 360)//if the angle is bigger than 360 
			circularAngle -= 360;//Than recount it from 0
	}

	glutPostRedisplay();
}

void mymouse(GLint btn, GLint state, GLint x, GLint y){
	if(btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		flying = !flying;
    }
}

void mykey(unsigned char key, int x, int y)
{
	switch(key) {
	case 'w'://white light
		currentLightings0 = &whiteLighting;//white light
		currentLightSphere=&white_emission;//white sphere
		break;
	case 'g'://green light
		currentLightings0 = &greenLighting;//green light
		currentLightSphere=&green_emission;//green sphere
		break;
	case 'm'://magenta light
		currentLightings0 = &magentaLighting;//magenta light
		currentLightSphere=&magenta_emission;//magenta sphere
		break;
	}
}

void mymenu2(int id)//create the light menu to chose light type
{
    if (id ==1)
    {
        SPOTTING = GL_FALSE;
        Sun = GL_TRUE;
        glEnable(GL_LIGHT1);
        glDisable(GL_LIGHT0);
    }
    if (id ==2)
    {
        SPOTTING = GL_TRUE;
        Sun = GL_FALSE;
        glEnable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
    }
    if (id ==3)
    {
        SPOTTING = GL_TRUE;
        Sun = GL_TRUE;
        glEnable(GL_LIGHT1);
        glEnable(GL_LIGHT0);
    }
}		

int main(int argc, char **argv)
{
    theta[6] = 2;

    glutInit(&argc, argv); 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(550, 550);//set window size
    glutCreateWindow("fly");//create a window named fly
    glutIdleFunc(idle);
    glutDisplayFunc(display); 
    init();
    glutMouseFunc(mymouse);

    menu_id = glutCreateMenu(mymenu);
    glutAddMenuEntry("Flying", 1);
    glutAddMenuEntry("Jumping", 2);
    glutAddMenuEntry("Rotating",3);
    glutAddMenuEntry("Colorful",4);
    glutAddMenuEntry("Monochrome",5);
    glutAddMenuEntry("Iron",6);
    glutAttachMenu(GLUT_LEFT_BUTTON);

	menu_id2 = glutCreateMenu(mymenu2);
    glutAddMenuEntry("SUN-light", 1);
    glutAddMenuEntry("SPOT-light", 2);
    glutAddMenuEntry("Both",3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutKeyboardFunc(mykey);
    glutReshapeFunc(reshape);//evoke reshape function
    glEnable(GL_DEPTH_TEST); /* Enable hidden-surface removal */

    glutMainLoop();
}

