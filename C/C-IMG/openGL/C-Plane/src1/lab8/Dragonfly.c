#include <stdlib.h>
#include <GL/glut.h>

#define HEAD_X_RADIUS 1.7
#define HEAD_Y_RADIUS 1.5
#define HEAD_Z_RADIUS 1.6

#define LOW_BODY_X_RADIUS 1.2
#define LOW_BODY_Y_RADIUS 1.0
#define LOW_BODY_Z_RADIUS 1.3

#define UP_BODY_RADIUS 0.7
#define UP_BODY_X_RADIUS 4.7

#define EYE_RADIUS 0.9

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

GLboolean relative = GL_TRUE;

GLint submenu_id1 = 0;
GLint submenu_id2 = 1;
GLint submenu_id3 = 2;

GLfloat theta[20] = {0.0, 0.0, 0.0};
GLint menu_id = 0;
GLfloat flyX = 0;
GLfloat jumpY = 0;
GLfloat delta = 0.3;
GLfloat delta1 = 0.01;
GLfloat delta2 = 0.1;
GLfloat flyZ = 0;
GLfloat dir = 0;

GLint move1, move2, move3, move4, move5;

GLboolean rotate = GL_FALSE;
GLboolean fly = GL_FALSE;
GLboolean jump = GL_FALSE;

GLUquadricObj *obj;

//shading...
typedef struct materialStruct {
  GLfloat ambient[4];
  GLfloat diffuse[4];
  GLfloat specular[4];
  GLfloat shininess;
} materialStruct;

materialStruct smoothMaterials = {
   {0.0, 0.0, 0.0, 1.0},
   {0.0, 0.0, 0.0, 1.0},
   {1.0, 1.0, 1.0, 1.0},
   70
};

materialStruct roughMaterials = {
   {0.6, 0.0, 0.0, 0.6},
   {1.0, 0.0, 0.0, 1.0},
   {0.0, 0.0, 0.0, 1.0},
   0
};

materialStruct wingMaterials = {
   {0.55, 1., 1.0, 0.6},
   {0.55, 1., 1.0, 1.0},
   {1.0, 1.0, 1.0, 1.0},
   0
};

materialStruct *currentMaterials;

typedef struct lightingStruct {
  GLfloat ambient[4];
  GLfloat diffuse[4];
  GLfloat specular[4];
} lightingStruct;

lightingStruct whiteLighting = {
   {0.0, 0.0, 0.0, 1.0},
   {1.0, 1.0, 1.0, 1.0},
   {1.0, 1.0, 1.0, 1.0}
};

lightingStruct coloredLighting = {
   {0.0, 0.0, 0.0, 0.0},
   {0.0, 1.0, 0.0, 1.0},
   {0.0, 0.0, 0.0, 0.6}
};

GLfloat light0_pos[4] = {-3.0, 0.0, .0, 0.0};
GLfloat light1_pos[4] = {-5.0, 1.0, -5.0, 0.0};


lightingStruct *currentLighting;

//individual parts of the Dragonfly

void df_head()
{
   glPushMatrix();
   glScalef (HEAD_X_RADIUS, HEAD_Y_RADIUS, HEAD_Z_RADIUS);
   gluSphere(obj, 1, 30, 30);
   glPopMatrix();
}

void lowBody()
{
   glPushMatrix();
   glScalef (LOW_BODY_X_RADIUS, LOW_BODY_Y_RADIUS, LOW_BODY_Z_RADIUS);
   gluSphere(obj, 1, 30, 30);
   glPopMatrix();
}

void df_tail()
{
   glPushMatrix();
   glScalef (UP_BODY_X_RADIUS, UP_BODY_RADIUS, UP_BODY_RADIUS);
   gluSphere(obj, 1, 30, 30);
   glPopMatrix();
}

void df_eye()
{
   glPushMatrix();
   glScalef (EYE_RADIUS, EYE_RADIUS, EYE_RADIUS);
   gluSphere(obj, 1, 30, 20);
   glPopMatrix();
}

void df_auxLeftLeg()
{
   glPushMatrix();
   glTranslatef (0.0, 0.5*AUX_LEG_LEN, 0.0);
   glRotatef(-45.0, 1.0, 0.0, 0.0);
   glRotatef(-45.0, 0.0, 1.0, 0.0);
   glRotatef(45.0, 0.0, 0.0, 1.0);
   gluCylinder(obj, AUX_LEG_RADIUS, 0.5*AUX_LEG_RADIUS, AUX_LEG_LEN, 30, 30);
   glPopMatrix();
}

