#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXNUM	100 //���100����ϵ��

typedef struct txl_item
{
	char name[20];	//����
	char tel[15];	//�绰
	char mail[50];	//�ʼ�
	char code[10];	//�ʱ�
	char addr[50];	//��ַ
} txl_item;

txl_item txl_book[MAXNUM];
int txl_idx = 0;

char t_name[20], t_tel[15], t_addr[50], t_mail[50], t_code[10];

/* �������� */
void txl_add();
void txl_del();
void txl_search();
void txl_update();
void txl_sort();
void txl_print_item(txl_item ti);
void txl_print();


/* ����ʵ�� */

/* �����ϵ�� */
void txl_add()
{
	if (txl_idx > MAXNUM) 
	{
		printf("�Ѿ��ﵽ�������100������.");
		return;
	}

	printf("����������:");
	scanf("%s", &t_name);

	printf("������绰:");
	scanf("%s", &t_tel);

	printf("�������ʼ���ַ:");
	scanf("%s", &t_mail);

	printf("�������ʱ�:");
	scanf("%s", &t_code);

	printf("�������ַ:");
	scanf("%s", &t_addr);

	strcpy(txl_book[txl_idx].name, t_name);
	strcpy(txl_book[txl_idx].tel, t_tel);
	strcpy(txl_book[txl_idx].addr, t_addr);
	strcpy(txl_book[txl_idx].code, t_code);
	strcpy(txl_book[txl_idx].mail, t_mail);

	txl_idx++;

	printf("��ӳɹ���\n");
	getch();
}

void txl_del()
{
	int i = 0, j = -1;

	printf("������Ҫɾ����ϵ������:");
	scanf("%s", &t_name);

	for (i = 0; i < txl_idx; i++)
	{
		if (strcmp(txl_book[i].name, t_name) == 0)
		{
			//����ɾ��
			j = i;
			while (j+1 < txl_idx)
			{
				memcpy(&txl_book[j], &txl_book[j+1], sizeof(txl_item));
				j++;
			}

			txl_idx--;
			printf("ɾ��%s(%s)�ɹ�.\n", t_name, txl_book[i].tel);
		}
	}

	if (j == -1)
	{
		printf("��Ҫɾ������ϵ�˲�����.\n");
	}
	else
	{
		printf("ɾ����ɡ�\n");
	}
	getch();
}

void txl_search()
{
	int i = 0, j = -1;

	printf("������Ҫ���ҵ���ϵ������:");
	scanf("%s", &t_name);

	for (i = 0; i < txl_idx; i++)
	{
		if (strcmp(txl_book[i].name, t_name) == 0)
		{
			txl_print_item(txl_book[i]);
			j = 1;
		}
	}

	if (j == -1)
	{
		printf("δ�ҵ�����ϵ����Ϣ.\n");
	}
	else
	{
		printf("��ѯ��ɡ�\n");
	}
	getch();

}

void txl_update()
{
	int i = 0, j = 0;

	printf("������Ҫ���µ���ϵ������:");
	scanf("%s", &t_name);

	for (i = 0; i < txl_idx; i++)
	{
		if (strcmp(txl_book[i].name, t_name) == 0)
		{
			printf("�����µĺ��룺");
			scanf("%s", &t_tel);

			strcpy(txl_book[i].tel, t_tel);
			j = 1;
		}
	}

	if (j == 0)
	{
		printf("δ�ҵ���Ҫ���µ���ϵ����Ϣ.\n");
	}
	else
	{
		printf("������ɡ�\n");
	}
	getch();
}

void txl_sort()
{
	int i , j, k;
	int opt;

	printf("����ʽѡ��1.������2.�绰��3.�ʼ���");
	scanf("%d", &opt);

	for (i = 0; i < txl_idx - 1; i++)
	{
		k = i;
		for (j = i+1; j < txl_idx; j++)
		{
			if (opt == 1)
			{
				if (strcmp(txl_book[j].name, txl_book[i].name) < 0)
				{
					k = j;
				}
			}
			else if (opt == 2)
			{
				if (strcmp(txl_book[j].tel, txl_book[i].tel) < 0)
				{
					k = j;
				}
			}
			else if (opt == 3)
			{
				if (strcmp(txl_book[j].mail, txl_book[i].mail) < 0)
				{
					k = j;
				}
			}
		}

		if (k != i) //���н���
		{
			txl_item temp;

			memcpy(&temp, &txl_book[i], sizeof(txl_item));
			memcpy(&txl_book[i], &txl_book[k], sizeof(txl_item));
			memcpy(&txl_book[k], &temp, sizeof(txl_item));
		}
	}

	printf("������ɡ�\n");
	getch();
}

void txl_print_item(txl_item ti)
{
	printf("������%s���绰��%s, ��ϵ��ַ:%s,�ʼ���%s, �ʱ�:%s\n",
			ti.name, ti.tel,
			ti.addr, ti.mail, ti.code);
}

void txl_print()
{
	int i = 0;

	for (i = 0; i < txl_idx; i++)
	{
		txl_print_item(txl_book[i]);
	}

	printf("�����ɡ�\n");
	getch();
}

void txl_menu()
{
	int opt;

	while (1)
	{
		printf("***************************************************\n");
		printf("*               ����ͨ��¼ V1.0                   *\n");
		printf("*-------------------------------------------------*\n");
		printf("*                                                 *\n");
		printf("* 1. �����Ϣ                                     *\n");
		printf("* 2. ��Ϣ��ѯ                                     *\n");
		printf("* 3. ��Ϣɾ��                                     *\n");
		printf("* 4. ��������                                     *\n");
		printf("* 5. ȫ�����                                     *\n");
		printf("* 0. �˳�                                         *\n");
		printf("*                                                 *\n");
		printf("***************************************************\n");

		printf("\n ��ѡ��:");

		scanf("%d", &opt);

		switch (opt)
		{
		case 1:
			txl_add();
			break;
		case 2:
			txl_search();
			break;
		case 3:
			txl_del();
			break;
		case 4:
			txl_sort();
			break;
		case 5:
			txl_print();
			break;
		case 0:
			return;
		default:
			break;
		}

		getchar();
		system("cls");
	}
}

int main()
{
	txl_menu();

	return 0;
}
