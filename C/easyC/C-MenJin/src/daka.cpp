#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAMLEN	30
#define FILENAME "info_1.dat"
#define FILENAME2 "info_2.dat"

char *ZJLX[] = {"���֤", "����֤", "������"};
char *CRDD[] = {"����","����","����","����"};
char *CRLX[] = {"��","��"};
char *CRSY[] = {"����","����","ѧϰ","����"};
char *JDBM[] = {"�칫��","���²�","��ί","��֯��","������", "��ְ��", 
	"��������", "������", "ѧ����", "����" };

/* �Ӵ���ʱ�䲻�ܳ���һ�� */
typedef struct daka {
	char name[NAMLEN];
	int zjlx; /* ֤������: 0:���֤, 1:����֤, 2:������ */
	char zjhm[NAMLEN];
	int crdd; /* ����ص�: 0:��,1:��,2:��,3:�� */
	int crlx; /* ��������: 0:��,1:�� */
	int crsy; /* ��������: 0:������1�����ʣ�2��ѧϰ��3������ */
	char datetime[16]; /* 201509011011 */
	char jdr[NAMLEN];
	int jdbm; /* �Ӵ����ţ�0:�칫�ң�1:���²���2:��ί��3.��֯����
	    4.������, 5.��ְ��, 6.��������, 7.������, 8.ѧ����, 9.���� */

	struct daka *next;

} daka_info;

/* �������� */
daka_info daka_list;
daka_info *tail_node; /* ���һ���ڵ� */
FILE *fp;

/* ���ú��� */

daka_info *findDakaByZjhm(char *zjhmKey);
daka_info *findDakaByName(char *nameKey);

char *getDesc(char *array[], int index) 
{
	return array[index];
}

/* ��ӡƴ���ַ��� */
char *getPrint(char *array[], int size, char *buf) 
{
	int i = 0, len = 0, L = 0;
	for (i = 0; i < size; i++) {
		L = sprintf(buf+len, "%d:%s,", i, array[i]);
		len += L;
	}

	return buf;
}

/* ��ȡ�Ž���¼ */
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

	printf("����������������:");
	scanf("%s", node.name);
	printf("��ѡ��֤������(%s):", getPrint(ZJLX, 3, buf));
	scanf("%d", &node.zjlx);
	printf("������֤������:");
	scanf("%s", node.zjhm);
	printf("���������ص�(%s):", getPrint(CRDD, 4, buf));
	scanf("%d", &node.crdd);
	printf("�������������(%s):", getPrint(CRLX, 2, buf));
	scanf("%d", &node.crlx);
	printf("����������(%s):", getPrint(CRSY, 4, buf));
	scanf("%d", &node.crsy);

	do {
		printf("���������ʱ�䣨��ʽ:������ʱ��201509016011):");
		scanf("%s", node.datetime);
	} while (strlen(node.datetime) != 12);

	printf("������Ӵ�������:");
	scanf("%s", node.jdr);
	printf("������Ӵ�����(%s):", getPrint(JDBM, 10, buf));
	scanf("%d", &node.jdbm);

	return node;
}

/* �����Ž���¼ */
daka_info *addDakaInfo(FILE *fp)
{
	daka_info node = readDakaInfo();

	daka_info *newNode = (daka_info*) malloc(sizeof(daka_info));
	*newNode = node;
	newNode->next = NULL;

	tail_node->next = newNode;
	tail_node = newNode;

	/* д�ļ� */
	fseek(fp, 0, SEEK_END);
	fwrite(newNode, sizeof(daka_info), 1, fp);
	fflush(fp);

	return newNode;
}

void queryDakaInfo()
{
	int choice;
	char buf[1024];

	printf("��ѯ����ѡ��\n1.��������������\n2.����֤���������\n");
	scanf("%d", &choice);

	if (choice == 1) {
		printf("����������������:");
		scanf("%s", buf);

		findDakaByName(buf);
	} else if (choice == 2) {
		printf("������������֤������:");
		scanf("%s", buf);

		findDakaByZjhm(buf);
	}
}