void df_knee()
{
   glPushMatrix();
   gluSphere(obj, KNEE_RADIUS, 20, 20);
   glPopMatrix();
}

void df_auxFoot()
{
   glPushMatrix();
   glTranslatef (0.0, -0.45, 0.0);
   glRotatef(90.0, 1.0, 0.0, 0.0);
   glutWireCone(AUX_FOOT_RADIUS, AUX_FOOT_LEN, 30, 30);
   glPopMatrix();
}

void df_auxRightLeg()
{
   glPushMatrix();
   glTranslatef (0.0, 0.5*AUX_LEG_LEN, 0.0);
   glRotatef(45.0, 1.0, 0.0, 0.0);
   glRotatef(-135.0, 0.0, 1.0, 0.0);
   glRotatef(-45.0, 0.0, 0.0, 1.0);
   gluCylinder(obj, AUX_LEG_RADIUS, 0.5*AUX_LEG_RADIUS, AUX_LEG_LEN, 30, 30);
   glPopMatrix();
}

void df_leftLowLeg()
{
   glPushMatrix();
   glRotatef(-30.0, 0.0, 1.0, 0.0);
   glRotatef(-50.0, 1.0, 0.0, 0.0);
   gluCylinder(obj, LOW_LEG_LOW_RADIUS, LOW_LEG_UP_RADIUS, LOW_LEG_LEN, 30, 30);
   glPopMatrix();
}

void df_leftMidLeg()
{
   glPushMatrix();
   glRotatef(-35.0, 0.0, 1.0, 0.0);
   glRotatef(45.0, 1.0, 0.0, 0.0);
   gluCylinder(obj, MIDDLE_LEG_LOW_RADIUS, MIDDLE_LEG_UP_RADIUS, MIDDLE_LEG_LEN, 30, 30);
   glPopMatrix();
}

void df_leftUpLeg()
{
   glPushMatrix();
   glRotatef(-40.0, 0.0, 1.0, 0.0);
   glRotatef(75.0, 1.0, 0.0, 0.0);
   gluCylinder(obj, UP_LEG_LOW_RADIUS, UP_LEG_UP_RADIUS, UP_LEG_LEN, 30, 30);
   glPopMatrix();
}

void df_leftFoot()
{
   glPushMatrix();
   glRotatef(-50.0, 0.0, 1.0, 0.0);
   glRotatef(25.0, 1.0, 0.0, 0.0);
   glutWireCone(FOOT_RADIUS, FOOT_LEN, 30, 30);
   glPopMatrix();
}

void df_rightLowLeg()
{
   glPushMatrix();
   glRotatef(30.0, 0.0, 1.0, 0.0);
   glRotatef(-130.0, 1.0, 0.0, 0.0);
   gluCylinder(obj, LOW_LEG_LOW_RADIUS, LOW_LEG_UP_RADIUS, LOW_LEG_LEN, 30, 30);
   glPopMatrix();
}

void df_rightMidLeg()
{
   glPushMatrix();
   glRotatef(35.0, 0.0, 1.0, 0.0);
   glRotatef(135.0, 1.0, 0.0, 0.0);
   gluCylinder(obj, MIDDLE_LEG_LOW_RADIUS, MIDDLE_LEG_UP_RADIUS, MIDDLE_LEG_LEN, 30, 30);
   glPopMatrix();
}

void df_rightUpLeg()
{
   glPushMatrix();
   glRotatef(40.0, 0.0, 1.0, 0.0);
   glRotatef(105.0, 1.0, 0.0, 0.0);
   gluCylinder(obj, UP_LEG_LOW_RADIUS, UP_LEG_UP_RADIUS, UP_LEG_LEN, 30, 30);
   glPopMatrix();
}

void df_rightFoot()
{
   glPushMatrix();
   glRotatef(50.0, 0.0, 1.0, 0.0);
   glRotatef(155.0, 1.0, 0.0, 0.0);
   glutWireCone(FOOT_RADIUS, FOOT_LEN, 30, 30);
   glPopMatrix();
}

void df_wing()
{
   glPushMatrix();
   glScalef (0.7, 0.7, 4.7);
   gluSphere(obj, 1, 20, 20);
   glPopMatrix();
}

//the whole Dragonfly

