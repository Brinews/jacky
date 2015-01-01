#include<stdio.h>//����������� 
#include<malloc.h>//��̬�洢���亯�� 
#include<time.h>//ʱ�亯��
#include<stdlib.h>//����ת�����ı������� #include<time.h>//ʱ�亯��?
#include<conio.h>//���̲��������뺯�� 
#include<windows.h> //windowsͷ�ļ�������ʾ��ɫ�ı��ĺ��� //���嵥������ʾ�߻������

typedef struct point//�����ṹ�岢����ΪPOINT1 
{ 
	int x;//������
	int y;//������
	struct point *next;//���ڣ�ָ���Աָ����struct point������ͬ�Ľṹ?
}POINT1;

//����ȫ�ֱ��� 
POINT1 rands[5],*head=NULL;//����һ���ṹ��������������5������㣬һ���ṹ��ָ��head���������ߵĳ��� 
int key,direction=3,level=1;//���ͱ�����key�����������ķ��򣬳�ʼ����ǰ���ķ�����Ϸ�ȼ�Ϊ1 
clock_t start,end;//����һ����ѭ�����������ѵ�ʱ�䣬����ʱ�䣬��ʧ�ܷ���-1

HANDLE hCon;//������������������ı����ı���ɫ?//��������?

void gotoxy(int x, int y);//���ù��λ��?
void born(POINT1 *head,int direction);//��ǰ����������һ���ڵ�?
void collision(POINT1 *head);//�ж��Ƿ�����ײ�������߽磩
void pointrands(POINT1 *node,int k);//����һ�������?
void delnode(POINT1 *head);//ɾ���ߵ����һ���ڵ�?
void viewhead(POINT1 *head);//����?
void viewrands(POINT1 *node);//�������
void HideCursor();//���ع��
void viewlevel(void); //����״̬��
int  eat(POINT1 *head); //�ж��Ƿ�Ե������

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
	printf("����������");
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
		printf("��");
		
		p = p->next;
	}
}

void viewrands(POINT1 *node)//�������?
{  
	SetConsoleTextAttribute(hCon,12);//���������ͼ�ε���ɫΪ��ɫ��
	gotoxy(node->x*2,node->y);//������λ�ã�һ��ͼ��ռ�����������һ��������
	printf("��");//��ͼ��?
}

void viewlevel(void)//��״̬ͼ?
{
	SetConsoleTextAttribute(hCon,15);//����״̬��������ɫΪ��ɫ???
	gotoxy(0,21);//��״̬����λ��21��?
	printf("length��%d  nextlength��%d  LEVEL��%d",head->x,level*20,level);
}//��Ŀ���ߵĳ��ȡ������ı�Ҫ���Ⱥ͵ȼ�  

void HideCursor()//���ع�꺯��?
{ 
	CONSOLE_CURSOR_INFO cursor_info = {1, 0};  
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

