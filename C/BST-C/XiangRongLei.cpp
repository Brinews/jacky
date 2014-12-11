#include<iostream>
#include<iomanip>
#include <queue>

using namespace std;

typedef struct Node {
	int nIndex;
	int childNum;
	struct Node** childList;
	struct Node *parent;
}Node;

bool subClass(int *p, int *q, int count);
int* getCptbClass(struct Node *last, int count);
void insertCptbClass(int *one, int **all, int count, int* total);

char *input_mat(int *n)//�����ڽӾ���
{
	char *p;
	cout<<"����ͼ�Ķ�����num=";
	cin>>*n;
	p=new char[*n**n];
	cout<<"����ͼ�ڽӾ���"<<endl;
	for(int i=0;i<*n;i++){
		for(int j=0;j<*n;j++)
			cin>>*(p+i**n+j);
	}
	return p;
}

void output_mat(char *p, int n)//����ڽӾ���
{
	cout<<"ͼ�ڽӾ���";
	for(int i=0;i<n;i++)
	{	
		for(int j=0;j<n;j++){
			if(j==0)cout<<'\n';
			cout<<*(p+i*n+j)<<setw(2);}
	}
	cout<<'\n'<<endl;
}

int find_pp(char *p,char *p2,int pp[][2],int num1,int num2)//Ѱ�ҵ��
{
	int count=0;
	for(int i=0;i<num1;i++)
		for(int j=0;j<num2;j++)
		{
			if(*(p+i*num1+i)==*(p2+j*num2+j))
			{
				pp[count][0]=i+1;
				pp[count][1]=j+1;
				count++;
			}
		}
	return count;
}

void putout_pp(int pp[][2],int count)//������
{
	cout<<"�����ԣ�";
	for(int i=0;i<count;i++){
		if(i%5==0)cout<<endl;
		cout<<"("<<pp[i][0]<<","<<pp[i][1]<<")"<<setw(4);
	}
	cout<<'\n'<<endl;
}

/* ��ʱ��ʹ�� */
int *output_table(int pp[][2],int count,char *p,
		int num1,int num2,char *p2)//�������ݱ�
{
	cout<<"���ݱ�"<<endl;
	int m,n;
	int *pp_2;
	pp_2=new int[count*count];

	for(m=0;m<count;m++)
		for(n=0;n<count;n++)
			pp_2[m*count+n]=0;

	for(m=1;m<count;m++)
	{
		cout<<"("<<pp[m][0]<<","<<pp[m][1]<<")\t";
		for(n=0;n<m;n++)
		{
			if(pp[m][1]!=pp[n][1]||pp[m][0]!=pp[n][0])
			{
				if(*(p+num1*pp[n][0]+pp[m][0]-num1-1)
						==*(p2+num2*pp[n][1]+pp[m][1]-num2-1))
				{
					if(*(p+num1*pp[m][0]+pp[n][0]-num1-1)
							==*(p2+num2*pp[m][1]+pp[n][1]-num2-1))
					{
						cout<<"1    ";
						pp_2[m*count+n]=1;
						continue;
					}
					else{
						cout<<"0    ";
						continue;
					}
				}
				else
				{
					cout<<"0    ";
					continue;
				}

			}
			else
			{
				cout<<"0    ";
				continue;
			}			
		}
		cout<<endl;
	}
	cout<<"      ";
	for(int i=0;i<count-1;i++)
		cout<<"("<<pp[i][0]<<","<<pp[i][1]<<")";
	cout<<'\n'<<endl;
	return pp_2;
}

/* �ж��Ƿ�Ϊ�ֵܽڵ� */
bool isParentChild(int m, struct Node* p)
{
	if (p->parent == NULL) return true;

	struct Node* para = p->parent;
	for (int i = 0; i < para->childNum; i++)
	{
		if (m == para->childList[i]->nIndex) return true;
	}

	return false;
}

/* �������ݱ����������࣬ ����һ������һ�� */
struct Node* buildCompatibleClassTree(int *p3, int count, 
		int start, int **all, int *total)
{
	struct Node *root = new Node;
	root->nIndex = start;
	root->parent = NULL;
	root->childList = NULL;
	root->childNum = 0;

	queue<Node*> q;
	q.push(root);

	Node *cur;
	while (q.size() != 0 && (cur = q.front()))
	{
		int childNum = 0;
		for (int m = cur->nIndex+1; m < count; m++)
		{
			if (p3[cur->nIndex + m*count] == 1 
				&& isParentChild(m, cur))
				childNum++;
		}

		if (childNum != 0)
		{
			cur->childList = new Node*[childNum];
			cur->childNum = childNum;

			int ci = 0;
			for (int m = cur->nIndex+1; m < count; m++)
			{
				if (p3[cur->nIndex + m*count] == 1 
						&& isParentChild(m, cur))
				{
					struct Node* child = new Node;
					child->nIndex = m;
					child->parent = cur;
					child->childList = NULL;
					child->childNum = 0;
					cur->childList[ci++] = child;

					q.push(child);
				}
			}
		}
		else
		{
			if (*total == 11)
			{
				cout << cur->nIndex << endl;
			}

			int *a = getCptbClass(cur, count);
			insertCptbClass(a, all, count, total);
		}

		q.pop();
	}

	return root;
}