void Dragonfly()
{
   glTranslatef(flyX, jumpY, flyZ);
   glRotatef(theta[4], 0.0, 1.0, 0.0);

   currentMaterials = &smoothMaterials;
   glMaterialfv(GL_FRONT, GL_AMBIENT, currentMaterials -> ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, currentMaterials -> diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, currentMaterials -> specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, &currentMaterials -> shininess);

   df_head();

   glPushMatrix();
   glTranslatef(2.5, 0.5, 0.0);
 
   currentMaterials = &smoothMaterials;
   glMaterialfv(GL_FRONT, GL_AMBIENT, currentMaterials -> ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, currentMaterials -> diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, currentMaterials -> specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, &currentMaterials -> shininess);

   lowBody();
   glTranslatef(5.2, 0.0, 0.0);

   currentMaterials = &smoothMaterials;
   glMaterialfv(GL_FRONT, GL_AMBIENT, currentMaterials -> ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, currentMaterials -> diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, currentMaterials -> specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, &currentMaterials -> shininess);
  
   df_tail();
   glPopMatrix();
   
   //left df_eye
   glPushMatrix();
   glTranslatef(-1.5, 1.0, 1.5);

   currentMaterials = &roughMaterials;
   glMaterialfv(GL_FRONT, GL_AMBIENT, currentMaterials -> ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, currentMaterials -> diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, currentMaterials -> specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, &currentMaterials -> shininess);
 
   df_eye();
   //right df_eye
   glTranslatef(0.0, 0.0, -3.0);

   currentMaterials = &roughMaterials;
   glMaterialfv(GL_FRONT, GL_AMBIENT, currentMaterials -> ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, currentMaterials -> diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, currentMaterials -> specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, &currentMaterials -> shininess);

   df_eye();
   glPopMatrix();
   
   //left aux leg+foot
   glPushMatrix();
   glTranslatef(-0.4, -2.0, 1.0);
   glRotatef(theta[1], 0.0, 0.0, 1.0);

   currentMaterials = &smoothMaterials;
   glMaterialfv(GL_FRONT, GL_AMBIENT, currentMaterials -> ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, currentMaterials -> diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, currentMaterials -> specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, &currentMaterials -> shininess);
 
   df_auxLeftLeg();

   glTranslatef(-1.0, 1.95, 0.7);
   glRotatef(theta[2], 0.0, 0.0, 1.0);

   currentMaterials = &smoothMaterials;
   glMaterialfv(GL_FRONT, GL_AMBIENT, currentMaterials -> ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, currentMaterials -> diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, currentMaterials -> specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, &currentMaterials -> shininess);

   df_auxFoot();

   glTranslatef(0.0, -0.5, 0.0);
   df_knee();
   glPopMatrix();

   //right aux leg+foot
   glPushMatrix();
   glTranslatef(-0.4, -2.0, -1.0);
   glRotatef(-theta[1], 0.0, 0.0, 1.0);

   currentMaterials = &smoothMaterials;
   glMaterialfv(GL_FRONT, GL_AMBIENT, currentMaterials -> ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, currentMaterials -> diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, currentMaterials -> specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, &currentMaterials -> shininess);

   df_auxRightLeg();

   glTranslatef(-1.0, 1.95, -0.7);
   glRotatef(-theta[2], 0.0, 0.0, 1.0);

   currentMaterials = &smoothMaterials;
   glMaterialfv(GL_FRONT, GL_AMBIENT, currentMaterials -> ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, currentMaterials -> diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, currentMaterials -> specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, &currentMaterials -> shininess);

   df_auxFoot();

   glTranslatef(0.0, -0.5, 0.0);
   df_knee();
   glPopMatrix();
   
   //left first leg
   glPushMatrix();
   //df_leftLeg();
   glRotatef(theta[3], 1.0, 0.0, 0.0);
   glTranslatef(0.5, -1.0, 1.0);
   df_leftLowLeg();
   
   glTranslatef(-0.9, 2.25, 1.5);
   glRotatef(theta[5], 0.0, 1.0, 1.0);
   df_leftMidLeg();
   
   glTranslatef(-1.1, -1.9, 1.58);
   df_leftUpLeg();
   
   glTranslatef(-0.22, -1.45, 0.27);
   glRotatef(-theta[6], 0.0, 0.0, 1.0);
   df_leftFoot();

   glPopMatrix();
   
   //right first leg
   glPushMatrix();
   glRotatef(theta[3], 1.0, 0.0, 0.0);
   glTranslatef(0.5, -1.0, -1.0);
   df_rightLowLeg();
   
   glTranslatef(-0.9, 2.25, -1.5);
   glRotatef(-theta[5], 0.0, 1.0, 1.0);
   df_rightMidLeg();
   
   glTranslatef(-1.1, -1.9, -1.58);
   df_rightUpLeg();
   
   glTranslatef(-0.22, -1.45, -0.27);
   glRotatef(-theta[6], 0.0, 0.0, 1.0);
   df_rightFoot();

   glPopMatrix();

   // left wing
   glPushMatrix();
   glTranslatef(0.2, 2.0, 3.7);
   glRotatef(theta[7], 1.0, 0.0, 0.0);
 
   currentMaterials = &wingMaterials;
   glMaterialfv(GL_FRONT, GL_AMBIENT, currentMaterials -> ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, currentMaterials -> diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, currentMaterials -> specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, &currentMaterials -> shininess);

   df_wing();
   glTranslatef(2.0, 0.0, 0);
   glRotatef(20, 0, 1, 0);
   glRotatef(theta[8], 1.0, 0.0, 0.0);
   df_wing();
   glPopMatrix();

   // right wing
   glPushMatrix();
   glTranslatef(0.2, 2.0, -3.7);
   glRotatef(-theta[7], 1.0, 0.0, 0.0);

   currentMaterials = &wingMaterials;
   glMaterialfv(GL_FRONT, GL_AMBIENT, currentMaterials -> ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, currentMaterials -> diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, currentMaterials -> specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, &currentMaterials -> shininess);

   df_wing();
   glTranslatef(2.0, 0.0, 0);
   glRotatef(-20, 0, 1, 0);
   glRotatef(-theta[8], 1.0, 0.0, 0.0);
   df_wing();
   glPopMatrix();

}

