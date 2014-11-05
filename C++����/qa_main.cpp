#include <fstream>
#include <iostream>
#include <ctime>
#include <string>

using namespace std;

const int OPTN=6;
const int MTK=1024;

//保存题目
int op1[MTK], op2[MTK], opr[MTK];
int result[MTK];		//题目回答正确与否

int maxQ;		 		//多少道目题, 一张试卷最多1024道
int maxN, minN; 		//运算数值范围
int fourOpt[OPTN];		//六种运算符(+-*/%^)
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

	maxQ = 20; //默认20道

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
	cout << "请输入题目数：";
	cin >> maxQ;

	cout << "请输入算数范围min, max(空格隔开)：";
	cin >> minN >> maxN;

	string opt;
	cout << "请选择运算符号+-*/%^(可多选)：";
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
		fourOpt[0] = 1; //默认做加法
	}

	for (i = 0; i < maxQ; i++)
	{
		op1[i] = minN + rand()%(maxN-minN);
		op2[i] = minN + rand()%(maxN-minN);

		//生成运算符
		int opc = rand()%OPTN;
		while (fourOpt[opc] == 0)
			opc = rand()%OPTN;

		opr[i] = opc;

		//为除法运算时
		if (opc == 3)
		{
			while(op2[i] == 0 || op1[i]%op2[i] != 0)
				op2[i] = minN + rand()%(maxN-minN);
		}

		result[i] = tk_answer(op1[i], op2[i], opc);
	}

	cout << "完毕.";
	getchar();
}

void tk_judge()
{
	int i;
	cout << " 开始判卷 " ;

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

	cout << "\n结果：答对" << score << "道，答错" << maxQ-score <<"道.\n";
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

	cout << "重做完成.";
	getchar();
}

void tk_save()
{
	char name[1024] = "data.txt";
	cout << "请输入保存文件名(.txt结尾):";
	cin >> name;

	ofstream out(name);

	for (int i = 0; i < maxQ; i++)
	{
		out << op1[i] << " " << optor[opr[i]] << " " << op2[i] << endl;
	}

	out.close();

	cout <<"试卷保存完毕。\n";
	getchar();
}

void tk_menu()
{
	int opt;

	while (1)
	{
		cout << "***************************************************\n";
		cout << "*               速算系统 V1.0                     *\n";
		cout << "*-------------------------------------------------*\n";
		cout << "*                                                 *\n";
		cout << "* 1. 随机出题                                     *\n";
		cout << "* 2. 试题判卷                                     *\n";
		cout << "* 3. 错题重做                                     *\n";
		cout << "* 4. 试卷保存                                     *\n";
		cout << "* 5. 退出系统                                     *\n";
		cout << "*                                                 *\n";
		cout << "***************************************************\n";

		cout <<"\n 请选择:";

		
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
