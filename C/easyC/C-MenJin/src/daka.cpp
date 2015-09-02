#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAMLEN	30
#define FILENAME "info_1.dat"
#define FILENAME2 "info_2.dat"

char *ZJLX[] = {"身份证", "工作证", "介绍信"};
char *CRDD[] = {"南门","北门","东门","西门"};
char *CRLX[] = {"出","入"};
char *CRSY[] = {"工作","访问","学习","其他"};
char *JDBM[] = {"办公室","人事部","党委","组织部","宣传部", "教职部", 
	"后勤中心", "保卫科", "学生会", "财务部" };

/* 接待的时间不能超出一天 */
typedef struct daka {
	char name[NAMLEN];
	int zjlx; /* 证件类型: 0:身份证, 1:工作证, 2:介绍信 */
	char zjhm[NAMLEN];
	int crdd; /* 出入地点: 0:南,1:北,2:东,3:西 */
	int crlx; /* 出入类型: 0:出,1:入 */
	int crsy; /* 出入事由: 0:工作，1：访问，2：学习，3：其他 */
	char datetime[16]; /* 201509011011 */
	char jdr[NAMLEN];
	int jdbm; /* 接待部门：0:办公室，1:人事部，2:党委，3.组织部，
	    4.宣传部, 5.教职部, 6.后勤中心, 7.保卫科, 8.学生会, 9.财务部 */

	struct daka *next;

} daka_info;

/* 公用数据 */
daka_info daka_list;
daka_info *tail_node; /* 最后一个节点 */
FILE *fp;

/* 公用函数 */

daka_info *findDakaByZjhm(char *zjhmKey);
daka_info *findDakaByName(char *nameKey);

char *getDesc(char *array[], int index) 
{
	return array[index];
}

/* 打印拼接字符串 */
char *getPrint(char *array[], int size, char *buf) 
{
	int i = 0, len = 0, L = 0;
	for (i = 0; i < size; i++) {
		L = sprintf(buf+len, "%d:%s,", i, array[i]);
		len += L;
	}

	return buf;
}

/* 读取门禁记录 */
daka_info *readDakaFile(FILE *fp)
{
	daka_info node;

	fseek(fp, 0, SEEK_SET);

	while (!feof(fp)) {
		int r = fread(&node, sizeof(daka_info), 1, fp);
		if (r < 1) break;

		daka_info *newNode = (daka_info *) malloc(sizeof(daka_info));
		*newNode = node;
		newNode->next = NULL;

		tail_node->next = newNode;
		tail_node = newNode;
	}

	/* first node is dummy */
	return &daka_list;
}

daka_info readDakaInfo()
{
	daka_info node;
	char buf[1024];

	printf("请输入来访者姓名:");
	scanf("%s", node.name);
	printf("请选择证件类型(%s):", getPrint(ZJLX, 3, buf));
	scanf("%d", &node.zjlx);
	printf("请输入证件号码:");
	scanf("%s", node.zjhm);
	printf("请输入出入地点(%s):", getPrint(CRDD, 4, buf));
	scanf("%d", &node.crdd);
	printf("请输入出入类型(%s):", getPrint(CRLX, 2, buf));
	scanf("%d", &node.crlx);
	printf("请输入事由(%s):", getPrint(CRSY, 4, buf));
	scanf("%d", &node.crsy);

	do {
		printf("请输入访问时间（格式:年月日时分201509016011):");
		scanf("%s", node.datetime);
	} while (strlen(node.datetime) != 12);

	printf("请输入接待人姓名:");
	scanf("%s", node.jdr);
	printf("请输入接待部门(%s):", getPrint(JDBM, 10, buf));
	scanf("%d", &node.jdbm);

	return node;
}

/* 增加门禁记录 */
daka_info *addDakaInfo(FILE *fp)
{
	daka_info node = readDakaInfo();

	daka_info *newNode = (daka_info*) malloc(sizeof(daka_info));
	*newNode = node;
	newNode->next = NULL;

	tail_node->next = newNode;
	tail_node = newNode;

	/* 写文件 */
	fseek(fp, 0, SEEK_END);
	fwrite(newNode, sizeof(daka_info), 1, fp);
	fflush(fp);

	return newNode;
}