//display function

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    gluLookAt(-1, 1, 1, 0, 0, 0, 0.0, 1.0, 0.0);

    glRotatef(theta[0], 0.0, 1.0, 0.0);

    if (relative){
        glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    }

    Dragonfly();

    glutSwapBuffers();
}

void mymouse(GLint button, GLint state, GLint x, GLint y) {
    if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){ 
        rotate = !rotate;
        jump = !jump;
    }
}


//init function

void init()
{
    int viewpoint = 0;

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, viewpoint);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    currentLighting = &whiteLighting;

    glLightfv(GL_LIGHT0, GL_AMBIENT, currentLighting -> ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, currentLighting -> diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, currentLighting -> specular);

    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    obj = gluNewQuadric();
}

//reshape function

void reshape(int w, int h)
{
    glViewport (0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();  
    glOrtho(-15.0, 15.0, -15.0, 15.0, -15.0, 15.0);
}

//animation

void idle()
{
    if (rotate) {
        theta[0] += 0.2;
        if (theta[0] > 360) theta[0] -= 360;
    }

    if (fly) {
        if (dir == 0){
            flyX -= 0.02;
            if(flyX < -8){
                dir = 0.5;
                theta[4] += 1;
            }
        } else if (dir == 0.5){
            theta[4] += 1;
            if(theta[4]>60){
                dir = 1;
                flyX -= 0.02;
                flyZ += 0.02;
            }
        } else if (dir == 1){
            flyX -= 0.02;
            flyZ += 0.02;
            if(flyX < -11){
                dir = 1.5;
                theta[4] += 1;
            }
        } else if (dir == 1.5){
            theta[4] += 1;
            if(theta[4] > 120){
                dir = 2;
                flyX += 0.02;
                flyZ += 0.02;
            }
        } else if (dir == 2){
            flyX += 0.02;
            flyZ += 0.02;
            if (flyX > -8){
                dir = 2.5;
                theta[4] += 1;
            }
        } else if (dir == 2.5){
            theta[4] += 1;
            if (theta[4] > 180){
                dir = 3;
                flyX += 0.02;
            }
        } else if (dir == 3){
            flyX += 0.02;
            if (flyX > 0){
                dir = 3.5;
                theta[4] += 1;
            }
        } else if (dir == 3.5){
            theta[4] += 1;
            if (theta[4] > 240){
                dir = 4;
                flyX += 0.02;
                flyZ -= 0.02;
            }
        } else if (dir == 4){
            flyX += 0.02;
            flyZ -= 0.02;
            if (flyX > 3){
                dir = 4.5;
                theta[4] += 1;
            }
        } else if (dir == 4.5){
            theta[4] += 1;
            if (theta[4] > 300){
                dir = 5;
                flyX -= 0.02;
                flyZ -= 0.02;
            }
        } else if (dir == 5){
            flyX -= 0.02;
            flyZ -= 0.02;
            if (flyX < 0){
                dir = 5.5;
                theta[4] += 1;
            }
        } else if (dir == 5.5){
            theta[4] += 1;
            if (theta[4] > 360){
                dir = 0;
                theta[4] = 0;
            }
        }
    }

    //jump aux legs
    if (jump) {
        if (move1 > 0) {
            theta[1] += 0.5;
            if (theta[1] > 10) {
                move1 = -1;
                theta[1] -= 0.5;
            }
        } else {
            theta[1] -= 0.5;
            if (theta[1] < -20) {
                move1 = 1;
                theta[1] += 0.5;
            }
        }

        if (move2 > 0) {
            theta[2] += 0.1;
            if (theta[2] > 5) {
                move2 = -1;
                theta[2] -= 0.1;
            }
        } else {
            theta[2] -= 0.1;
            if (theta[2] < -5) {
                move2 = 1;
                theta[2] += 0.1;
            }
        }

        //lower leg
        if (move3 > 0) {
            theta[3] += 0.1;
            if (theta[3] > 7) {
                move3 = -1;
                theta[3] -= 0.1;
            }
        } else {
            theta[3] -= 0.1;
            if (theta[3] < -4) {
                move3 = 1;
                theta[3] += 0.1;
            }
        }

        //middle leg
        if(move4 > 0) {
            theta[5] += 0.25;
            if (theta[5] >12.5){
                move4 = -1;
                theta[5] -= 0.25;
            }
        } else{
            theta[5] -= 0.25;
            if(theta[5] < -12.5){
                move4 = 1;
                theta[5] += 0.25;
            }
        }
        //foot
        if(move5 > 0) {
            theta[6] += 0.5;
            if (theta[6] >0){
                move5 = -1;
                theta[6] -= 0.5;
            }
        } else{
            theta[6] -= 0.5;
            if(theta[6] < -50){
                move5 = 1;
                theta[6] += 0.5;
            }
        }

        if (jumpY > 1.0 || jumpY < 0.0) {
            delta1 *= -1;
        }
        if ( theta[7] > 5 || theta[7] < -2) {
            delta *= -1;
        }
        if ( theta[8] > 4 || theta[7] < -1) {
            delta2 *= -1;
        }

        jumpY += delta1;

        theta[7] += delta;
        theta[8] += delta2;
    }

    glutPostRedisplay();
}

//menu function
void mymenu(int id)
{
    if(id == 1) {
        fly = !fly;
    }
    if(id == 2) {
        jump = !jump;
    }
    if(id == 3) {
        rotate =!rotate;
    }
}

void mysubmenu1(int id)
{
    if(id == 1) relative = GL_FALSE;
    if(id == 2) relative = GL_TRUE;
}
void mysubmenu2(int id)
{
    if(id == 1){ 
        float a = 0.8;
        float model_ambient[] = {a, a, a, 1.0f};
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);
        glDisable(GL_LIGHT0);
    }
    if(id == 2){
        float a = 0.0;
        float model_ambient[] = {a, a, a, 1.0f};
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);
        glEnable(GL_LIGHT0);
    }

}
void mysubmenu3(int id)
{	
    if(id == 1)
    {
        currentLighting = &whiteLighting;

        glLightfv(GL_LIGHT0, GL_AMBIENT, currentLighting -> ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, currentLighting -> diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, currentLighting -> specular);
        glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);

        glDisable(GL_LIGHT1);
        glEnable(GL_LIGHT0);
    }
    if(id == 2) {
        currentLighting = &coloredLighting;

        glLightfv(GL_LIGHT1, GL_AMBIENT, currentLighting -> ambient);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, currentLighting -> diffuse);
        glLightfv(GL_LIGHT1, GL_SPECULAR, currentLighting -> specular);
        glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);

        glDisable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
    }
}


//main function

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Dragonfly");
    glutIdleFunc(idle);
    glutMouseFunc(mymouse);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glEnable(GL_DEPTH_TEST); /* Enable hidden-surface removal */
    init();

    submenu_id1 = glutCreateMenu(mysubmenu1);
    glutAddMenuEntry("fixed", 1);
    glutAddMenuEntry("relative", 2);
    submenu_id2 = glutCreateMenu(mysubmenu2);
    glutAddMenuEntry("ambient", 1);
    glutAddMenuEntry("distant", 2);
    submenu_id3 = glutCreateMenu(mysubmenu3);
    glutAddMenuEntry("white", 1);
    glutAddMenuEntry("colored", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    menu_id = glutCreateMenu(mymenu);
    glutAddSubMenu("Light position", submenu_id1);
    glutAddSubMenu("Light type", submenu_id2);
    glutAddSubMenu("Light properties", submenu_id3);
    glutAddMenuEntry("Flying", 1);
    glutAddMenuEntry("Jumping", 2);
    glutAddMenuEntry("Rotating", 3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();
}
