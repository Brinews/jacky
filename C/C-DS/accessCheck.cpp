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
			cout << "文件不存在\n";
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

		if (name != "姓名" || zjlx != "证件类型" || sfzh != "证件号") {
			cout << "Line:" << line <<  "\t文件头格式不对\n";
			count++;
		}

		while (getline(fin, name, ',')) {
			getline(fin, zjlx, ',');
			getline(fin, sfzh, '\n');

			line++;
			infoCnt++;

			//cout << name << "+" << zjlx << "+" << sfzh << endl;

			if (hasSpace(name)) {
				cout << "Line:" << line <<  "\t姓名格式不对\n";
				count++;
			}

			if (zjlx == "护照") {
				if (sfzh.length() < 3 || !isLetter(sfzh[0])
						|| !isLetter(sfzh[1]) || !isLetter(sfzh[2])) {
					cout << "Line:" << line <<  "\t护照格式不对\n";
					count++;
				}
			} else if (zjlx == "身份证") {
				if (sfzh.length() != 18 || !validDigit(sfzh)) {
					cout << "Line:" << line <<  "\t身份证格式不对\n";
					count++;
				}
			} else {
				cout << "Line:" << line <<  "\t证件类型不对\n";
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

	Checker c("上报报表.csv");
	ret += c.checkFileName();
	ret += c.checkContent();
	c.closeFile();

	if (ret == 0) {
		cout << "文件格式全部正确，共处理" << c.getCount() << "条记录\n";
	}

	system("pause");

	return 0;
}
