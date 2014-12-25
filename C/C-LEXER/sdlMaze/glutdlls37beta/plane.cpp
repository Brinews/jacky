#include<GL/glut.h>  
#include<iostream>  
#include<string>  
#include<cmath>  
#include<windows.h>  
using namespace std;  
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")  
static float Pi = 3.1415926;  
static float lookat_r = 10; //观察点旋转半径  
static float height = 8;        //观察点高度  
static float angle = 0;     //观察点起始角度  
static GLfloat head[] = {0, 0, 0};    

//类圆柱  
void myCylinder(GLfloat r, GLfloat front[], GLfloat length)  
{  
	GLfloat front_x = front[0];  
	GLfloat behind_x = front[0] + length;  
	GLfloat center_y = front[1];  
	GLfloat center_z = front[2];  


	glBegin(GL_QUAD_STRIP);  
	for(GLfloat i=0; i<=2 * Pi; i += 0.01)  
	{  
		GLfloat y = center_y + r * cos(i);  
		GLfloat z = center_z + r * sin(i);  
		glVertex3f(front_x, y, z);  
		glVertex3f(behind_x, y*1.1, z*1.1);  
	}  
	glEnd();  
}  

//椭球  
void myEllipsoid(GLfloat Rx, GLfloat Ry, GLfloat Rz)  
{  
	GLfloat Cx = head[0] + 2.8,   
			Cy = head[1],   
			Cz = head[2] + 0.3;  

	glBegin(GL_POLYGON);  

	for(GLfloat angle_b = 0; angle_b < 2*Pi; angle_b += Pi/56)  
		for(GLfloat angle_a = 0; angle_a < 2*Pi; angle_a += Pi/28)  
		{  
			GLfloat x = Rx * sin(angle_b) * cos(angle_a) + Cx,  
					y = Ry * sin(angle_b) * sin(angle_a) + Cy,  
					z = Rz * cos(angle_b) + Cz;  
			glVertex3f(x, y, z);  
		}  
	glEnd();  

}  

//喷管  
void Pipe(GLfloat head[])  
{  
	static GLfloat length = 0.3;        //喷管长度  
	static GLfloat r = 0.4;             //喷管半径  

	GLfloat left_pipe_front[] = {head[0] + 10, head[1] - 0.5, head[2]};  
	GLfloat right_pipe_front[] = {left_pipe_front[0], head[1] + 0.5, head[2]};  
	glColor3f(0.4, 0.4, 0.4);  
	myCylinder(r, left_pipe_front, length);  
	myCylinder(r, right_pipe_front, length);  
}  

//求绝对值  
GLfloat GLabs(GLfloat a)  
{  
	return a > 0 ? a : -a;  
}  

//转向舵  
void Rudder(GLfloat head[], int direction)  
{  
	GLfloat x1 =  8,  
			x2 =  9.5,  
			y1 = 1.0,  
			y2 = 1.3,                 
			z = head[2] + 0.4,  

			left_up[]= {head[0] + x1, head[1] + y1, z},  
			right_low[] = {head[0] + x2, head[1] + y2, z},  
			height = 1.5;  

	if(direction ==1)  
	{  
		left_up[1] = head[1] -y1;  
		right_low[1] = head[1] -y2;  
	}  

	GLfloat left_low[3] = {right_low[0], left_up[1], left_up[2]},  
			right_up[3] = {left_up[0], right_low[1], left_up[2]},  
			center_y = (right_up[1] - left_up[1]),  
			factor = (right_low[0] - left_up[0])*0.1,  
			ahead[3] = {left_up[0] + factor + 1.5, left_up[1] - 2*GLabs(center_y), left_up[2] + height};      
	if(direction == 2)  
		ahead[1] = right_up[1] + center_y;  
	GLfloat behind[3] = {ahead[0] + 0.8, ahead[1], left_up[2] + height};  

	glColor3f(0.75, 0.75, 0.75);  
	//机翼左面  
	glBegin(GL_POLYGON);  
	glVertex3fv(left_up);  
	glVertex3fv(left_low);  
	glColor3f(1, 1, 1);  
	glVertex3fv(behind);  
	glVertex3fv(ahead);  
	glEnd();  

	glColor3f(0.75, 0.75, 0.75);      
	//机翼右面  
	glBegin(GL_POLYGON);  
	glVertex3fv(right_up);  
	glVertex3fv(right_low);  
	glColor3f(1, 1, 1);  
	glVertex3fv(behind);  
	glVertex3fv(ahead);  
	glEnd();  

	glColor3f(0.6, 0.6, 0.6);     
	//机翼前面  
	glBegin(GL_POLYGON);  
	glVertex3fv(right_up);  
	glVertex3fv(left_up);  
	glVertex3fv(ahead);  
	glEnd();  

	//机翼后面  
	glBegin(GL_POLYGON);  
	glVertex3fv(right_low);  
	glVertex3fv(left_low);  
	glVertex3fv(behind);  
	glEnd();  
}  

