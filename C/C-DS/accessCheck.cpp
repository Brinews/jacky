#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>

using namespace std;

class Checker {
private:
	string fname;
	ifstream fin;
	int infoCnt;
public:
	Checker(string filename) : fname(filename) {
		infoCnt = 0;
	}

	int checkFileName() {
		fin.open(fname.c_str());
		if (!fin) {
			cout << "�ļ�������\n";
			return 1;
		}

		return 0;
	}

	bool hasSpace(string name) {
		int i;
		for (i = 0; i < name.length(); i++)
			if (name[i] == ' ') return true;

		return false;
	}

	bool isLetter(char c) {
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
	}

	bool isDigit(char c) {
		return c >= '0' && c <= '9';
	}

	bool validDigit(string sfzh) {
		int i;
		for (i = 0; i < sfzh.length() - 1; i++)
			if (!isDigit(sfzh[i])) return false;

		if (!isDigit(sfzh[i]) && sfzh[i] != 'X' && sfzh[i] != 'x') 
			return false;

		return true;
	}

	int checkContent() {
		int line = 0;
		int count = 0;

		string buf;
		string name, zjlx, sfzh;

		getline(fin, name, ',');
		getline(fin, zjlx, ',');
		getline(fin, sfzh, '\n');

		//cout << name << "+" << zjlx << "+" << sfzh << endl;

		if (name != "����" || zjlx != "֤������" || sfzh != "֤����") {
			cout << "Line:" << line <<  "\t�ļ�ͷ��ʽ����\n";
			count++;
		}

		while (getline(fin, name, ',')) {
			getline(fin, zjlx, ',');
			getline(fin, sfzh, '\n');

			line++;
			infoCnt++;

			//cout << name << "+" << zjlx << "+" << sfzh << endl;

			if (hasSpace(name)) {
				cout << "Line:" << line <<  "\t������ʽ����\n";
				count++;
			}

			if (zjlx == "����") {
				if (sfzh.length() < 3 || !isLetter(sfzh[0])
						|| !isLetter(sfzh[1]) || !isLetter(sfzh[2])) {
					cout << "Line:" << line <<  "\t���ո�ʽ����\n";
					count++;
				}
			} else if (zjlx == "���֤") {
				if (sfzh.length() != 18 || !validDigit(sfzh)) {
					cout << "Line:" << line <<  "\t���֤��ʽ����\n";
					count++;
				}
			} else {
				cout << "Line:" << line <<  "\t֤�����Ͳ���\n";
				count++;
			}
		}
	}

	void closeFile() {
		fin.close();
	}

	int getCount() {
		return infoCnt;
	}
};

int main()
{
	int ret = 0;

	Checker c("�ϱ�����.csv");
	ret += c.checkFileName();
	ret += c.checkContent();
	c.closeFile();

	if (ret == 0) {
		cout << "�ļ���ʽȫ����ȷ��������" << c.getCount() << "����¼\n";
	}

	system("pause");

	return 0;
}