void queryDakaInfo()
{
	int choice;
	char buf[1024];

	printf("查询功能选择：\n1.按照姓名检索；\n2.按照证件号码检索\n");
	scanf("%d", &choice);

	if (choice == 1) {
		printf("请输入来访者姓名:");
		scanf("%s", buf);

		findDakaByName(buf);
	} else if (choice == 2) {
		printf("请输入来访者证件号码:");
		scanf("%s", buf);

		findDakaByZjhm(buf);
	}
}

void printDakaInfo(daka_info *di)
{
	printf("来访姓名:%32s\t", di->name);
	printf("访问日期:%32s\n", di->datetime);
	printf("证件类型:%32s\t", getDesc(ZJLX, di->zjlx));
	printf("证件号码:%32s\n", di->zjhm);
	printf("出入地点:%32s\t", getDesc(CRDD, di->crdd));
	printf("出入类型:%32s\n", getDesc(CRLX, di->crlx));
	printf("出入事由:%32s\n", getDesc(CRSY, di->crsy));
	printf("接待姓名:%32s\t", di->jdr);
	printf("接待部门:%32s\n", getDesc(JDBM, di->jdbm));
	printf("\n");
}

daka_info *findDakaByName(char *nameKey)
{
	daka_info *last;

	if (nameKey == NULL) return NULL;

	daka_info *ptr = daka_list.next;
	while (ptr != NULL) {
		if (strcmp(ptr->name, nameKey) == 0) {
			last = ptr;
			printDakaInfo(ptr);
		}

		ptr = ptr->next;
	}

	return last;
}

daka_info *findDakaByZjhm(char *zjhmKey)
{
	daka_info *last;

	if (zjhmKey == NULL) return NULL;

	daka_info *ptr = daka_list.next;
	while (ptr != NULL) {
		if (strcmp(ptr->zjhm, zjhmKey) == 0) {
			last = ptr;
			printDakaInfo(ptr);
		}

		ptr = ptr->next;
	}

	return last;
}

void updateDakaInfo(FILE *fp)
{
	char name[NAMLEN];
	printf("请输入需要更新的来访者姓名:");
	scanf("%s", name);

	int i = 0;
	daka_info *ptr = daka_list.next;
	while (ptr != NULL) {
		if (strcmp(ptr->name, name) == 0) {
			/* found */
			break;
		}

		i++;
		ptr = ptr->next;
	}

	if (ptr != NULL) {

		daka_info *next = ptr->next;

		printf("原来访信息:\n");
		printDakaInfo(ptr);

		printf("更新来访信息:\n");
		daka_info node = readDakaInfo();
		*ptr = node;

		/* 恢复原来的next */
		ptr->next = next;

		rewind(fp);
		fseek(fp, i*sizeof(daka_info), SEEK_SET);
		fwrite(ptr, sizeof(daka_info), 1, fp);
		fflush(fp);

	} else {
		printf("未找到待修改信息.\n");
	}
}

FILE* deleteDakaInfo(FILE *fp)
{
	char name[NAMLEN];
	printf("请输入需要更新的来访者姓名:");
	scanf("%s", name);

	int i = 0;
	daka_info *ptr = daka_list.next;
	daka_info *lptr = &daka_list;

	while (ptr != NULL) {
		if (strcmp(ptr->name, name) == 0) {
			/* found */
			break;
		}

		i++;
		ptr = ptr->next;
		lptr = ptr;
	}

	if (ptr != NULL) {
		lptr->next = ptr->next;
		if (ptr->next == NULL)
			tail_node = lptr;
		free(ptr);

		/* 重新写文件 */
		fclose(fp);
		fp = fopen(FILENAME, "w");

		ptr = daka_list.next;
		while (ptr != NULL) {
			fwrite(ptr, sizeof(daka_info), 1, fp);
			ptr = ptr->next;
		}
	}
	else {
		printf("未找到待删除信息.\n");
	}

	return fp;
}