//机翼  
void Wing(GLfloat head[],int direction)   
{  
	GLfloat front[] = {head[0]+5.5, head[1] +1.5, head[2] + 0.6};         
	if(direction == 1)  
		front[1] = head[1] -1.5;  
	GLfloat back[] = {head[0] + 10, front[1], front[2]};  
	GLfloat Wing_height = front[2];  

	GLfloat x1, x2, x3, x4, x5, y1, y2, y3, y4, y5;  
	x1 = front[0];  y1 = front[1];  
	x2 = back[0];   y2 = back[1];  
	x3 = x1 + (x2 - x1) * 0.6667;       y3 = y1 - GLabs((x3 - x1)) * 4 / 3;   
	x5 = x2 - (x2 - x1)/10;   
	x4 = (x5 - x3) / 2 +x3;  
	y4 = y3;  
	y5 = y4 + (x4 - x3);  
	if(direction == 2)  
	{  
		y3 = y1 + GLabs((x3 - x1)) * 4 / 3;  
		y4 = y3;  
		y5 = y4 - GLabs(x4 - x3);  
	}  

	glBegin(GL_POLYGON);  
	glColor3f(0.4, 0.47, 0.42);  
	glVertex3f(x2, y2, Wing_height);  
	glVertex3f(x1 - 1.1, y1, Wing_height);  
	if(direction == 1)  
		glVertex3f(x1+0.5, y1 - 0.5, Wing_height);  
	else  
		glVertex3f(x1+0.5, y1 + 0.5, Wing_height);  
	glColor3f(0.9, 0.9, 0.9);  
	glVertex3f(x3, y3, Wing_height);  
	glVertex3f(x4, y4, Wing_height);  
	glVertex3f(x5, y5, Wing_height);  
	glEnd();  
}  

//坐标系  
void Coordinate()  
{  
	//坐标系  
	glBegin(GL_LINES);  
	glColor3f(1, 1, 1);     glVertex3f(0, 0, 0);  
	glColor3f(1, 0, 0);     glVertex3f(15, 0, 0);  
	glColor3f(1, 1, 1);     glVertex3f(0, 0, 0);  
	glColor3f(0, 1, 0);     glVertex3f(0, 15, 0);  
	glColor3f(1, 1, 1);     glVertex3f(0, 0, 0);  
	glColor3f(0, 0, 1);     glVertex3f(0, 0, 15);     
	glEnd();  
}  