void printDakaInfo(daka_info *di)
{
	printf("��������:%32s\t", di->name);
	printf("��������:%32s\n", di->datetime);
	printf("֤������:%32s\t", getDesc(ZJLX, di->zjlx));
	printf("֤������:%32s\n", di->zjhm);
	printf("����ص�:%32s\t", getDesc(CRDD, di->crdd));
	printf("��������:%32s\n", getDesc(CRLX, di->crlx));
	printf("��������:%32s\n", getDesc(CRSY, di->crsy));
	printf("�Ӵ�����:%32s\t", di->jdr);
	printf("�Ӵ�����:%32s\n", getDesc(JDBM, di->jdbm));
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
	printf("��������Ҫ���µ�����������:");
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

		printf("ԭ������Ϣ:\n");
		printDakaInfo(ptr);

		printf("����������Ϣ:\n");
		daka_info node = readDakaInfo();
		*ptr = node;

		/* �ָ�ԭ����next */
		ptr->next = next;

		rewind(fp);
		fseek(fp, i*sizeof(daka_info), SEEK_SET);
		fwrite(ptr, sizeof(daka_info), 1, fp);
		fflush(fp);

	} else {
		printf("δ�ҵ����޸���Ϣ.\n");
	}
}

FILE* deleteDakaInfo(FILE *fp)
{
	char name[NAMLEN];
	printf("��������Ҫ���µ�����������:");
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

		/* ����д�ļ� */
		fclose(fp);
		fp = fopen(FILENAME, "w");

		ptr = daka_list.next;
		while (ptr != NULL) {
			fwrite(ptr, sizeof(daka_info), 1, fp);
			ptr = ptr->next;
		}
	}
	else {
		printf("δ�ҵ���ɾ����Ϣ.\n");
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
 * ͳ�Ƹ���������
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
		printf("�����:%s, �������:%d\n", getDesc(CRDD, i), Crdd[i]);
	}
}

/*
 * ����ʱ���, ���ط�����
 * �Ӵ���ʱ�䲻�ܳ���һ��
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
 * ͬһ���ж�
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
		printf("�Ӵ�����:%s, �Ӵ�ʱ��:%d\n", getDesc(JDBM, i), Jdbm[i]);
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
		printf("��������:%s, �������:%d\n", getDesc(CRSY, i), Crsy[i]);
	}
}

/*
 * ͳ�ƹ���
 */
void statDakaInfo()
{
	int choice;
	printf("ͳ�ƹ���\n1.ͳ�Ƹ������ĳ������;\n2.ͳ�Ƹ����ŵĽӴ�ʱ��;\n3.ͳ�Ƹ����ɶ�Ӧ�������\n");

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
 * ͳ�Ʋ�ͬ�����ߵ�����ʱ��
 * ��д�ļ�����
 * ��������ʾ
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

	/* ��ʱ�䳤������ */
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

	/* ��Ļ��ʾ */
	iptr = st.next;
	while (iptr != NULL) {
		printf("����������:%32s,֤������:%32s,������ʱ��:%d\n",
				iptr->name, iptr->zjhm, iptr->time);
		iptr = iptr->next;
	}

	/* д�ļ�info_2 */
	FILE *f = fopen(FILENAME2, "w+");
	iptr = st.next;
	while (iptr != NULL) {
		fwrite(iptr, sizeof(struct stayTime), 1, f);
		iptr = iptr->next;
	}

	fclose(f);

	/* �ͷ��ڴ� */
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

/* ������ */
int main()
{
	int choice;

	daka_list.next = NULL;
	tail_node = &daka_list; /* ���һ���ڵ� */

	fp = fopen(FILENAME, "r+");
	if (fp == NULL) fp = fopen(FILENAME, "w+");
	readDakaFile(fp);

	while(1)
	{
		system("cls");
		printf("*----------------------------�Ž���Ϣ����ϵͳ------------------------*\n");
		printf("*��ѡ��                                                            *\n");
		printf("*             1. ¼���Ž���Ϣ                                        *\n");
		printf("*             2. ��ѯ�Ž���Ϣ                                        *\n");
		printf("*             3. �޸��Ž���Ϣ                                        *\n");
		printf("*             4. ɾ���Ž���Ϣ                                        *\n");
		printf("*             5. ͳ���Ž���Ϣ                                        *\n");
		printf("*             6. ͳ������ʱ��                                        *\n");
		printf("*             7. ��ӡ��������                                        *\n");
		printf("*             8. �˳�ϵͳ                                            *\n");
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

	freeDakaInfo(); //�ռ��ͷ�

	if (fp != NULL)
		fclose(fp);

	return 0;
}
