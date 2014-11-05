#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXNUM	100 //最多100个联系人

typedef struct txl_item
{
	char name[20];	//姓名
	char tel[15];	//电话
	char mail[50];	//邮件
	char code[10];	//邮编
	char addr[50];	//地址
} txl_item;

txl_item txl_book[MAXNUM];
int txl_idx = 0;

char t_name[20], t_tel[15], t_addr[50], t_mail[50], t_code[10];

/* 函数声明 */
void txl_add();
void txl_del();
void txl_search();
void txl_update();
void txl_sort();
void txl_print_item(txl_item ti);
void txl_print();


/* 函数实现 */

/* 添加联系人 */
void txl_add()
{
	if (txl_idx > MAXNUM) 
	{
		printf("已经达到最大数量100的限制.");
		return;
	}

	printf("请输入姓名:");
	scanf("%s", &t_name);

	printf("请输入电话:");
	scanf("%s", &t_tel);

	printf("请输入邮件地址:");
	scanf("%s", &t_mail);

	printf("请输入邮编:");
	scanf("%s", &t_code);

	printf("请输入地址:");
	scanf("%s", &t_addr);

	strcpy(txl_book[txl_idx].name, t_name);
	strcpy(txl_book[txl_idx].tel, t_tel);
	strcpy(txl_book[txl_idx].addr, t_addr);
	strcpy(txl_book[txl_idx].code, t_code);
	strcpy(txl_book[txl_idx].mail, t_mail);

	txl_idx++;

	printf("添加成功。\n");
	getch();
}

void txl_del()
{
	int i = 0, j = -1;

	printf("请输入要删除联系人姓名:");
	scanf("%s", &t_name);

	for (i = 0; i < txl_idx; i++)
	{
		if (strcmp(txl_book[i].name, t_name) == 0)
		{
			//进行删除
			j = i;
			while (j+1 < txl_idx)
			{
				memcpy(&txl_book[j], &txl_book[j+1], sizeof(txl_item));
				j++;
			}

			txl_idx--;
			printf("删除%s(%s)成功.\n", t_name, txl_book[i].tel);
		}
	}

	if (j == -1)
	{
		printf("需要删除的联系人不存在.\n");
	}
	else
	{
		printf("删除完成。\n");
	}
	getch();
}

void txl_search()
{
	int i = 0, j = -1;

	printf("请输入要查找的联系人姓名:");
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
		printf("未找到该联系人信息.\n");
	}
	else
	{
		printf("查询完成。\n");
	}
	getch();

}

void txl_update()
{
	int i = 0, j = 0;

	printf("请输入要更新的联系人姓名:");
	scanf("%s", &t_name);

	for (i = 0; i < txl_idx; i++)
	{
		if (strcmp(txl_book[i].name, t_name) == 0)
		{
			printf("输入新的号码：");
			scanf("%s", &t_tel);

			strcpy(txl_book[i].tel, t_tel);
			j = 1;
		}
	}

	if (j == 0)
	{
		printf("未找到需要更新的联系人信息.\n");
	}
	else
	{
		printf("更新完成。\n");
	}
	getch();
}

void txl_sort()
{
	int i , j, k;
	int opt;

	printf("排序方式选择（1.姓名，2.电话，3.邮件）");
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

		if (k != i) //进行交换
		{
			txl_item temp;

			memcpy(&temp, &txl_book[i], sizeof(txl_item));
			memcpy(&txl_book[i], &txl_book[k], sizeof(txl_item));
			memcpy(&txl_book[k], &temp, sizeof(txl_item));
		}
	}

	printf("排序完成。\n");
	getch();
}

void txl_print_item(txl_item ti)
{
	printf("姓名：%s，电话：%s, 联系地址:%s,邮件：%s, 邮编:%s\n",
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

	printf("输出完成。\n");
	getch();
}

void txl_menu()
{
	int opt;

	while (1)
	{
		printf("***************************************************\n");
		printf("*               个人通信录 V1.0                   *\n");
		printf("*-------------------------------------------------*\n");
		printf("*                                                 *\n");
		printf("* 1. 添加信息                                     *\n");
		printf("* 2. 信息查询                                     *\n");
		printf("* 3. 信息删除                                     *\n");
		printf("* 4. 进行排序                                     *\n");
		printf("* 5. 全部输出                                     *\n");
		printf("* 0. 退出                                         *\n");
		printf("*                                                 *\n");
		printf("***************************************************\n");

		printf("\n 请选择:");

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