void freeDakaInfo()
{
	daka_info *ptr = daka_list.next;

	while (ptr != NULL) {
		daka_info *p = ptr->next;
		free(ptr);
		ptr = p;
	}
}

/*
 * 统计各出入点次数
 */
void statCRDDInfo()
{
	int i;
	int Crdd[4] = {0};

	daka_info *ptr = daka_list.next;
	while (ptr != NULL) {
		Crdd[ptr->crdd]++;
		ptr = ptr->next;
	}

	for (i = 0; i < 4; i++) {
		printf("出入点:%s, 出入次数:%d\n", getDesc(CRDD, i), Crdd[i]);
	}
}

/*
 * 计算时间差, 返回分钟数
 * 接待的时间不能超出一天
 */
int calculateTimeInterval(char *start, char *end)
{
	int dist = 0;

	int shour = start[8] - '0';
	shour = shour * 10 + start[9] - '0';

	int ehour = end[8] - '0';
	ehour = ehour * 10 + end[9] - '0';

	dist = (ehour - shour) * 60;

	shour = start[10] - '0';
	shour = shour * 10 + start[11] - '0';

	ehour = end[10] - '0';
	ehour = ehour * 10 + end[10] - '0';

	dist += (ehour - shour);

	return dist;
}

/*
 * 同一天判断
 */
int sameDay(char *start, char *end)
{
	int ret = 1;
	int i;

	for (i = 0; i < 8; i++) {
		if (start[i] != end[i]) return 0;
	}

	return ret;
}

void statJDBMInfo()
{
	int i;
	int Jdbm[10] = {0};

	daka_info *ptr = daka_list.next;
	while (ptr != NULL) {
		if (ptr->crlx == 1) {
			daka_info *p = daka_list.next;
			while (p != NULL) {
				if (strcmp(p->name, ptr->name) == 0 &&
						strcmp(p->zjhm, ptr->zjhm) == 0 &&
						p->jdbm == ptr->jdbm &&
						p->crlx == 0 &&
						sameDay(p->datetime, ptr->datetime)) {
					Jdbm[p->jdbm] += calculateTimeInterval(ptr->datetime,
							p->datetime);
					break;
				}

				p = p->next;
			}
		}
		ptr = ptr->next;
	}

	for (i = 0; i < 10; i++) {
		printf("接待部门:%s, 接待时间:%d\n", getDesc(JDBM, i), Jdbm[i]);
	}

}

void statCRSYInfo()
{
	int i;
	int Crsy[4] = {0};

	daka_info *ptr = daka_list.next;
	while (ptr != NULL) {
		Crsy[ptr->crsy]++;
		ptr = ptr->next;
	}

	for (i = 0; i < 4; i++) {
		printf("出入事由:%s, 出入次数:%d\n", getDesc(CRSY, i), Crsy[i]);
	}
}

/*
 * 统计功能
 */
void statDakaInfo()
{
	int choice;
	printf("统计功能\n1.统计各出入点的出入次数;\n2.统计各部门的接待时间;\n3.统计各事由对应出入次数\n");

	scanf("%d", &choice);
	if (choice == 1) {
		statCRDDInfo();
	} else if (choice == 2) {
		statJDBMInfo();
	} else if (choice == 3) {
		statCRSYInfo();
	}
}

/* 
 * 统计不同访问者的滞留时间
 * 并写文件保存
 * 及排序显示
 */