//机身  
void Body(GLfloat head[])  
{  
	GLfloat x1 = head[0] + 4,  
			x2 = x1 + 0.5,  
			x4 = head[0] + 10,  
			y1 = head[1] - 1,           y2 = head[1] + 1,         
			y3 = head[1] - 1.6,     y4 = head[1] + 1.6,       
			y5 = head[1] - 0.4,     y6 = head[1] + 0.4,   
			z1 = head[2] + 0.6,     z2 = head[2] - 0.6,  
			Coord1[] =      {x1, y1, z1},               Coord2[] =      {x1, y2, z1},  
			Coord3[] =      {x2, y3, z1},               Coord4[] =      {x2, y4, z1},  
			Coord5[] =      {x1, y5, z2},               Coord6[] =      {x1, y6, z2},  
			Coord7[] =      {x2, y1, z2},               Coord8[] =      {x2, y2, z2},  
			Coord9[] =      {x4, y3, z1},               Coord10[] =     {x4, y4, z1},  
			Coord11[] =     {x4, y1, z2 + 0.2},  
			Coord12[] =     {x4, y2, Coord11[2]},  
			Coordk1[] =     {x1, head[1], z1},  
			Coordk2[] =     {x4 -1, head[1], z1},  
			Coordk3[] =     {x4, head[1] - 0.2, z1},  
			Coordk4[] =     {x4, head[1] + 0.2, z1},  
			Coordk5[] =     {x4, head[1], z1 - 0.2},  
			Coordk6[] =     {x4, head[1], Coord12[2]};  
	//上  
	glBegin(GL_POLYGON);      
	glColor3f(0.4, 0.47, 0.42);  
	glVertex3fv(Coordk1);       glVertex3fv(Coord2);  
	glVertex3fv(Coord4);        glVertex3fv(Coord10);  
	glVertex3fv(Coordk4);       glVertex3fv(Coordk2);  
	glVertex3fv(Coordk1);         
	glEnd();  

	glBegin(GL_POLYGON);  
	glVertex3fv(Coordk1);       glVertex3fv(Coord1);  
	glVertex3fv(Coord3);        glVertex3fv(Coord9);  
	glVertex3fv(Coordk3);       glVertex3fv(Coordk2);  
	glVertex3fv(Coordk1);  
	glEnd();  

	glColor3f(0.3, 0.37, 0.32);  
	glBegin(GL_POLYGON);      
	glVertex3fv(Coordk2);       glVertex3fv(Coordk3);  
	glVertex3fv(Coordk5);  
	glEnd();  

	glBegin(GL_POLYGON);      
	glVertex3fv(Coordk2);       glVertex3fv(Coordk4);  
	glVertex3fv(Coordk5);  
	glEnd();  

	//左  
	glBegin(GL_POLYGON);  
	glColor3f(0.6, 0.6, 0.6);  
	glVertex3fv(Coord3);        glVertex3fv(Coord7);  
	glVertex3fv(Coord11);       glVertex3fv(Coord9);  
	glEnd();  
	//右  
	glBegin(GL_POLYGON);  
	glColor3f(0.6, 0.6, 0.6);  
	glVertex3fv(Coord4);        glVertex3fv(Coord8);  
	glVertex3fv(Coord12);       glVertex3fv(Coord10);  
	glEnd();  
	//下  
	glBegin(GL_POLYGON);  
	glColor3f(0.9, 0.9, 0.9);  
	glVertex3fv(Coord5);        glVertex3fv(Coord6);  
	glVertex3fv(Coord8);        glVertex3fv(Coord12);  
	glVertex3fv(Coord11);       glVertex3fv(Coord7);  
	glEnd();  
	//后  
	glBegin(GL_POLYGON);  
	glColor3f(0.8, 0.8, 0.8);  
	glVertex3fv(Coord9);        glVertex3fv(Coordk3);  
	glVertex3fv(Coordk5);       glVertex3fv(Coordk6);  
	glVertex3fv(Coord11);  
	glEnd();  
	glBegin(GL_POLYGON);  
	glVertex3fv(Coordk4);       glVertex3fv(Coordk5);  
	glVertex3fv(Coordk6);       glVertex3fv(Coord12);  
	glVertex3fv(Coord10);  
	glEnd();  
	//前  
	glColor3f(0.3867,0.4063,0.4297);    //歼20涂装色  
	glBegin(GL_POLYGON);  
	glVertex3fv(Coord1);        glVertex3fv(Coord5);  
	glVertex3fv(Coord2);        glVertex3fv(Coord6);  
	glEnd();  
	//左进气道  
	glColor3f(0.3, 0.4, 0.3);  
	glBegin(GL_POLYGON);  
	glVertex3fv(Coord1);  
	glVertex3f(Coord1[0] + 5, Coord1[1],  Coord1[2]);  
	glVertex3f(Coord5[0] + 5, Coord5[1],  Coord5[2] + 0.15);  
	glVertex3fv(Coord5);  
	glEnd();  
	//右进气道  
	glColor3f(0.3, 0.4, 0.3);  
	glBegin(GL_POLYGON);  
	glVertex3fv(Coord2);  
	glVertex3f(Coord2[0] + 5, Coord2[1],  Coord2[2]);  
	glVertex3f(Coord6[0] + 5, Coord6[1],  Coord6[2] + 0.15);  
	glVertex3fv(Coord6);  
	glEnd();  
}  

