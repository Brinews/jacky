#include<stdio.h>//输入输出函数 
#include<malloc.h>//动态存储分配函数 
#include<time.h>//时间函数
#include<stdlib.h>//数据转换，改变程序进程 #include<time.h>//时间函数?
#include<conio.h>//键盘产生的输入函数 
#include<windows.h> //windows头文件，含显示彩色文本的函数 //定义单链表，表示蛇或随机点

typedef struct point//声明结构体并命名为POINT1 
{ 
	int x;//横坐标
	int y;//纵坐标
	struct point *next;//链节，指针成员指向与struct point类型相同的结构?
}POINT1;

//定义全局变量 
POINT1 rands[5],*head=NULL;//定义一个结构体数组用来储存5个随机点，一个结构体指针head用来储存蛇的长度 
int key,direction=3,level=1;//整型变量，key储存键盘输入的方向，初始化蛇前进的方向，游戏等级为1 
clock_t start,end;//测量一个大循环运行所花费的时间，返回时间，若失败返回-1

HANDLE hCon;//句柄，设置输入或输出文本的文本颜色?//函数声明?

void gotoxy(int x, int y);//设置光标位置?
void born(POINT1 *head,int direction);//在前进方向生成一个节点?
void collision(POINT1 *head);//判断是否发生碰撞（自身或边界）
void pointrands(POINT1 *node,int k);//生成一个随机点?
void delnode(POINT1 *head);//删除蛇的最后一个节点?
void viewhead(POINT1 *head);//画蛇?
void viewrands(POINT1 *node);//画随机点
void HideCursor();//隐藏光标
void viewlevel(void); //更新状态栏
int  eat(POINT1 *head); //判断是否吃掉随机点

int main()
{
	POINT1 *p = NULL, *q = NULL;
	int i, spend, movetime = 700;

	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	system("color 0E");
	system("mode con:cols=40 lines=23");
	system("title Snake");

	gotoxy(0, 20);

	SetConsoleTextAttribute(hCon, 10);
	printf("█████");
	HideCursor();

	head = (POINT1 *)malloc(sizeof(POINT));
	head->x = 0;
	p = head;
	for (i = 1; i <= 3; i++) {
		q = (POINT1 *) malloc(sizeof(POINT1));
		q->x = 11 + i;
		q->y = 13;
		p->next = q;
		p = q;
		head->x++;
	}

	p->next = NULL;
	viewhead(head);
	viewlevel();

	srand((unsigned int) time(NULL));

	for (i = 0; i < 5; i++) {
		pointrands(&rands[i], i);
		viewrands(&rands[i]);
	}

	start = clock();
	while (1) {
		if (kbhit()) {
			key = getch();
			switch(key) {
				case 72: direction = 1; break;
				case 80: direction = 2; break;
				case 75: direction = 3; break;
				case 77: direction = 4; break;
				default: continue;
			}

			born(head, direction);

			if (!eat(head)) {
				delnode(head);
				collision(head);
			}
			else {
				if (head->x%20 == 0) {
					level++;
					movetime = movetime - 100;
				}
				viewlevel();
			}
			
			viewhead(head);
			start = clock();
		}

		end = clock();

		spend = (end-start)*1000/CLOCKS_PER_SEC;

		if (spend >= movetime) {
			born(head, direction);

			if (!eat(head)) {
				delnode(head);
				collision(head);
				viewhead(head);
			}
			else {
				if (head->x%20 == 0) {
					level++;
					movetime = movetime - 100;
				}
				viewlevel();
			}
			start = clock();
		}
		if (head->x >= 70)
			break;
	}

	return 0;
}


void born(POINT1 *head, int diretion)
{
	POINT1 *p = NULL;
	p = (POINT1 *) malloc(sizeof(POINT1));

	switch (direction) {
		case 1: p->x = head->next->x;
				p->y = head->next->y - 1;
				break;
		case 2: p->x = head->next->x;
				p->y = head->next->y + 1;
				break;
		case 3: p->x = head->next->x - 1;
				p->y = head->next->y;
				break;
		case 4: p->x = head->next->x + 1;
				p->y = head->next->y;
				break;
	}
	p->next = head->next;
	head->next = p;
	head->x++;
}

void delnode(POINT1 *head)
{
	POINT1 *p = NULL, *q = NULL;
	
	p = head;

	while (p->next != NULL) {
		q = p;
		p = p->next;
	}

	gotoxy(p->x*2, p->y);
	printf("  ");
	q->next = NULL;

	free(p);
	head->x--;
}

void collision(POINT1 *head)
{
	POINT1 *p = NULL;
	p = head->next->next;

	while (p->next != NULL) {
		if (p->x == head->next->x && p->y == head->next->y)
			exit(0);
		p = p->next;
	}

	if (head->next->x == -1 ||
			head->next->y == -1 ||
			head->next->x == 20 ||
			head->next->y == 20)
		exit(0);
}

void pointrands(POINT1 *node, int k)
{
	int i, temp;
	POINT1 *p = head->next;

	while (1) {
		temp = 0;
		node->x = rand()%20;
		node->y = rand()%20;
		for (i = 0; i <= k; i++) {
			if (node->x == rands[i].x &&
					node->y == rands[i].y) {
				temp++;
				if (temp == 2) break;
			}
		}

		if (temp != 2)
			while (p != NULL) {
				if (node->x == p->x && node->y == p->y)
					break;

				if (p->next == NULL) return;

				p = p->next;
			}
	}
}

int eat(POINT1 *head)
{
	int i;
	for (i = 0; i < 5; i++) {
		if (rands[i].x == head->next->x && rands[i].y == head->next->y) {
			pointrands(&rands[i], 4);
			viewrands(&rands[i]);
			viewhead(head);
			return 1;
		}
	}

	return 0;
}

void gotoxy(int x, int y)
{
	COORD pos = {x, y};
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut, pos);
}

void viewhead(POINT1 *head)
{
	POINT1 *p = head->next;

	SetConsoleTextAttribute(hCon, 14);

	while (p != NULL) {
		gotoxy(p->x*2, p->y);
		printf("■");
		
		p = p->next;
	}
}

void viewrands(POINT1 *node)//画随机点?
{  
	SetConsoleTextAttribute(hCon,12);//设置随机点图形的颜色为红色，
	gotoxy(node->x*2,node->y);//定坐标位置，一个图形占两个横坐标和一个纵坐标
	printf("●");//画图形?
}

void viewlevel(void)//画状态图?
{
	SetConsoleTextAttribute(hCon,15);//设置状态栏文字颜色为白色???
	gotoxy(0,21);//将状态栏定位第21行?
	printf("length：%d  nextlength：%d  LEVEL：%d",head->x,level*20,level);
}//项目有蛇的长度、升级的必要长度和等级  

void HideCursor()//隐藏光标函数?
{ 
	CONSOLE_CURSOR_INFO cursor_info = {1, 0};  
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