void sortDakaInfo()
{
	struct stayTime {
		char name[NAMLEN];
		char zjhm[NAMLEN];
		int time;
		struct stayTime *next;
	};

	struct stayTime st;
	st.next = NULL;

	daka_info *ptr = daka_list.next;
	while (ptr != NULL) {
		if (ptr->crlx == 1) {
			daka_info *p = daka_list.next;

			while (p != NULL) {
				if (strcmp(p->name, ptr->name) == 0 &&
						strcmp(p->zjhm, ptr->zjhm) == 0 &&
						p->jdbm == ptr->jdbm &&
						p->crlx == 0 &&
						sameDay(p->datetime, ptr->datetime)) {

					int flag = 0;
					struct stayTime *sptr = st.next;
					while (sptr != NULL) {
						if (strcmp(sptr->name, p->name) == 0 &&
								strcmp(sptr->zjhm, p->zjhm) == 0) {
							flag = 1;
							break;
						}

						sptr = sptr->next;
					}

					if (flag == 1) {
						/* update */
						sptr->time += calculateTimeInterval(ptr->datetime,
								p->datetime);
					}
					else {
						/* add */
						struct stayTime *nptr = (struct stayTime *)
							malloc(sizeof(struct stayTime));
						strcpy(nptr->name, p->name);
						strcpy(nptr->zjhm, p->zjhm);
						nptr->time = calculateTimeInterval(ptr->datetime,
								p->datetime);

						nptr->next = st.next;
						st.next = nptr;
					}

					break;
				}

				p = p->next;
			}
		}

		ptr = ptr->next;
	}

	/* 按时间长短排序 */
	struct stayTime *iptr, *jptr;
	iptr = st.next;
	while (iptr != NULL) {
		jptr = st.next;
		while (jptr != iptr) {
			if (jptr->time < iptr->time) {
				/* swap */
				struct stayTime temp;
				temp = *jptr;
				*jptr = *iptr;
				*iptr = temp;

				/* next not swap */
				temp.next = iptr->next;
				iptr->next = jptr->next;
				jptr->next = temp.next;
			}

			jptr = jptr->next;
		}
		iptr = iptr->next;
	}

	/* 屏幕显示 */
	iptr = st.next;
	while (iptr != NULL) {
		printf("来访者姓名:%32s,证件号码:%32s,滞留总时间:%d\n",
				iptr->name, iptr->zjhm, iptr->time);
		iptr = iptr->next;
	}

	/* 写文件info_2 */
	FILE *f = fopen(FILENAME2, "w+");
	iptr = st.next;
	while (iptr != NULL) {
		fwrite(iptr, sizeof(struct stayTime), 1, f);
		iptr = iptr->next;
	}

	fclose(f);

	/* 释放内存 */
	iptr = st.next;
	while (iptr != NULL) {
		struct stayTime *s = iptr->next;
		free(iptr);
		iptr = s;
	}
}

void printAllDaka()
{
	daka_info *ptr = daka_list.next;
	while (ptr != NULL) {
		printDakaInfo(ptr);
		ptr = ptr->next;
	}
}

/* 主程序 */
int main()
{
	int choice;

	daka_list.next = NULL;
	tail_node = &daka_list; /* 最后一个节点 */

	fp = fopen(FILENAME, "r+");
	if (fp == NULL) fp = fopen(FILENAME, "w+");
	readDakaFile(fp);

	while(1)
	{
		system("cls");
		printf("*----------------------------门禁信息管理系统------------------------*\n");
		printf("*请选择：                                                            *\n");
		printf("*             1. 录入门禁信息                                        *\n");
		printf("*             2. 查询门禁信息                                        *\n");
		printf("*             3. 修改门禁信息                                        *\n");
		printf("*             4. 删除门禁信息                                        *\n");
		printf("*             5. 统计门禁信息                                        *\n");
		printf("*             6. 统计滞留时间                                        *\n");
		printf("*             7. 打印所有来访                                        *\n");
		printf("*             8. 退出系统                                            *\n");
		printf("**********************************************************************\n");

		scanf("%d", &choice);
		switch(choice)
		{
			case 1:
				addDakaInfo(fp);
				break;

			case 2:
				queryDakaInfo();
				break;

			case 3:
				updateDakaInfo(fp);
				break;

			case 4:
				fp = deleteDakaInfo(fp);
				break;

			case 5:
				statDakaInfo();
				break;

			case 6:
				sortDakaInfo();
				break;

			case 7:
				printAllDaka();
				break;

			default:
				break;
		}

		if (choice == 8) break;
		system("pause");
	}

	freeDakaInfo(); //空间释放

	if (fp != NULL)
		fclose(fp);

	return 0;
}