GLfloat get_ldz_R(GLfloat x)  
{  
	return log( x+ ( 1 - head[0])) * 0.5;  
}  

//机头  
void Head(GLfloat head[])  
{  
	/* 机头主干 */  
	glBegin(GL_LINE_LOOP);    
	for(GLfloat x = 0; x < 4; x+=0.01)  
	{  
		GLfloat factor = 1 - x /8;  
		glColor3f(factor, factor, factor);  
		GLfloat R = get_ldz_R(x) * 0.8;       
		for( GLfloat this_angle =0; this_angle <= 2*Pi; this_angle += 0.1)  
		{             
			GLfloat y = head[1] + R * cos(this_angle),  
					z = head[2] + R * sin(this_angle)*0.95;  
			glVertex3f(x*0.9375 + 0.25, y, z);  
		}                      
	}  
	glEnd();  

	/* 整流罩 */  
	glColor4f(1, 0.3, 0.3, 0.5);  
	myEllipsoid(1.2, 0.5, 0.5);  
}  

//合成飞机  
void planeMode()  
{  
	glEnable(GL_DEPTH_TEST);//启用透视  
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除颜色 | 透视  
	glMatrixMode(GL_PROJECTION);    //指定当前矩阵是下次矩阵操作的目标  
	glLoadIdentity();   //重置当前矩阵为单位矩阵（与上一个glMatrixMode一同使用）  
	gluPerspective(80, 1, 0.1, 30);//“焦距” | “x/y”| “离近处的距离” | “离远出的距离”  
	glMatrixMode(GL_MODELVIEW);  
	glLoadIdentity();   //GL_PROJECTION是对投影矩阵操作，GL_MODELVIEW是对模型视景矩阵操作，GL_TEXTURE是对纹理矩阵进行随后的操作  
	gluLookAt(lookat_r*cos(angle) + 5, lookat_r*sin(angle), height, 5, 0, 0, 0, 0, 1);  //眼睛位置坐标 | 视觉中心位置坐标 | 观察着方向坐标  
	//gluLookAt(5, -8, 8, 5, 0, 0, 0, 0, 1);  
	Coordinate(); //这里是坐标系  

	Wing(head, 1);  
	Wing(head, 2);  
	Rudder(head, 1);  
	Rudder(head, 2);  
	Body(head);  
	Pipe(head);  
	Head(head);  

	glutSwapBuffers();  
	glFlush();  
}  

//旋转  
void Spin()  
{  
	angle += 0.01;  
	Sleep(20);  
	if(angle >=2*Pi )  
		angle = 0;  
	planeMode();  
}  

int main(int argc, char *argv[])  
{  
	glutInit(&argc, argv);  
	glutInitDisplayMode(GL_COLOR_BUFFER_BIT);  
	glutInitWindowPosition(100, 100);  
	glutInitWindowSize(800, 800);  
	glutCreateWindow("鹏哥的飞机模型");  
	glutDisplayFunc(&planeMode);  
	glutIdleFunc(&Spin);  
	glutMainLoop();  
	return 0;  
}  