/* �������õ������� */
int* getCptbClass(struct Node *last, int count)
{
	int *ret = new int[count];
	memset(ret, 0, sizeof(int)*count);

	while (last->parent != NULL)
	{
		ret[last->nIndex] = 1;
		last = last->parent;
	}

	ret[last->nIndex] = 1;

	return ret;
}

/* �ҵ�һ�������࣬���в��� */
void insertCptbClass(int *one, int **all, int count, int* total)
{
	for (int i = 0; i < *total; i++)
	{
		if (subClass(one, all[i], count))
			return;

		if (all[i][0] != 2 && subClass(all[i], one, count))
		{
			all[i][0] = 2;
		}
	}

	for (int i = 0; i < count; i++)
	{
		all[*total][i] = one[i];
	}

	(*total)++;
}

/* ��ӡ������ */
void printCptbClass(int **all, int total, int count, int pp[][2])
{
	int c = 1;
	for (int i = 0; i < total; i++)
	{
		if (all[i][0] != 2)
		{
			cout << setw(4) << c++ << ":   {"; 
			for (int j = 0; j < count; j++)
			{
				if (all[i][j] == 1)
				{
					cout << "(" << pp[j][0] << "," << pp[j][1] <<"),";
				}
			}
			cout << "}" << endl;
		}
	}
}

/* x-cordi, y-cordi 
   p3[x*count + y]
 */
int* getP3(int *p3, int x, int y, int count)
{
	return p3 + x*count + y;
}

/* sub Class judge( p in q or not ) */
bool subClass(int *p, int *q, int count)
{
	for (int i = 0; i < count; i++)
	{
		if (p[i] - q[i] == 1)
			return false;
	}

	return true;
}

/* all element is 0 */
bool allZero(int *p, int count)
{
	int r = 0;
	for (int i = 0; i < count; i++)
		r += p[i];

	if (r == 0) return true;
	return false;
}

void getCompatibleClass(int *p3, int pp[][2], int count)
{
	int maxCC = count * (count - 1) / 2;
	int **ret = new int*[maxCC];
	for (int i = 0; i < maxCC; i++)
	{
		ret[i] = new int[count];
		memset(ret[i], 0, count*sizeof(int));
	}

	int* temp = new int[count];

	int idx = 0, n, m;
	//�������ݱ����к���
	for (n = 0; n < count-1; n++)
	{
		p3[n*count+n] = 1;

		//������
		for (m = count-1; m >= n+1; m--)
		{
			//ȡ����ǰ��
			for (int k = 0; k < count; k++)
				temp[k] = p3[k*count + n];

			//ÿ�н���&����
			for (int i = 0; i < count; i++)
			{
				cout << temp[i] << ",";
				temp[i] = (temp[i] & p3[m*count+i]);
			}
			cout << endl;

			for (int i = 0; i < count; i++)
				cout << temp[i] << " ";
			cout << endl;

			int flag = 0;
			if (allZero(temp, count)) flag = 1;
			//ȥ���ظ�����
			if (flag != 1)
			{
				for (int i = 0; i < idx; i++)
				{
					if (subClass(temp, ret[i], count))
					{
						flag = 1;
						break;
					}
				}
			}

			//���������
			if (flag == 0)
			{
				for (int i = 0; i < count; i++)
				{
					ret[idx][i] = temp[i];
				}
				//�������һ����
				if (m < count-1)
					ret[idx][m+1] = 1;

				idx++;
			}
		}
	}

	//���������
	for (int i = 0; i < idx; i++)
	{
		cout << setw(4) << i << ":   {"; 
		for (int j = 0; j < count; j++)
		{
			if (ret[i][j] == 1)
			{
				cout << "(" << pp[j][0] << "," << pp[j][1] <<"),";
			}
		}
		cout << "}" << endl;
	}

	for (int i = 0; i < maxCC; i++)
		delete []ret[i];
	delete []ret;

	delete temp;

}

int main()
{
	int num1,num2,count=0;
	char *p,*p2;
	int *p3;
	//Ϊ�˷���ֱ��������֤����
	p="ax0x00000dzy000000a0yx00000c0000000xc0000000xd0000000xbyz00000yb";
	num1=8;
	p2="bxyy0000a0xx00y0b00y0000dy0z0000c000000xc000x00ya";
	num2=7;

	int pp[100][2];
	//p = input_mat(&num1);
	//p2 = input_mat(&num2);
	output_mat(p,num1);
	output_mat(p2,num2);
	count = find_pp(p,p2,pp,num1,num2);
	putout_pp(pp,count);

	p3=output_table(pp,count,p,num1,num2,p2);

	/** added code */
	//getCompatibleClass(p3, pp, count);
	int **all = new int*[count*(count-1)/2];
	for (int i = 0; i < count*(count-1)/2; i++)
	{
		all[i] = new int[count];
		memset(all[i], 0, count*sizeof(int));
	}

	int total = 0;
	for (int n = 0; n < count-1; n++)
	{
		buildCompatibleClassTree(p3, count, n, all, &total);
	}

	printCptbClass(all, total, count, pp);

	for (int i = 0; i < count*(count-1)/2; i++)
		delete []all[i];
	delete []all;

	delete p3;
	system("pause");

	return 0;
}
