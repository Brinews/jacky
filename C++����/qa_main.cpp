#include <fstream>
#include <iostream>
#include <ctime>
#include <string>

using namespace std;

const int OPTN=6;
const int MTK=1024;

//������Ŀ
int op1[MTK], op2[MTK], opr[MTK];
int result[MTK];		//��Ŀ�ش���ȷ���

int maxQ;		 		//���ٵ�Ŀ��, һ���Ծ����1024��
int maxN, minN; 		//������ֵ��Χ
int fourOpt[OPTN];		//���������(+-*/%^)
char *optor = "+-*/%^";

void tk_init();
void tk_rand();
void tk_judge();
void tk_redo();
void tk_save();

void tk_init()
{
	int i;
	srand(time(NULL));

	maxN = INT_MAX;
	minN = INT_MIN;

	maxQ = 20; //Ĭ��20��

	for (i = 0; i < OPTN; i++)
		fourOpt[i] = 0;

	for (i = 0; i < MTK; i++)
	{
		op1[i] = op2[i] = 0;
		opr[i] = 0;
		result[i] = 0;
	}
}

int tk_answer(int op1, int op2, int opr)
{
	int ans = 0, myAns;

	cout << op1 << optor[opr] << op2 << " = " ;
	cin >> myAns;

	if (opr == 0) ans = op1 + op2;
	if (opr == 1) ans = op1 - op2;
	if (opr == 2) ans = op1 * op2;
	if (opr == 3) ans = op1 / op2;
	if (opr == 4) ans = op1 % op2;
	if (opr == 5) ans = op1 ^ op2;

	if (ans == myAns) return 1;

	return 0;
}

void tk_rand()
{
	int i;
	cout << "��������Ŀ����";
	cin >> maxQ;

	cout << "������������Χmin, max(�ո����)��";
	cin >> minN >> maxN;

	string opt;
	cout << "��ѡ���������+-*/%^(�ɶ�ѡ)��";
	cin >> opt;

	int sel = 0;
	for (i = 0; i < opt.length(); i++)
	{
		char c = opt.at(i);
		if (c == '+') { fourOpt[0] = 1; sel = 1; }
		if (c == '-') { fourOpt[1] = 1; sel = 1; }
		if (c == '*') { fourOpt[2] = 1; sel = 1; }
		if (c == '/') { fourOpt[3] = 1; sel = 1; }
		if (c == '%') { fourOpt[4] = 1; sel = 1; }
		if (c == '^') { fourOpt[5] = 1; sel = 1; }
	}

	if (sel == 0)
	{
		fourOpt[0] = 1; //Ĭ�����ӷ�
	}

	for (i = 0; i < maxQ; i++)
	{
		op1[i] = minN + rand()%(maxN-minN);
		op2[i] = minN + rand()%(maxN-minN);

		//���������
		int opc = rand()%OPTN;
		while (fourOpt[opc] == 0)
			opc = rand()%OPTN;

		opr[i] = opc;

		//Ϊ��������ʱ
		if (opc == 3)
		{
			while(op2[i] == 0 || op1[i]%op2[i] != 0)
				op2[i] = minN + rand()%(maxN-minN);
		}

		result[i] = tk_answer(op1[i], op2[i], opc);
	}

	cout << "���.";
	getchar();
}

void tk_judge()
{
	int i;
	cout << " ��ʼ�о� " ;

	for (i = 0; i < 7; i++)
	{
		cout << ".";

		int time = 0;
		while (time < 100000000) time++;
	}

	int score = 0;
	for (i = 0; i < maxQ; i++)
	{
		if (result[i]) score ++;
	}

	cout << "\n��������" << score << "�������" << maxQ-score <<"��.\n";
	getchar();
}

void tk_redo()
{
	int i = 0; 
	for (i = 0; i < maxQ; i++)
	{
		if (result[i] == 0)
		{
			result[i] = tk_answer(op1[i], op2[i], opr[i]);
		}
	}

	cout << "�������.";
	getchar();
}

void tk_save()
{
	char name[1024] = "data.txt";
	cout << "�����뱣���ļ���(.txt��β):";
	cin >> name;

	ofstream out(name);

	for (int i = 0; i < maxQ; i++)
	{
		out << op1[i] << " " << optor[opr[i]] << " " << op2[i] << endl;
	}

	out.close();

	cout <<"�Ծ�����ϡ�\n";
	getchar();
}

void tk_menu()
{
	int opt;

	while (1)
	{
		cout << "***************************************************\n";
		cout << "*               ����ϵͳ V1.0                     *\n";
		cout << "*-------------------------------------------------*\n";
		cout << "*                                                 *\n";
		cout << "* 1. �������                                     *\n";
		cout << "* 2. �����о�                                     *\n";
		cout << "* 3. ��������                                     *\n";
		cout << "* 4. �Ծ���                                     *\n";
		cout << "* 5. �˳�ϵͳ                                     *\n";
		cout << "*                                                 *\n";
		cout << "***************************************************\n";

		cout <<"\n ��ѡ��:";

		
		cin >> opt;

		switch (opt)
		{
		case 1:
			tk_rand();
			break;
		case 2:
			tk_judge();
			break;
		case 3:
			tk_redo();
			break;
		case 4:
			tk_save();
			break;
		case 5:
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
	tk_init();

	tk_menu();

	return 0;
}
